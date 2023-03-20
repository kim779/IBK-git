// DrdsRealReceiverAddInImp.cpp: implementation of the CDrdsRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "DrdsRealReceiverAddInImp.h"

#include "../../Include_Chart/Dll_Load/XScaleManager.h"		// XScaleManager �߰� : �������� - ojtaso (20070118)
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/PropertiesData.h"				// for g_aszAddInLoadString
#include "../Include_AddIn/AddInCommand.h"					// for EAI_LOG_DRDS_ADVISE
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IPacketListManager.h"	// for IPacketListManager
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "RealSettingInfoList.h"							// for CRealSettingInfoList
#include "DrdsAdvisedInfoList.h"							// for CDrdsAdvisedInfoList
#include "RsiPacketGroup.h"									// for CRsiPacketGroup
#include "DaiPacketGroup.h"									// for CDaiPacketGroup

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.11.25, ��¿�) ������� ���� ���� Real Meta Name�� �����Ѵ�.
// (2004.12.30, ��¿�) Ư�� �帶������ TEST Real�� TEST ����� Packet�� ���� �ϵ��� ���� Macro�� �����Ѵ�.
#ifdef _DEBUG
#define _CHART_TEST_REAL 1
#endif
#if _CHART_TEST_REAL
	#define _CHART_CLOCK_REAL_PACKET	"S31"
	#define _CHART_CLOCK_REAL_FIELD		"time"
	#define _CHART_CLOCK_REAL_KEYCODE	"000660"
#else
	#define _CHART_CLOCK_REAL_PACKET	"JS0"
	#define _CHART_CLOCK_REAL_FIELD		"time"
	#define _CHART_CLOCK_REAL_KEYCODE	"0000"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrdsRealReceiverAddInImp::CDrdsRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CRealReceiverAddInImp( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.11.17, ��¿�) LoadDrdsDll()�� �̰������� 1ȸ�� �Ҹ��Ƿ� ������ Routine���� �������� �ʴ´�.
	m_DrdsDLL.SetLibName("Drds.dll");
	m_DrdsDLL.m_hLib = m_DrdsDLL.LoadLib();
	if(m_DrdsDLL.m_hLib) m_DrdsVoid = m_DrdsDLL.DrdsInit( m_pChartCWnd->GetSafeHwnd());
	else m_DrdsVoid = NULL;

	m_plRealSettingInfoList	= new CRealSettingInfoList();
	m_plDrdsAdvisedInfoList	= new CDrdsAdvisedInfoList();

	// (2004.12.01, ��¿�) OnDrdsAdvised()�� Real�� ���۵Ǹ鼭 ������� ó�����ϴ� �������� Flag���� �����Ѵ�.
	//		OnDrdsAdvse()/OnDrdsData()/OnDrdsPacketEnd()/OffDrdsAdvised()������ �̿�ȴ�.
	m_bOnNMTR = FALSE;

	// XScaleManager �߰� : �������� - ojtaso (20070118)
	m_pXScaleManager = NULL;
}

CDrdsRealReceiverAddInImp::~CDrdsRealReceiverAddInImp()
{
	if( m_DrdsDLL.m_hLib && m_DrdsVoid)
	{
		m_DrdsDLL.DrdsTerm(m_DrdsVoid, m_pChartCWnd->GetSafeHwnd());
		m_DrdsDLL.UnLoadLib();
	}

	// (2004.11.30, ��¿�) ������ Real Setting Info, DRDS Advised Info, Local Share Info�� �����Ѵ�.
	if( m_plRealSettingInfoList)
	{
		m_plRealSettingInfoList->RemoveAllRealSettingInfo();
		delete m_plRealSettingInfoList;
		m_plRealSettingInfoList = NULL;
	}
	if( m_plDrdsAdvisedInfoList)
	{
		m_plDrdsAdvisedInfoList->RemoveAllDrdsAdvisedInfo();
		delete m_plDrdsAdvisedInfoList;
		m_plDrdsAdvisedInfoList = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CDrdsRealReceiverAddInImp::m_strAddInItemName = "DRDS_REAL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CDrdsRealReceiverAddInImp)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2006/7/25 - Seung-Won, Bae) Support Real Requesting
//////////////////////////////////////////////////////////////////////

// 1. XFM�� Script���� �����Ǵ� Real ���� �Ӽ� String Data�� �����ϴ� Interface�� �����Ѵ�.
BOOL CDrdsRealReceiverAddInImp::ResetRealSettingInfo( const char *p_szRealSettingInfo)
{
	// (2004.11.29, ��¿�) XFM�� Real ������ m_strRealList�� �м��Ͽ� Real Setting Info�� �����Ѵ�.
	if( !m_plRealSettingInfoList) return FALSE;
	return m_plRealSettingInfoList->ResetRealSettingInfo( p_szRealSettingInfo);
}

// 2. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
//		Ư��, p_pstrShareName�� p_pstrSharedData�� �־����� ��쿡�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
//			p_szRealSettingItem���� Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
//		(ResetRealSettingInfo()�� AddRealItem()�� ������ �� �ֵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
// (2006/4/28 - Seung-Won, Bae) with Separator
// (2006/2/3 - Seung-Won, Bae) on Error, return FALSE
BOOL CDrdsRealReceiverAddInImp::AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator)
{
	if( !m_plRealSettingInfoList) return FALSE;
	return m_plRealSettingInfoList->AddRealSettingInfo( p_szRealSettingItem, p_pstrShareName, p_pstrSharedData, p_cSeparator);
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CDrdsRealReceiverAddInImp::AddRealSettingInfoRQ(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator)
{
	if( !m_plRealSettingInfoList) return FALSE;
	_ML_TEST_NOT_USED();
	return m_plRealSettingInfoList->AddRealSettingInfo(lpszRQ, p_szRealSettingItem, p_pstrShareName, p_pstrSharedData, p_cSeparator);
}

// 2. ������ Method(RemoveRealItem)�� ���Ͽ� �����Ǵ� 1���� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
//		Ư��, Local Share Name�� ���� �ٷ� Real Key Code�� �־���,
//			������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�ؾ� �Ѵ�.
//		����� ���̻� �ش� ���� Key�� ������ Real�� ���� ��쿡�� �������� ��ȯ�ȴ�.
//		(AddRealSettingInfo()�� Interface�� �����ϵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
// (2006/4/28 - Seung-Won, Bae) with Separator
// 1. Real Setting Info�� �߰��Ѵ�.
BOOL CDrdsRealReceiverAddInImp::RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator)
{
	if( !m_plRealSettingInfoList) return FALSE;
	return m_plRealSettingInfoList->RemoveRealSettingInfo( p_szRealSettingItem, p_pstrShareName, p_cSeparator);
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CDrdsRealReceiverAddInImp::RemoveRealSettingInfoRQ(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator)
{
	if( !m_plRealSettingInfoList) return FALSE;
	_ML_TEST_NOT_USED();
	return m_plRealSettingInfoList->RemoveRealSettingInfo(lpszRQ, p_szRealSettingItem, p_pstrShareName, p_cSeparator);
}

BOOL CDrdsRealReceiverAddInImp::SetRealAdvise( BOOL p_bAdvise)
{
	if( p_bAdvise) return OnDrdsAdvise();
	return OffDrdsAdvise();
}

// RQ�� DRDS���� : �������� - ojtaso (20070111)
BOOL CDrdsRealReceiverAddInImp::SetRealAdviseRQ(LPCTSTR lpszRQ, BOOL p_bAdvise)
{
	if( p_bAdvise) return OnDrdsAdvise(lpszRQ);
	return OffDrdsAdvise(lpszRQ);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetRealSettingInfo
 - Created at  :  2005-05-24   13:56
 - Author      :  ������
 - Return Value:  BSTR - ������ ����Ÿ
 - Description :  ��Ʈ�� ���õ� ������ ����Ÿ�� �����´�.
 -----------------------------------------------------------------------------------------*/
BOOL CDrdsRealReceiverAddInImp::GetRealAdvisedInfo( CString &p_strAdvisedInfo)
{
	p_strAdvisedInfo.Empty();

	if( !m_plDrdsAdvisedInfoList) return FALSE;

	POSITION pos = m_plDrdsAdvisedInfoList->GetHeadPosition();
	while(pos)
	{
		// Packet Name���ϱ� : ex) S31
		CDaiPacketGroup* pPacketGroup = m_plDrdsAdvisedInfoList->GetNext(pos);
		CString strRealPacketName = pPacketGroup->m_strRealPacketName;
		
		POSITION posPacketGroup = pPacketGroup->GetHeadPosition();
		while(posPacketGroup)
		{
			CDaiCodeGroup* pCodeGroup = pPacketGroup->GetNext(posPacketGroup);
			// �����ڵ� ���ϱ� : ex) 000660
			CString strRealKeyCode = pCodeGroup->m_strRealKeyCode;

			POSITION posCodeGroup = pCodeGroup->GetHeadPosition();
			while(posCodeGroup)
			{
				CDrdsAdvisedInfo* pAdviseInfo = pCodeGroup->GetNext(posCodeGroup);
				
				// ��Ŷ���ϱ� : ex) �ڷ�����
				ILPCSTR szRealName = pAdviseInfo->m_pIPacket->GetPacketNameString();
				// Field Name ���ϱ� : ex) time
				CString strRealFieldName = pAdviseInfo->m_strRealFieldName;

				CString strRealSettingInfo = CString( szRealName) + _T(".") 
										+ strRealPacketName + _T(".")
										+ strRealFieldName + _T(".")
										+ strRealKeyCode + _T(";");
				p_strAdvisedInfo += strRealSettingInfo;
			}
		}
	}
		
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.17, ��¿�, �м�) Real ���� Interface�̴�.
//////////////////////////////////////////////////////////////////////

// RQ�� DRDS���� : �������� - ojtaso (20070111)
BOOL CDrdsRealReceiverAddInImp::OnDrdsAdvise(LPCTSTR lpszRQ/* = NULL*/)
{
	if( !m_pIChartOCX->IsRuntimeMode()) return FALSE;
	if( !m_plRealSettingInfoList || !m_plDrdsAdvisedInfoList) return FALSE;

	CScaleBaseData::HORZSCALEDRAWERTYPE nType = m_pIDefaultPacketManager->GetBaseTimeDataType();
	if( nType != CScaleBaseData::HORZ_TICK 
		&& nType != CScaleBaseData::HORZ_TIME_SECOND
		&& nType != CScaleBaseData::HORZ_TIME_ONE 
		&& nType != CScaleBaseData::HORZ_TIME_TWO
		&& nType != CScaleBaseData::HORZ_DAILY
		&& nType != CScaleBaseData::HORZ_WEEKLEY
		&& nType != CScaleBaseData::HORZ_MONTHLY
	) return FALSE;

	if(!lpszRQ)
		OffDrdsAdvise(lpszRQ);

	if(m_DrdsDLL.m_hLib == NULL ||m_DrdsVoid == NULL) return FALSE;

	// (2004.12.01, ��¿�) OnDrdsAdvised()�� Real�� ���۵Ǹ鼭 ������� ó�����ϴ� �������� Flag���� �����Ѵ�.
	//		OnDrdsAdvse()/OnDrdsData()/OnDrdsPacketEnd()/OffDrdsAdvised()������ �̿�ȴ�.
	m_bOnNMTR = m_pIChartOCX->GetNormalizedMinuteTimeRule() && ( nType == CScaleBaseData::HORZ_TIME_ONE || nType == CScaleBaseData::HORZ_TIME_TWO);

	// (2004.12.01, ��¿�) ������� ���� ���� Real�� Advise�� �� �ֵ��� Real Setting Info�� Local Share Info�� �߰��Ѵ�.
	//		����! �ߺ��Ǵ� Real ��û�� DRDS Advised Info���� Filltering�ϹǷ� ������ ���� �ʴ´�.
	if( m_bOnNMTR)
	{
		CString strShareName;
		m_plRealSettingInfoList->AddRealSettingInfo( _MTEXT( C0_DATE_TIME), _CHART_CLOCK_REAL_PACKET, _CHART_CLOCK_REAL_FIELD,
			_CHART_CLOCK_REAL_KEYCODE, &strShareName);
		m_pIChartOCX->AddLocalShareInfo( strShareName, 0, _CHART_CLOCK_REAL_KEYCODE);
	}

	// Packet Type
	BOOL bFullReturn = TRUE;
	POSITION posRsiPacketGroup = m_plRealSettingInfoList->GetHeadPosition();
	while( posRsiPacketGroup) 
	{
		// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
		CRsiPacketGroup *&pRsiPacketGroup = m_plRealSettingInfoList->GetNext( posRsiPacketGroup);
		if( m_plDrdsAdvisedInfoList->AddDaiPacketGroup( pRsiPacketGroup->m_strRealPacketName))
		{
			if(	m_DrdsDLL.m_hLib && m_DrdsVoid)
				bFullReturn = bFullReturn && m_DrdsDLL.DrdsConnect( m_DrdsVoid, m_pChartCWnd->GetSafeHwnd(), (LPSTR)(LPCTSTR)pRsiPacketGroup->m_strRealPacketName);
		}
		else
		{
			bFullReturn = FALSE;
		}

		POSITION posRSI = pRsiPacketGroup->GetHeadPosition();
		while( posRSI)
		{
			// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
			CRealSettingInfo *&pRSI = pRsiPacketGroup->GetNext( posRSI);

			BOOL bRsiReturn = FALSE;

			if(!pRSI->m_strRQ.IsEmpty() && pRSI->m_strRQ != lpszRQ)
				continue;
			
			ILPCSTR szRealKeyCode = m_pIChartOCX->GetLocalSharedDataString( pRSI->m_strShareName);
			if( !szRealKeyCode.IsEmpty())
			{
				IPacket *pIPacket = m_pIPacketListManager->GetPacket( lpszRQ, pRSI->m_strChartPacketName);
				if( pIPacket)
				{
					if( m_plDrdsAdvisedInfoList->AddDrdsAdvisedInfo(lpszRQ, pIPacket, pRsiPacketGroup->m_strRealPacketName,
						pRSI->m_strRealFieldName, szRealKeyCode))
					{
						bRsiReturn = m_DrdsDLL.DrdsAdvise( 
								m_DrdsVoid, 
								m_pChartCWnd->GetSafeHwnd(), 
								(long)pIPacket,
								(LPSTR)(LPCTSTR)pRsiPacketGroup->m_strRealPacketName, 
								(LPSTR)(LPCTSTR)pRSI->m_strRealFieldName,
								(LPSTR)(LPCTSTR)szRealKeyCode);

						// (2005/11/21 - Seung-Won, Bae) Log for AddRealUpdate
						if( m_pIChartOCX->IsInLogging()) m_pIAddInToolMgr->OnAddInToolCommand( EAI_LOG_DRDS_ADVISE, 
								(long)pIPacket,
								(LPSTR)(LPCTSTR)pRsiPacketGroup->m_strRealPacketName, 
								(LPSTR)(LPCTSTR)pRSI->m_strRealFieldName,
								(LPSTR)(LPCTSTR)szRealKeyCode);

					}//end of if(m_pl...

					pIPacket->Release();
				}//end of if(pPacket)
			}//end of if(!strRealKey...
			bFullReturn = bFullReturn && bRsiReturn;
		}//end of while(...
	}//end of while(...

	if( m_bOnNMTR)
	{
		// (2004.12.01, ��¿�) ������� �����ϴ� ��쿡�� ����� ���� Packet�� ��ٸ��� �ʵ��� �̸� ���� ó���� �Ѵ�.
		m_plDrdsAdvisedInfoList->SetPacketReceivedFlag( _CHART_CLOCK_REAL_PACKET, _CHART_CLOCK_REAL_KEYCODE);
		// (2004.12.01, ��¿�) ������� ��Ȯ�� ������ ������ Ȯ���ϱ� ���Ͽ�, ���� �ڷ������� Data ������ �����Ѵ�.
		IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
		if( pIPacket)
		{
			m_nTimeDataCount = pIPacket->GetDataCount();
			pIPacket->Release();
		}
	}

	return bFullReturn;
}

// RQ�� DRDS���� : �������� - ojtaso (20070111)
BOOL CDrdsRealReceiverAddInImp::OffDrdsAdvise(LPCTSTR lpszRQ/* = NULL*/)
{
	if( !m_pIChartOCX->IsRuntimeMode()) return FALSE;
	if( m_DrdsDLL.m_hLib == NULL || m_DrdsVoid == NULL) return FALSE;
	if( !m_plDrdsAdvisedInfoList) return FALSE;

	BOOL bFullReturn = TRUE;
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	POSITION posRemove = NULL;
	POSITION posDaiPacketGroup = m_plDrdsAdvisedInfoList->GetHeadPosition();
	while( posDaiPacketGroup) 
	{
		// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
		CDaiPacketGroup *&pDaiPacketGroup = m_plDrdsAdvisedInfoList->GetNext( posDaiPacketGroup);
		if(!pDaiPacketGroup)
			continue;

		POSITION posDaiCodeGroup = pDaiPacketGroup->GetHeadPosition();
		while( posDaiCodeGroup)
		{
			CDaiCodeGroup *pDaiCodeGroup = pDaiPacketGroup->GetNext( posDaiCodeGroup);
			if(!pDaiCodeGroup)
				continue;

			POSITION posDAI = pDaiCodeGroup->GetHeadPosition();
			while( posDAI)
			{
				// RQ�� DRDS���� : �������� - ojtaso (20070111)
				posRemove = posDAI;

				// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
				CDrdsAdvisedInfo *&pDAI = pDaiCodeGroup->GetNext( posDAI);
				if(!pDAI)
					continue;

				// RQ�� DRDS ���� : �������� - ojtaso (20070111)
				if(lpszRQ)
				{
					if(::lstrcmp(pDAI->m_lpszRQ, lpszRQ))
						continue;
				}

				bFullReturn = bFullReturn && m_DrdsDLL.DrdsUnadvise( 
							m_DrdsVoid, 
							m_pChartCWnd->GetSafeHwnd(), 
							(long)pDAI->m_pIPacket,
							(LPSTR)(LPCTSTR)pDaiPacketGroup->m_strRealPacketName, 
							(LPSTR)(LPCTSTR)pDAI->m_strRealFieldName,
							(LPSTR)(LPCTSTR)pDaiCodeGroup->m_strRealKeyCode);

				// (2005/11/21 - Seung-Won, Bae) Log for ResetRealUpdate
				if( m_pIChartOCX->IsInLogging()) m_pIAddInToolMgr->OnAddInToolCommand( EAI_LOG_DRDS_UNADVISE, 
							( long)pDAI->m_pIPacket,
							(LPSTR)(LPCTSTR)pDaiPacketGroup->m_strRealPacketName, 
							(LPSTR)(LPCTSTR)pDAI->m_strRealFieldName,
							(LPSTR)(LPCTSTR)pDaiCodeGroup->m_strRealKeyCode);

				delete pDAI;
				pDAI = NULL;

				pDaiCodeGroup->RemoveAt(posRemove);
			}
		}

		bFullReturn = bFullReturn && m_DrdsDLL.DrdsClose(m_DrdsVoid, m_pChartCWnd->GetSafeHwnd(), (LPSTR)(LPCTSTR)pDaiPacketGroup->m_strRealPacketName);
	}

//	RQ�� DRDS���� : �������� - ojtaso (20070111)
//	m_plDrdsAdvisedInfoList->RemoveAllDrdsAdvisedInfo();

	// (2004.12.01, ��¿�) ������� ���� ���� Real�� Advise�� �� �ֵ��� Real Setting Info�� Local Share Info�� ��ϵ� ������ �����Ѵ�.
	//		����! �ߺ��Ǵ� Real ��û�� DRDS Advised Info���� Filltering�ϹǷ� ������ ���� �ʴ´�.
	if( m_bOnNMTR)
	{
		CString strShareName;
		m_plRealSettingInfoList->RemoveRealSettingInfo( _MTEXT( C0_DATE_TIME), _CHART_CLOCK_REAL_PACKET, _CHART_CLOCK_REAL_FIELD,
			_CHART_CLOCK_REAL_KEYCODE, &strShareName);

		m_pIChartOCX->RemoveLocalShareInfo( strShareName);

		// (2004.12.01, ��¿�) OnDrdsAdvised()�� Real�� ���۵Ǹ鼭 ������� ó�����ϴ� �������� Flag���� �����Ѵ�.
		//		OnDrdsAdvse()/OnDrdsData()/OnDrdsPacketEnd()/OffDrdsAdvised()������ �̿�ȴ�.
		m_bOnNMTR = FALSE;
	}

	return bFullReturn;
}

// (2004.12.30, ��¿�, �м�) Real�� 1�� Field Data�� �����ϴ� Routine�̴�.
//		Notify Routine of 1 Real Field
//			int			wParam	~ Size of DrdsData, Fixed Value
//			DrdsData *	lParam	~ Data Structure
BOOL CDrdsRealReceiverAddInImp::SetDrdsData(UINT wParam, LPARAM lParam)
{
	// (2006/7/26 - Seung-Won, Bae) Use DRDS Structure directly
	//	BOOL bResult = m_pPacketList->SetRealOrgData( wParam, lParam, nRemovePacketDataCount);
	BOOL bResult = FALSE;
	int nlen = (int)wParam;
	if( 0 < nlen)
	{
		DrdsData* pDrdsData = (DrdsData*)lParam;

		CString strField = pDrdsData->Field;
		CString strData = pDrdsData->Data +3;
		CString strKey = pDrdsData->Key;
		IPacket* pIPacket = ( IPacket*)pDrdsData->Id;
		ILPCSTR szPcketName = NULL;
		if( pIPacket) szPcketName = pIPacket->GetPacketNameString();

		// �������� Real�ݿ� : �������� - ojtaso (20070131)
		CString strRQ = GetDrdsAdvisedRQ(pIPacket, strField, strKey);
		if( strRQ.IsEmpty()) strRQ = "DEFAULT";

		// (2009/11/1 - Seung-Won, Bae) for CRealReceiverAddInImp
		bResult = CRealReceiverAddInImp::SetRealDataRQ( strRQ, szPcketName, atof(strData), TRUE);

		if(m_pXScaleManager && strField.CompareNoCase(_T("price")) == 0)
			m_pXScaleManager->RealDataReceived( strRQ);
	}

	return (bResult ? 1L: 0L);
	//sy end
}

// RQ��� : �������� - ojtaso (20070111)
LPCSTR CDrdsRealReceiverAddInImp::GetDrdsAdvisedRQ(LPVOID p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	POSITION posDaiPacketGroup = m_plDrdsAdvisedInfoList->GetHeadPosition();
	POSITION posDaiCodeGroup = NULL;
	POSITION posDAI = NULL;
	while( posDaiPacketGroup) 
	{
		// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
		CDaiPacketGroup *&pDaiPacketGroup = m_plDrdsAdvisedInfoList->GetNext( posDaiPacketGroup);
		posDaiCodeGroup = pDaiPacketGroup->FindDaiCodeGroup(p_szRealKeyCode);
		if(posDaiCodeGroup)
		{
			CDaiCodeGroup* pDaiCodeGroup = pDaiPacketGroup->GetAt(posDaiCodeGroup);
			posDAI = pDaiCodeGroup->FindDrdsAdvisedInfo((IPacket*)p_pIPacket, p_szRealFieldName);

			if(posDAI)
				return pDaiCodeGroup->GetAt(posDAI)->m_lpszRQ;
		}
	}

	return NULL;
}

// (2004.12.30, ��¿�, �м�) Real�� 1�� Packet ������ ���� ����� ó���̴�.
//		Notify Routine of 1 Real Packet Process End
//			const char *wParam	~ Packet Name
//			const char *lParam	~ Key Code
BOOL CDrdsRealReceiverAddInImp::SetDrdsPacketEnd(UINT wParam, LPARAM lParam)
{
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	DrdsData* pDrdsData = (DrdsData*)lParam;
	
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	CString strField = pDrdsData->Field;
	CString strKey = pDrdsData->Key;
	IPacket* pIPacket = ( IPacket*)pDrdsData->Id;

	LPCSTR lpszRQ = GetDrdsAdvisedRQ(pIPacket, strField, strKey);

	CRealReceiverAddInImp::SetRealPacketEndRQ( lpszRQ, TRUE);
	
	return 0L;
}

// RQ�� Packet ���� : �������� - ojtaso (20070122)
void CDrdsRealReceiverAddInImp::RemoveRQPacket(LPCTSTR lpszRQ)
{
	// (2007/2/13 - Seung-Won, Bae) Not Used.
	//	POSITION posDaiPacketGroup = m_plDrdsAdvisedInfoList->GetHeadPosition();
	//	POSITION posDaiCodeGroup = NULL;
	//	POSITION posDAI = NULL;
	//	while( posDaiPacketGroup) 
	//	{
	//		// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
	//		CDaiPacketGroup *&pDaiPacketGroup = m_plDrdsAdvisedInfoList->GetNext( posDaiPacketGroup);
	//		posDaiCodeGroup = pDaiPacketGroup->GetHeadPosition();
	//		while(posDaiCodeGroup)
	//		{
	//			CDaiCodeGroup*& pDaiCodeGroup = pDaiPacketGroup->GetNext(posDaiCodeGroup);
	//			posDAI = pDaiCodeGroup->GetHeadPosition();
	//			while(posDAI)
	//			{
	//				CDrdsAdvisedInfo*& pDrdsAdvisedInfo = pDaiCodeGroup->GetNext(posDAI);
	//				if( !::lstrcmp( pDrdsAdvisedInfo->m_lpszRQ, lpszRQ)) m_pIPacketListManager->RemovePacket( lpszRQ, pDrdsAdvisedInfo->m_pIPacket);
	//			}
	//		}
	//	}
}
