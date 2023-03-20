// ChartDataSimulatorAddInImp.cpp: implementation of the CChartDataListViewerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "ChartDataSimulatorAddInImp.h"

#include "../Include_Chart/Conversion.h"									// for CDataConversion
#include "../Include_Chart/Dll_Load/IMetaTable.h"							// for _MTEXT()
#include "../Include_Addin_134221/_resource.h"								// for ID_COA_...
#include "../Include_Addin_134221/I134221/_AddInCommand.h"					// for EAI_DRDS_PACKET_END
#include "../Include_Addin_134221/I134221/_ICmdUI.h"						// for ICmdUI
#include "../Include_Addin_134221/I134221/_IChartManager.h"					// for IChartManager
#include "../Include_Addin_134221/I134221/_IBlock.h"						// for IBlock
#include "../Include_Addin_134221/I134221/_IPacket.h"						// for IPacket
#include "../Include_Addin_134221/I134221/_IPacketManager.h"				// for IPacketManager
#include "../Include_Addin_134221/I134221/_IPacketListManager.h"			// for IPacketListManager
#include "../Include_Addin_134221/I134221/_IString.h"						// for ILPCSTR
#include "../Include_Addin_134221/I134221/_IChartOCX.h"						// for IChartOCX
#include "../Include_Addin_134221/I134221/_IStringList.h"					// for IStringList
#include "../Include_Addin_134221/I134221/_IDoubleList.h"					// for IStringList
#include "../Include_Addin_134221/I134221/_IHelpMessageMap.h"				// for IHelpMessageMap

#include "DlgDataSimulator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartDataSimulatorAddInImp::CChartDataSimulatorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_strSimulateRQ.Empty();
	m_nDataType				= -1;
	m_bUpdateRealData		= TRUE;
	m_dLastSimulateDateTime = -1;	
}

CChartDataSimulatorAddInImp::~CChartDataSimulatorAddInImp()
{

}

CString CChartDataSimulatorAddInImp::m_strAddInItemName = "DATA_SIMULATOR";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartDataSimulatorAddInImp)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnPreRealData)
	ONADDINEVENT( OnPreRealPacketEnd)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

void CChartDataSimulatorAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( p_nToolType != CToolOptionInfo::T_DATA_SIMULATOR) return;
	OnAddInToolCommand( EAI_DATA_SIMULATOR);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CChartDataSimulatorAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( ID_DLV_DATA_SIMULATOR != nID) return FALSE;
	if( CN_COMMAND == nCode) return OnAddInToolCommand( EAI_DATA_SIMULATOR);
	else if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( p_pICmdUI) p_pICmdUI->Enable( TRUE);
		return TRUE;
	}
	return TRUE;
}

BOOL CChartDataSimulatorAddInImp::GetDataFromPacketList(int nColIndex, LPCTSTR szRQ, LPCTSTR szPacketName, 
														CChartDataGridManager * pOutChartDataGridManager, BOOL bAdd)
{
	if(!pOutChartDataGridManager) 
		return FALSE;

	IPacket * pPacket = m_pIPacketListManager->GetPacket(szRQ, szPacketName);
	if(!pPacket) return FALSE;

	ILPCSTR IPacketType	= pPacket->GetType();
	CString strPacketType=IPacketType;

	double dData;
	CString strData;
	POSITION pos, posChartDataList;
	CChartRecordSet * pChartRecordSet = NULL;
	IDoubleList * pDataList = pPacket->GetDataList();
	if(!pDataList) 
	{
		pPacket->Release();
		return FALSE;
	}

	CChartDataGridHeaderInfo * pHeaderInfo = new CChartDataGridHeaderInfo;
	pHeaderInfo->m_strHeader = szPacketName;	
	pHeaderInfo->m_strPacketType = strPacketType;
	pHeaderInfo->m_strTitle  = _MTOL0( szPacketName );
	pOutChartDataGridManager->m_ChartDataGridHeaderInfoArray.Add(pHeaderInfo);

	CChartDataList * pOutChartDataList = &(pOutChartDataGridManager->m_ChartDataList);
	
	//bAdd = true�� ��� list�� GetCount == 0�̾�� �Ѵ�.
	if(bAdd)
	{
		if(pOutChartDataList->GetCount() > 0) 
		{
			pDataList->Release();
			pPacket->Release();
			return FALSE;
		}
		else
		{
			for(pos = pDataList->GetHeadPosition(); pos ; )
			{
				if(!pDataList->GetNext(pos, dData)) 
					goto error;

				strData = CDataConversion::DoubleToString(dData);
				//strData = CDataConversion( m_hOcxWnd).GetStringFormatTrans( strData, strPacketType);

				pChartRecordSet	= new CChartRecordSet;		
				pChartRecordSet->m_Data[nColIndex]	= strData;								

				pOutChartDataList->AddTail(pChartRecordSet);
			}
		}
	}
	else if(!bAdd)
	{
//		if(pDataList->GetCount() != pOutChartDataList->GetCount())
//		{			
//			pDataList->Release();
//			pPacket->Release();
//			return FALSE;
//		}
//		else
		{

			//20081117 �̹��� ��������� ó�� �߰�>>
			int iIndex = 0;
			//20081117 �̹��� <<
			for(pos = pDataList->GetHeadPosition(), posChartDataList = pOutChartDataList->GetHeadPosition();
					pos && posChartDataList; iIndex++ )//20081117 �̹��� ��������� ó�� �߰�>>
			{
				if(!pDataList->GetNext(pos, dData)) 
					goto error;

				strData = CDataConversion::DoubleToString(dData);
				strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strData, strPacketType);				

				pChartRecordSet	= pOutChartDataList->GetNext(posChartDataList);
				pChartRecordSet->m_Data[nColIndex]	= strData;
			}

			//20081117 �̹��� ��������� ó�� �߰�>>
			int nDiff = pOutChartDataList->GetCount() - iIndex;
			if(nDiff > 0)
			{
				for(iIndex = 0 ; iIndex < nDiff && !pOutChartDataList->IsEmpty(); iIndex++)
				{
					CChartRecordSet * pSet = pOutChartDataList->RemoveTail();
					delete pSet;
				}

			}
			//20081117 �̹��� ��������� ó�� �߰�<<
		}
	}

	pDataList->Release();
	pPacket->Release();
	return TRUE;

error:
	{
		pDataList->Release();
		pPacket->Release();

		return FALSE;
	}
}

BOOL CChartDataSimulatorAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( EAI_DATA_SIMULATOR != p_eCommandID) return FALSE;

	ML_SET_LANGUAGE_RES();

	if(!m_pIChartOCX || !m_pIChartManager || !m_pIPacketListManager) return FALSE;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Ư����Ʈ(�Ｑ��ȯ��, P&F, ���ð�)�� �ڷ����ڿʹ� ������ ��ǥ���̴�.
	// �׷��Ƿ� ��Ƽ��Ʈ ����Ÿâ ����� ���ǹ��ϴ�.
	ILPCSTR szGraphName = m_pIChartManager->GetAllIndicatorNameString();
	CString strGraphName( szGraphName);
	if( g_iMetaTable.IsSpecialGroupGraph(strGraphName.Left(strGraphName.Find(_T(","))))) return FALSE;

	int			col;
	CString		strRQ;
	ILPCSTR		IPacketType, IRQ;
	LPCTSTR     szPacketName[ COL_SIZE] = { _MTEXT(C0_DATE_TIME), _MTEXT(C0_OPEN) , _MTEXT(C0_HIGH) , _MTEXT(C0_LOW) , 	_MTEXT(C0_CLOSE), _MTEXT(C0_VOLUME)};
	CChartDataGridManager ChartDataGridManager;	

	IRQ	= m_pIChartOCX->GetCurrentRQ();
	strRQ = IRQ;
	
	for( col = 0 ; col < COL_SIZE; col++ )
	{
		// (2009/9/25 - Seung-Won, Bae) for FX
		if( COL_SIZE - 1 == col) if( CChartInfo::FX == m_eChartMode) break;

		if(!GetDataFromPacketList(col, IRQ, szPacketName[col], &ChartDataGridManager, col == 0))
			return FALSE;		
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//Ÿ��Ʋ ������ ���� ó�� 
	CString strTitle;
	ILPCSTR IKey,	IValue;
	CString strKey, strValue, strHelpMsg;
	ILPCSTR szHelpMsg = NULL;
	CString strDataType, strDataInterval;	
	IPacketManager * pCurPacketManager = m_pIPacketListManager->GetPacketManager(IRQ);
	if(!pCurPacketManager) return FALSE;	


	////////////////////////////////////////////////////////////////////////////////////////////	
	//Ÿ��Ʋ ������ ���� ó��(�����, �����ڵ� ������)
	LPCTSTR lpcszCode = NULL;
	LPCTSTR lpcszName = NULL;
	CString strItemCode, strItemName;
	
	m_pIPacketListManager->GetItemCode(IRQ, lpcszCode, lpcszName);
	if(lpcszCode && strlen(lpcszCode) > 0)
	{
		strItemCode = CString(lpcszCode, strlen(lpcszCode));
		if( strItemCode.GetAt( 0) == '|' && strItemCode.GetLength() >= 4 )
			strItemCode = strItemCode.Mid( 3);
	}
	if(lpcszName && strlen(lpcszName) > 0)
		strItemName = CString(lpcszName, strlen(lpcszName));

	////////////////////////////////////////////////////////////////////////////////////////////
	//Ÿ��Ʋ ������ ���� ó�� (��,��,��,��,ƽ-�ֱ�)
	pCurPacketManager->GetHelpMessageItem( "UD", szHelpMsg);	strDataType = szHelpMsg;
	pCurPacketManager->GetHelpMessageItem( "UT", szHelpMsg);	strDataInterval=szHelpMsg;	if(strDataInterval.IsEmpty()) strDataInterval = "1";
	pCurPacketManager->Release();

	int nDataType = atoi(strDataType) - 1;	
	int nDataInterval = atoi(strDataInterval);	
	

	//////////////////////////////////////////////////////////////////////////
	// ���� �޽��� ���	
	POSITION	pos;
	CString		strDateTime;
	IHelpMessageMap * pHelpMsgMap = pCurPacketManager->GetHelpMessageMap();
	if(pHelpMsgMap)
	{		
		pos = pHelpMsgMap->GetStartPosition();
		while(pos)
		{
			pHelpMsgMap->GetNextAssoc(pos, IKey, IValue);
			strKey = IKey;	strValue=IValue;
			// (2008/11/3 - Seung-Won, Bae) Set UM first.
			if( strKey != "UM") strHelpMsg.Insert(0, strKey + "=" + strValue + "@");
			if( strKey == "RDATET") strDateTime = strValue;
		}
		// (2008/11/3 - Seung-Won, Bae) Set UM first.
		strHelpMsg.Insert( 0, "UM=E@");

		pHelpMsgMap->Release();
	}

	//////////////////////////////////////////////////////////////////////////
	// user file path
	ILPCSTR IUserFilePath = m_pIChartOCX->GetUserFilePathString();

	//////////////////////////////////////////////////////////////////////////////////////////////
	// ��ȭ���� ����..
	CDlgDataSimulator dlg( m_eChartMode);
	dlg.m_hOcxWnd						= m_hOcxWnd;
	dlg.m_strUserFilePath				= IUserFilePath;
	ChartDataGridManager.m_nDataType	= nDataType;
	ChartDataGridManager.m_nDataInterval= nDataInterval;
	ChartDataGridManager.m_strDateTime	= strDateTime;
	ChartDataGridManager.m_strItemCode	= strItemCode;
	ChartDataGridManager.m_strItemName	= strItemName;	
	ChartDataGridManager.m_strHelpMessage= strHelpMsg;
	dlg.m_pChartDataGridManager			= &ChartDataGridManager;
	if(dlg.DoModal() == IDOK)
	{

		SetDataToChartOCX(pCurPacketManager, &ChartDataGridManager, dlg.m_strLastDateTime);
		m_strSimulateRQ			= strRQ;
		m_dLastSimulateDateTime = atof(dlg.m_strLastDateTime);	
		m_nDataType				= ChartDataGridManager.m_nDataType;

		// (2008/11/3 - Seung-Won, Bae) Modify Last Simulate DateTime to Original.
		IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( strRQ);
		if( pIPacketManager)
		{
			int nYear = int( m_dLastSimulateDateTime / 10000000000.);
			int nMonth = int( m_dLastSimulateDateTime / 100000000. - nYear * 100) ;
			int nDay = int( m_dLastSimulateDateTime / 1000000. - nYear * 10000. - nMonth * 100.) ;
			int nHour = int( m_dLastSimulateDateTime / 10000. - nYear * 1000000. - nMonth * 10000. - nDay * 100.) ;
			m_dLastSimulateDateTime -= ( nYear * 1000000. + nMonth * 10000. + nDay * 100. + nHour) * 10000.;
			nHour -= pIPacketManager->GetCalcDataTimeDifference();
			if( nHour < 0)
			{
				nHour += 24;
				nDay--;
			}
			if( nDay < 1)
			{
				nDay = 31;
				nMonth--;
				switch( nMonth)
				{
					case 2:	nDay = 29;
							break;
					case 4:
					case 6:
					case 9:
					case 11:	nDay = 30;
								break;
				}
			}
			if( nMonth < 1)
			{
				nMonth = 12;
				nYear--;
			}
			m_dLastSimulateDateTime += ( nYear * 1000000. + nMonth * 10000. + nDay * 100. + nHour) * 10000.;
			pIPacketManager->Release();
		}
	}


	return TRUE;
}

BOOL CChartDataSimulatorAddInImp::SetDataToChartOCX(IPacketManager * pCurPacketManager, CChartDataGridManager * pChartDataGridManager,CString strLastDateTime)
{
	if(!pChartDataGridManager) return FALSE;

	if(pChartDataGridManager->m_strHelpMessage.IsEmpty())
	{
		AfxMessageBox("�����޼��� ������ �̻��� �ֽ��ϴ�.");
		return FALSE;	
	}

	// (2008/10/29 - Seung-Won, Bae) Remove TDC.
	CString strHelpMsg( pChartDataGridManager->m_strHelpMessage);
	int nTdcStart = strHelpMsg.Find( "TDC=");
	if( 0 <= nTdcStart)
	{
		CString strNewMsg = strHelpMsg.Left( nTdcStart + 4);
		int nTdcEnd = strHelpMsg.Find( "@", nTdcStart);
		if( 0 <= nTdcEnd) strNewMsg = strNewMsg + strHelpMsg.Mid( nTdcEnd);
		strHelpMsg = strNewMsg;
	}


//20081107 �̹��� >>
//�����޼��� ���� �ּ�ó��
//	CString strKey = "DATATIME=", strReplace="DATATIME=" + strLastDateTime + "@";
//	CString strDateTime, strDateTime2;
//	int nPosEnd, nPosStart = strHelpMsg.Find(strKey);
//	if(nPosStart >= 0)
//	{
//		if((nPosEnd = strHelpMsg.Find("@", nPosStart)) >= 0)
//		{
//			CString strTemp = strHelpMsg.Left(nPosStart) + strReplace + strHelpMsg.Mid(nPosEnd+1);
//			strHelpMsg		= strTemp;
//		}
//		else
//		{
//			CString strTemp = strHelpMsg.Left(nPosStart) + strReplace;
//			strHelpMsg		= strTemp;
//		}
//	}
//	else
//	{
//		strHelpMsg = strReplace + strHelpMsg;
//	}
//	strKey = "MARKETDAY=", strReplace = "MARKETDAY=" + strLastDateTime.Left(8) + "@";
//	nPosStart = strHelpMsg.Find(strKey);
//	if(nPosStart >= 0)
//	{
//		if((nPosEnd = strHelpMsg.Find("@", nPosStart)) >= 0)
//		{
//			CString strTemp = strHelpMsg.Left(nPosStart) + strReplace + strHelpMsg.Mid(nPosEnd+1);
//			strHelpMsg		= strTemp;
//		}
//		else
//		{
//			CString strTemp = strHelpMsg.Left(nPosStart) + strReplace;
//			strHelpMsg		= strTemp;
//		}
//	}
//	else
//	{
//		strHelpMsg = strReplace + strHelpMsg;
//	}
//20081107 �̹��� <<


	m_pIChartOCX->SetBinTrHelpMsg( strHelpMsg);	// SetBinTr�� ���� ���� �޽��� ���� 
	UINT nMaxRows = pChartDataGridManager->m_ChartDataList.GetCount();
	UINT nMaxCols = pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetSize();
	CString strPacketName;
	UINT i, j;
	double* p_dBuf;
	POSITION pos;
	//////////////////////////////////////////////////////////////////////////
	// data import
	for( i = 0 ;i < nMaxCols ;i++ )
	{
		p_dBuf = new double[nMaxRows];
		memset(p_dBuf,0,sizeof(double)*(nMaxRows));
		
		// Packet �� ���
		CChartDataGridHeaderInfo * pHeaderInfo = pChartDataGridManager->m_ChartDataGridHeaderInfoArray.GetAt(i);
		strPacketName = pHeaderInfo->m_strTitle;
		
		IPacket *pIPacket = NULL;
		CString strDate;
		strDate.LoadString(IDS_DATE);
		if(strPacketName.Find(strDate) >= 0)
		{
			pIPacket		= pCurPacketManager->GetPacket(_MTEXT(C0_DATE_TIME));
			strPacketName	= _MTEXT(C0_DATE_TIME);
		}				
		else if(strPacketName.Find( _LTEXT3( C3_VOLUME)) >= 0)
		{
			pIPacket		= pCurPacketManager->GetPacket(_MTEXT(C0_VOLUME));
			strPacketName	= _MTEXT(C0_VOLUME);
		}
		else
		{
			strPacketName	= _LTOM0( strPacketName);
			pIPacket		= pCurPacketManager->GetPacket( strPacketName);
		}
		
		if(!pIPacket) continue;					
		
		pIPacket->ClearData(true);
		

		for( j = 0 , pos = pChartDataGridManager->m_ChartDataList.GetHeadPosition(); pos && j < nMaxRows ; j++ )
		{
			CChartRecordSet * pRecordSet = pChartDataGridManager->m_ChartDataList.GetNext(pos);

			switch(i)
			{
			case DATE_TIME:
				{
					//p_dBuf[nMaxRows - 1 - j] = atof(pRecordSet->m_Data[i]);
					p_dBuf[j] = atof(pRecordSet->m_Data[i]);
				}
				break;
			default:
				{
					CString strInput;
					CString strOutput;
					strInput = pRecordSet->m_Data[i];
					strInput.Remove(',');
					if(!CDataConversion::GetDataToDec(strInput, pHeaderInfo->m_strPacketType, strOutput))
						//p_dBuf[nMaxRows - 1 - j] = 0.;
						p_dBuf[j] = 0.;
					else
						//p_dBuf[nMaxRows - 1 - j] = atof(strOutput);
						p_dBuf[j] = atof(strOutput);
				}
				break;
			}
		}					

		m_pIChartOCX->SetBinTrDoubleData(strPacketName, nMaxRows, p_dBuf);
		delete []p_dBuf;
		pIPacket->Release();
	}
	
	// import end
	m_pIChartOCX->SetBinTrEnd();

	return TRUE;
}


CString CChartDataSimulatorAddInImp::GetDataWindowRealPacketNames(const char *szTitle, const char *szPacketNames) const
{
	CString strRealPacketNames = szPacketNames;

	if( !strcmp( szTitle, _MTEXT( C3_PRICE)) && strRealPacketNames.Find( _MTEXT( C0_CLOSE)) > 0)
	{
		if( m_pIDefaultPacketManager->GetBaseTimeDataType() == CScaleBaseData::HORZ_TICK && m_pIDefaultPacketManager->GetBaseTimeUnit() == 1)
			strRealPacketNames = _MTEXT( C0_CLOSE);
	}

	if(strRealPacketNames.Right(1) != ",")
		strRealPacketNames += ",";
	
	return strRealPacketNames;
}

BOOL CChartDataSimulatorAddInImp::OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ)
{
	CString strRQ = p_szRQ;
	if(m_strSimulateRQ == strRQ)
	{
//		int nPos;
//		CString strPacketName = p_szPacketName;
//		if((nPos = strPacketName.Find( _MTEXT( C0_DATE_TIME) )) >= 0)
//		{
//			if(m_dLastSimulateDateTime >= p_dData)
//			{			
//				m_bUpdateRealData = FALSE;
//				return FALSE;
//			}
//			else
//				m_bUpdateRealData = TRUE;
//		}
//		else if(!m_bUpdateRealData)
//			return FALSE;

		int nPos;
		CString strPacketName = p_szPacketName;
		if((nPos = strPacketName.Find( _MTEXT( C0_DATE_TIME) )) >= 0)
		{
			if(m_dLastSimulateDateTime >= p_dData)
			{		
				if(m_nDataType == MIN_TYPE)
					p_dData = m_dLastSimulateDateTime-50;
				else
					p_dData = m_dLastSimulateDateTime;

				m_bUpdateRealData = FALSE;
				return TRUE;
			}
			else
				m_bUpdateRealData = TRUE;
		}
		else if(!m_bUpdateRealData)
			return TRUE;
	}
	
	return TRUE;
}

BOOL CChartDataSimulatorAddInImp::OnPreRealPacketEnd( const char *p_szRQ)
{
	CString strRQ = p_szRQ;
	if(m_strSimulateRQ == strRQ)
	{
		//if(!m_bUpdateRealData)
		//	return FALSE;
	}
	return TRUE;
}

void CChartDataSimulatorAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2004.10.13, ��¿�) �켱 �����Ѱ� ���� Message�� ������ �������� Ȯ���Ѵ�.
	// (2004.10.22, ��¿�) ����/���Ѱ� ǥ�� ���� ���� ���Ϻ� �ʱ�ȭ �����
	//			��� ���� ��Ʈ�� �ִ� ������ �ʱ�ȭ TR�� ���Ž����� �ش��Ѵ�.
	//		�� ��ɵ��� ���� ������Ʈ�� ����/������ ����/������ ����Ʈ�� �����ϸ鼭,
	//			��/���Ѱ��� ǥ���ϰų� ���� ���� ǥ�ø� Reset ��Ű�� ����� ó���ϴ� ���̴�.
	//		Ư�� �� ��ɵ��� ��Ʈ�� 1���� Graph���� ������ �� �ִ� �������
	//			(����� Data ������ AddIn �� 1���� ���ѵ� ������� �����̴�.)
	//			���� Packet�� �̿��ϴ� ������Ʈ���� �����Ǵ� ������� �����Ѵ�.
	//		*. ���� �ڼ��� ������ �� Class Header ����
	
	//if( !( BOOL)strstr( p_szPacketNames, CString( _MTEXT( C0_CLOSE)) + ";") || !p_bWithReset) return;

	// (2004.10.14, ��¿�) ���� ���Ϻ� �ʱ�ȭ �ð��� Ȯ���Ѵ�.
	//ILPCSTR szHelpMsg;
	//if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->GetHelpMessageItem( "RELPSA", szHelpMsg);
	//ResetRELPSA( szHelpMsg);

	m_strSimulateRQ.Empty();
	m_nDataType				= -1;
	m_bUpdateRealData		= TRUE;
	m_dLastSimulateDateTime = -1;	
}