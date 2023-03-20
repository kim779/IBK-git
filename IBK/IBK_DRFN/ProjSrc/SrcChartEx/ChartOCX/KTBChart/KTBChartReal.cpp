// KTBChartReal.cpp: implementation of the Real Processing Routine
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/DLL_Load/IMetaTable.h"			// for _STOM0()
#include "../Include_Chart/DLL_Load/XScaleManager.h"		// for CXScaleManager
#include "../Include_Chart/IRealReceiver.h"					// for IRealReceiver
#include "../Include_Chart/IOldFXRealReceiver.h"			// for IOldFXRealReceiver
#include "../Include_Chart/IOldHTSRealReceiver.h"			// for IOldHTSRealReceiver
#include "../Include_Chart/MessageDefine.h"					// for UDM_RECALC_DRI
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_LOG_REAL_SETTING_CHANGED
#include "KTBChartCtl.h"									// for CKTBChartCtrl
#include "Conversion.h"										// for CDataConversion
#include "MainBlock.h"										// for CMainBlock
#include "ToolBarManager.h"									// for IToolBarManager
#include "LocalShareInfoList.h"								// for CLocalShareInfo
#include "PacketList.h"										// for CPacketList
#include "PacketListManager.h"								// ���� PacketList�� ���� : �������� - ojtaso (20061224)
#include "OutsideCommonInterface.h"								// �������� ����/���� : �������� - ojtaso (20070209)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// (2004.11.17, ��¿�, �м�) Real ���� Property ó�� Routine
//////////////////////////////////////////////////////////////////////

// (2004.11.17, ��¿�, �м�) m_bUseRealData (Real �̿뿩��) ���� ó�� Routine�̴�.
//	(����! �׷��� ������ FALSE�� ������ Map������ Script�� TRUE�� �����Ͽ� �̿��� �� ����.
//		DRDS DLL�� Loading���� �ʱ� �����̴�.)
void CKTBChartCtrl::OnbUseRealDataChanged() 
{
	BoundPropertyChanged(dispidBUseRealData);

	SetModifiedFlag();
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/3/2
// Modifier		: 
// Comments		: Change Real Setting String with Adising
//-----------------------------------------------------------------------------
BOOL CKTBChartCtrl::ResetRealListString(LPCTSTR p_szRealList, BOOL p_bAdviseNow) 
{
	// TODO: Add your dispatch handler code here

	_ML_TEST_NOT_USED();
	// 1. Reset Real Setting String
	m_strRealList = p_szRealList;

	// 2. Update Real Setting Info
	OnstrRealListChanged();

	// 3. Advise Now
	if( p_bAdviseNow && m_pIRealReceiver)
	{
		m_pIRealReceiver->SetRealAdvise( FALSE);
		m_pIRealReceiver->SetRealAdvise( TRUE);
	}

	return TRUE;
}

// (2004.11.17, ��¿�, �м�) m_strRealList (Real ����)�� ���� ó���ϴ� Routine�̴�.
//  (����! ������ �� ������ Real�� ó���Ǳ� ���ؼ��� m_AdviseList�� �ڷᱸ���� �籸�� �Ͽ��� �ϴµ�
//		�̴� ������ ����Ǿ� �����Ǿ�� �ݿ��ȴ�.)
void CKTBChartCtrl::OnstrRealListChanged() 
{
	BoundPropertyChanged( dispidStrRealList);

	_ML_TEST_NOT_USED();
	SetModifiedFlag();

	// (2005/3/2 - Seung-Won, Bae) Update Real Setting Info
	// (2006/2/3 - Seung-Won, Bae) Show Real Setting Error Message
	if( m_pIRealReceiver) if( !m_pIRealReceiver->ResetRealSettingInfo( m_strRealList))
	{
		AfxMessageBox( "Error in real list string setting!\r\nPlease, check string again!\r\n\r\n" + m_strRealList);
		return;
	}

	// (2005/9/29 - Seung-Won, Bae) Log the New Real Setting
	if( m_bInLogging && m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_LOG_REAL_SETTING_CHANGED, m_strRealList);
}

// (2004.11.26, ��¿�) Real�� ����Ʈ�� ����������� ��ü �ð��� ������ �������� Flag���� ���� �����Ѵ�.
//		Ư��, ������ Global�� �Ӽ��� �ξ� HTS �⺻ ������ �����ϰ�,
//		������ Property�� �����Ͽ� Chart�� �Ӽ��� ������� �Ѵ�.
void CKTBChartCtrl::OnbNormalizedMinuteTimeRule() 
{
	BoundPropertyChanged( dispidBNormalizedMinuteTimeRule);

	SetModifiedFlag();

	// (2009/11/1 - Seung-Won, Bae) for Nomalized Minute Time Rule.
	if( CChartInfo::FX == m_nChartMode) m_bNormalizedMinuteTimeRule = TRUE;

// (2009/11/23 - Seung-Won, Bae) for DEV
//#ifdef _DEBUG
//	char szChartMode[ 256];
//	DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szChartMode, 255, GetBaseDeskPath() + DATA_FILE_FOLDER + "ChartMetaTable.dat");
//	CString strChartMode( szChartMode);
//	if( strChartMode == "HTS") m_bNormalizedMinuteTimeRule = TRUE;
//#endif

	// (2005/11/7 - Seung-Won, Bae) Log for Normalized Minute Time Rule
	if( m_bInLogging && m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_LOG_SET_IMAGENARY_CANDLE, "CKTBChartCtrl::OnbNormalizedMinuteTimeRule", m_bNormalizedMinuteTimeRule);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.17, ��¿�, �м�) Real Data Process Routine & Method
//////////////////////////////////////////////////////////////////////

// (2004.12.30, ��¿�, �м�) Real�� 1�� Field Data�� �����ϴ� Routine�̴�.
//		Notify Routine of 1 Real Field
//			int			wParam	~ Size of DrdsData, Fixed Value
//			DrdsData *	lParam	~ Data Structure
LONG CKTBChartCtrl::OnDrdsData( UINT wParam, LPARAM lParam)
{
	if( !m_pIRealReceiver) return 0;
	return m_pIRealReceiver->SetDrdsData( wParam, lParam);
}

// (2004.12.30, ��¿�, �м�) Real�� 1�� Packet ������ ���� ����� ó���̴�.
//		Notify Routine of 1 Real Packet Process End
//			const char *wParam	~ Packet Name
//			const char *lParam	~ Key Code
LONG CKTBChartCtrl::OnDrdsPacketEnd(UINT wParam, LPARAM lParam)
{
	if( !m_pIRealReceiver) return 0;
	return m_pIRealReceiver->SetDrdsPacketEnd( wParam, lParam);
}

// (2004.01.06, ��¿�) BigBang�� MarketProfile Chart�� ���Ͽ� Interface�� Porting�Ѵ�.
//	����	:	nOption == 1	- update
//				nOption == 2	- append
//sy 2004.07.16. -> packet �� ������ ��ŭ scroll ����
void CKTBChartCtrl::SetRealData(LPCTSTR strPacketName2, double dData, short nOption) 
{
	if(!m_pPacketList)
		return;

	CString strPacketName = _STOM0( strPacketName2);
	// ADD: ����(04/08/03) PacketData���� PacketData���ŷ�ƾ�߰��� ���� �� ���� Flag�� ����.
	int nRemovePacketDataCount = 0;
	int nResult = m_pPacketList->SetRealData( strPacketName, dData, nRemovePacketDataCount, nOption);
	if(nResult <= 0)
		return;
	
	// return : 0(ó�� �ȵ�)  1(ó���� : packet������ �ȵ�)  2(ó���� : packet���� ��)
	//sy 2003.3.7 - �ӵ� ���� - ��ũ��Ʈ�� �ǽð� data����
	if(m_pMainBlock != NULL)
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::REALDATA_CAL);

	// (2004.12.07, ��¿�) Real�� �ݿ��� ���� ��ǥ ����� ������ AddIn Module���� �˸���.
	if( m_pAddInManager) m_pAddInManager->OnManualRealData( strPacketName, dData, nOption);

	if( nRemovePacketDataCount)
	{
		// (2006/6/5 - Seung-Won, Bae) Notify to ST AddIn for Packet Data Cutting.
		if( m_pAddInManager) m_pAddInManager->OnPacketTruncated( nRemovePacketDataCount);

		// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
		if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal( TRUE);
	}

	// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
	if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal();

	InvalidateControl();
	// packet �� ������ ��ŭ scroll ����
	//if(nResult == 1)
	//	SetScrollData_RemovePacketPart();

	return;
}

// ���������� ���� PacketList�� RealData : �������� - ojtaso (20061224)
void CKTBChartCtrl::SetRealDataCode(LPCTSTR strCode, LPCTSTR strPacketName2, double dData, short nOption) 
{
	if(!m_pPacketListManager)
		return;

	CString strPacketName = _STOM0( strPacketName2);
	// ADD: ����(04/08/03) PacketData���� PacketData���ŷ�ƾ�߰��� ���� �� ���� Flag�� ����.
	int nRemovePacketDataCount = 0;
	m_pPacketListManager->SetRealData(strCode, strPacketName, dData, nOption);
	
	// return : 0(ó�� �ȵ�)  1(ó���� : packet������ �ȵ�)  2(ó���� : packet���� ��)
	//sy 2003.3.7 - �ӵ� ���� - ��ũ��Ʈ�� �ǽð� data����
	if(m_pMainBlock != NULL)
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::REALDATA_CAL);

	// (2004.12.07, ��¿�) Real�� �ݿ��� ���� ��ǥ ����� ������ AddIn Module���� �˸���.
	if( m_pAddInManager) m_pAddInManager->OnManualRealData( strPacketName, dData, nOption);

	if( nRemovePacketDataCount)
	{
		// (2006/6/5 - Seung-Won, Bae) Notify to ST AddIn for Packet Data Cutting.
		if( m_pAddInManager) m_pAddInManager->OnPacketTruncated( nRemovePacketDataCount);

		// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
		if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal( TRUE);
	}

	// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
	if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal();

	InvalidateControl();
	// packet �� ������ ��ŭ scroll ����
	//if(nResult == 1)
	//	SetScrollData_RemovePacketPart();

	return;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.17, ��¿�, �м�) Real Setting Methods
//////////////////////////////////////////////////////////////////////

// (2004.11.17, ��¿�, �м�) 1�� Real�� �ٷ� ���� �� ��û�ϴ� Method�̴�.
// (2004.05.18, ��¿�) DLL Container�κ��� Real Item�� �����޴� Interface�� �����Ѵ�.
BOOL CKTBChartCtrl::AddRealItem(LPCTSTR szRealInfo) 
{
	// TODO: Add your dispatch handler code here
	return AddRealItemWithSeparator( szRealInfo, ".");
}
// (2006/4/28 - Seung-Won, Bae) for Compare Chart in Future & Option of SK
//		Packet Name can have '.'. So, We need new separator.
BOOL CKTBChartCtrl::AddRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar) 
{					
	// TODO: Add your dispatch handler code here
	if( !IsRunTimeMode() || !m_pPacketList || !szRealInfo || !szSeparatorChar)	return FALSE;
	if( !*szRealInfo || !*szSeparatorChar)										return FALSE;
	if( !m_pIRealReceiver)														return FALSE;
	// 1. Real Setting Info�� �߰��Ѵ�.
	CString strShareName, strSharedData;
	// (2006/2/3 - Seung-Won, Bae) on Error, return FALSE
	if( !m_pIRealReceiver->AddRealSettingInfoRQ(m_strRQ, szRealInfo, &strShareName, &strSharedData, *szSeparatorChar)) return FALSE;

	// 2. Local Shared Info�� �߰��Ѵ�.
	if( m_plLocalShareInfoList) m_plLocalShareInfoList->AddLocalShareInfo( strShareName, 0, strSharedData);

	return TRUE;
}

// (2004.11.17, ��¿�, �м�) 1�� Real�� �ٷ� ��� �� �����ϴ� Method�̴�.
// (2004.05.24, ��¿�) ������ Real Item�� �����ϴ� Interface�� �����Ѵ�.
// (2004.11.18, ��¿�, ����) ���� m_AdviseList�� ��� ���� ��Ŀ� �־ Data Packet�� Key�� �̿����� �ʴ� �����,
//		���� Real Data (�ϳ��� Field��) �������� Data Packet�� �������� ���Ѵ�.
void CKTBChartCtrl::RemoveRealItem(LPCTSTR szRealInfo) 
{
	// TODO: Add your dispatch handler code here
	RemoveRealItemWithSeparator( szRealInfo, ".");
}
// (2006/4/28 - Seung-Won, Bae) for Compare Chart in Future & Option of SK
//		Packet Name can have '.'. So, We need new separator.
void CKTBChartCtrl::RemoveRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar) 
{					
	// TODO: Add your dispatch handler code here
	if( !IsRunTimeMode() || !m_pPacketList)	return;
	if( !szRealInfo || !*szRealInfo)		return;
	if( !m_pIRealReceiver)					return;

	// 1. Real Setting Info�� �߰��Ѵ�.
	CString strShareName;
	m_pIRealReceiver->RemoveRealSettingInfoRQ(m_strRQ, szRealInfo, &strShareName, *szSeparatorChar);

	// 2. Local Shared Info���� �����Ѵ�.
	if( !strShareName.IsEmpty() && m_plLocalShareInfoList) m_plLocalShareInfoList->RemoveLocalShareInfo( strShareName);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.17, ��¿�, �м�) Real On/Off Method & Routine
//////////////////////////////////////////////////////////////////////

// (2004.11.17, ��¿�, �м�) Real�� �Ͻ� ������Ű��, �ٽ� ��û�ϴ� Method Interface�̴�.
BOOL CKTBChartCtrl::SetDrdsAdvise(BOOL bOnAdvise) 
{
	if( !m_pIRealReceiver) return FALSE;

	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	m_pIRealReceiver->SetRealAdviseRQ(m_strRQ, bOnAdvise);

	return TRUE;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetRealSettingInfo
 - Created at  :  2005-05-24   13:56
 - Author      :  ������
 - Return Value:  BSTR - ������ ����Ÿ
 - Description :  ��Ʈ�� ���õ� ������ ����Ÿ�� �����´�.
 -----------------------------------------------------------------------------------------*/
BSTR CKTBChartCtrl::GetRealSettingInfo() 
{
	CString strResult = _T("");
	if( m_pIRealReceiver) m_pIRealReceiver->GetRealAdvisedInfo( strResult);
	return strResult.AllocSysString();
}

// RQ�� Real���/���� : �������� - ojtaso (20070119)
void CKTBChartCtrl::SetDrdsAdviseRQ(LPCTSTR lpszRQ, BOOL bOnAdvise) 
{
	if(m_pIRealReceiver) 
	{
		if(!bOnAdvise)
			m_pIRealReceiver->RemoveRQPacket(lpszRQ);
		
		m_pIRealReceiver->SetRealAdviseRQ(lpszRQ, bOnAdvise);
	}
}

// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
void CKTBChartCtrl::SetFirstNMTRClock( void)
{
	if( !m_pPacketListManager || !m_pIRealReceiver) return;

	_int64 nClock = 0;

	IOldFXRealReceiver	*pIOldFXRealReceiver = ( IOldFXRealReceiver *)m_pIRealReceiver;
	IOldHTSRealReceiver	*pIOldHTSRealReceiver = ( IOldHTSRealReceiver *)m_pIRealReceiver;
	// (2009/9/5 - Seung-Won, Bae) Support Old MChart Mode
	char szMChartMode[ 256];
	DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szMChartMode, 255, GetDefaultPath() + "ChartMetaTable.dat");
	CString strChartMode( szMChartMode);
	if(			strChartMode == "FX")		nClock = pIOldFXRealReceiver->GetNMTRClock();
	else if(	strChartMode == "WORLDON")	nClock = m_pIRealReceiver->GetNMTRClock();
	else if(	strChartMode == "HTS")		nClock = pIOldHTSRealReceiver->GetNMTRClock();
	//<<20100310_JS.Kim	�ַθ�.. HTS/�ؿܼ���/FX�� ���� ChartMetaTable.dat�� ���Ƿ� ���⿡ ������ �� ���� ���� �ȵǾ� �ִ� ���� �Ϲ������� ó��
	else									nClock = m_pIRealReceiver->GetNMTRClock();
	//>>
	
	nClock += 5 - nClock % 5;
	m_pPacketListManager->OnNMTRClock( nClock, m_pIRealReceiver);
}

