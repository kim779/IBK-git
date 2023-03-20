// KTBChartEvent.cpp: implementation of the Event Routines
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "KTBChartCtl.h"		// for CKTBChartCtrl
#include "BaseDesk.h"			// for CBaseDesk2
#include "ToolBarManager.h"		// for ToolBar.dll
#include "Conversion.h"			// for CDataConversion
#include "PacketList.h"			// for CPacketList
#include "OutsideCommonInterface.h"			// for UWM_ZOOMVALUE

//�̺�Ʈ
#define		EVENT_DATAUPANDDOWN				    0x00000001	// (��2byte)0000 0000 0000 0001: 
#define		EVENT_REQUESTBONGDATA				0x00000002	// (��2byte)0000 0000 0000 0010: 
#define		EVENT_CHANGEONEPAGEDATACOUNT	    0x00000004	// (��2byte)0000 0000 0000 0100: 
#define		EVENT_RESTORE						0x00000008	// (��2byte)0000 0000 0000 1000: 
#define		EVENT_REQUESTMAPLOAD				0x00000010	// (��2byte)0000 0000 0001 0000: 
#define		EVENT_REQUESTNEWPACKET			    0x00000020	// (��2byte)0000 0000 0010 0000: 
#define		EVENT_SAVESETTEDCHARTINFOFROMSERVER	0x00000040	// (��2byte)0000 0000 0100 0000: 
#define		EVENT_GETSETTEDCHARTINFOFROMSERVER  0x00000080	// (��2byte)0000 0000 1000 0000: 
#define		EVENT_REQUESTPACKETDATA				0x00000100	// (��2byte)0000 0001 0000 0000: 
#define		EVENT_ADDINEVENT					0x00000200	// (��2byte)0000 0010 0000 0000: 
#define		EVENT_CHANGEDATEINDEX				0x00000400	// (��2byte)0000 0100 0000 0000: 
#define		EVENT_REQUESTCODEANDNAME			0x00000800	// (��2byte)0000 1000 0000 0000: 
#define		EVENT_SENDDATAINDEXINLBUTTONUP		0x00001000	// (��2byte)0001 0000 0000 0000: 
#define		EVENT_RETURNORGCHART				0x00002000	// (��1byte)0010 0000 0000 0000: 


void CKTBChartCtrl::NeoFireDataUpandDown(int nOption)
{
	// ����(05/01/12)
	if(!m_pToolBarManager)
		return;

	BOOL ret = SetScrollPosToNextPage( nOption == 0);
	if(ret == FALSE)
	{
		CString strStartDate = CDataConversion::IntToString(m_pPacketList->GetStartDate(), m_pPacketList->GetPacketType( _MTEXT( C0_DATE_TIME)).GetLength());
		CString strEndDate = CDataConversion::IntToString(m_pPacketList->GetEndDate(), m_pPacketList->GetPacketType( _MTEXT( C0_DATE_TIME)).GetLength());

		// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
		if( m_pBaseDesk && ( m_lFlagEvent & EVENT_DATAUPANDDOWN))
		{
			// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
			CString szFire ;
			szFire.Format( "%d,%s,%s", nOption, strStartDate, strEndDate);
			if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("DataUpandDown"), szFire))
				m_lFlagEvent &= ~EVENT_DATAUPANDDOWN;
		}
		if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_DATAUPANDDOWN))
			FireDataUpandDown( nOption, (LPSTR)(LPCTSTR)strStartDate, (LPSTR)(LPCTSTR)strEndDate);
	}
}

void CKTBChartCtrl::NeoRequestBongData(int nOption, int nTemp)
{
	m_bWaitRequest = TRUE;

	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && ( m_lFlagEvent & EVENT_REQUESTBONGDATA))
	{
			// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
			CString szFire ;
			szFire.Format( "%d,%d", nOption, nTemp);
			if( !m_pBaseDesk->InvokeEvent(m_hWnd, _T("RequestBongData"), szFire))
				m_lFlagEvent &= ~EVENT_REQUESTBONGDATA;
	}
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_REQUESTBONGDATA))
		FireRequestBongData( nOption, nTemp);
}

void CKTBChartCtrl::NeoChangeOnePageDataCount(int nOnePageDataCount)
{
	// (2004.05.18, ��¿�) DLL Container���� �� ȭ���� Data �� (ZoomValue)�� ��ȭ�� �����Ѵ�.
	if( m_hContainerDllWnd)
	{
		SendEventMsgToDLL( UWM_ZOOMVALUE, 0, nOnePageDataCount);
		// (2008/12/21 - Seung-Won, Bae) Do not notify with other way.
		return;
	}

	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && ( m_lFlagEvent & EVENT_CHANGEONEPAGEDATACOUNT))
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format( "%d", nOnePageDataCount);
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("ChangeOnePageDataCount"), szFire))
			m_lFlagEvent &= ~EVENT_CHANGEONEPAGEDATACOUNT;
	}
// (2008/12/21 - Seung-Won, Bae) Do not notify with other way.
//	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_CHANGEONEPAGEDATACOUNT))
//		FireChangeOnePageDataCount(nOnePageDataCount);
}


void CKTBChartCtrl::NeoFireSaveSettedChartInfoToServer()
{
	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && m_lFlagEvent & EVENT_SAVESETTEDCHARTINFOFROMSERVER)
	{
		if( !m_pBaseDesk->InvokeEvent( m_hWnd,_T("SaveSettedChartInfoToServer"), ""))
			m_lFlagEvent &= ~EVENT_SAVESETTEDCHARTINFOFROMSERVER;
	}
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_SAVESETTEDCHARTINFOFROMSERVER))
		FireSaveSettedChartInfoToServer();
}

void CKTBChartCtrl::NeoFireGetSettedChartInfoFromServer()
{
	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && m_lFlagEvent & EVENT_GETSETTEDCHARTINFOFROMSERVER)
	{
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("GetSettedChartInfoFromServer"), ""))
			m_lFlagEvent &= ~EVENT_GETSETTEDCHARTINFOFROMSERVER;
	}
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_GETSETTEDCHARTINFOFROMSERVER))
		FireGetSettedChartInfoFromServer();
}

void CKTBChartCtrl::NeoRestore()
{
	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && m_lFlagEvent & EVENT_RESTORE)
	{
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("Restore"), ""))
			m_lFlagEvent &= ~EVENT_RESTORE;
	}
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_RESTORE))
		FireRestore();
}

// 02.28.2002
// Map Load ��û Event
// nOption = 1 : �ż�
//           2 : �ŵ� 
//           3 : ����
//			 4 : ����
//			 5 : ������������
void CKTBChartCtrl::NeoRequestMapLoad(int nOption, CString strCode, CString strDate)
{
	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && m_lFlagEvent & EVENT_REQUESTMAPLOAD)
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format( "%d,%s,%s", nOption, strCode, strDate);
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("RequestMapLoad"), szFire))
			m_lFlagEvent &= ~EVENT_REQUESTMAPLOAD;
	} 
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_REQUESTMAPLOAD))
		FireRequestMapLoad(nOption, (LPSTR)(LPCTSTR)strCode, (LPSTR)(LPCTSTR)strDate);
}

// 02.28.2002
// ���ο� ����Ÿ ��û Event
// nOption = 0 : ���� �ְ� ������
//			 1 : ���� �ְ� �����
//			 2 : ���� / �Ǹ��� ��û
void CKTBChartCtrl::NeoRequestNewPacket(int nOption, CString strName2, CString strCode, CString strDate)
{
	CString strName = _MTOL0( strName2);
	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && m_lFlagEvent & EVENT_REQUESTNEWPACKET)
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format( "%d,%s,%s,%s", nOption, strName, strCode, strDate);
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("RequestNewPacket"), szFire))
			m_lFlagEvent &= ~EVENT_REQUESTNEWPACKET;
	} 
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_REQUESTNEWPACKET))
		FireRequestNewPacket(nOption, (LPSTR)(LPCTSTR)strName, (LPSTR)(LPCTSTR)strCode, (LPSTR)(LPCTSTR)strDate);
}

void CKTBChartCtrl::NeoRequestPacketData(const CString& strDateType, const CString& strStartDate, const CString& strPacketNames)
{
	_ML_TEST_NOT_USED();

	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	if( m_pBaseDesk && m_lFlagEvent & EVENT_REQUESTPACKETDATA)
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format( "%s,%s,%s", strDateType, strStartDate, strPacketNames);
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("RequestPacketData"), szFire))
			m_lFlagEvent &= ~EVENT_REQUESTPACKETDATA;
	} 
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_REQUESTPACKETDATA))
		FireRequestPacketData( (LPSTR)(LPCTSTR)strDateType, (LPSTR)(LPCTSTR)strStartDate, (LPSTR)(LPCTSTR)strPacketNames);
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/18
// Modifier		: 
// Comments		: AddIn Event Notify Routine
//-----------------------------------------------------------------------------
void CKTBChartCtrl::NeoAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData)
{
	// (2005/9/16 - Seung-Won, Bae) Event Flag is only used in BaseDesk.
	//		Caution.
	if( m_pBaseDesk && m_lFlagEvent & EVENT_ADDINEVENT)
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format( "%s,%d,%s,%f,%ld", p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("AddInEvent"), szFire))
			m_lFlagEvent &= ~EVENT_ADDINEVENT;
	} 
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_ADDINEVENT))
		FireAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);
}

void CKTBChartCtrl::NeoChangeDateIndex(int nDateIndex)
{
	if(m_lFlagEvent & EVENT_CHANGEDATEINDEX)
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format("%d", nDateIndex);
		if(m_pBaseDesk)
		{
			if(m_pBaseDesk->InvokeEvent(m_hWnd, _T("ChangeDateIndex"), szFire))
				FireChangeDateIndex(nDateIndex);
			else
				m_lFlagEvent &= ~EVENT_CHANGEDATEINDEX;
		} 
	}
}

void CKTBChartCtrl::NeoRequestCodeAndName()
{
	if(m_lFlagEvent & EVENT_REQUESTCODEANDNAME)
	{
		if(m_pBaseDesk)
		{
			if(m_pBaseDesk->InvokeEvent(m_hWnd, _T("RequestCodeAndName"), ""))
				FireRequestCodeAndName();
			else
				m_lFlagEvent &= ~EVENT_REQUESTCODEANDNAME;
		} 
	}
}

void CKTBChartCtrl::NeoSendDataIndexInLButtonUp(int nRowIndex, int nColumnIndex, int nDataIndex, double dValue)
{
	if(m_lFlagEvent & EVENT_SENDDATAINDEXINLBUTTONUP)
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format( "%d,%d,%d,%f", nRowIndex, nColumnIndex, nDataIndex, dValue);
		
		if(m_pBaseDesk)
		{
			if(m_pBaseDesk->InvokeEvent(m_hWnd, _T("SendDataIndexInLButtonUp"), szFire))
				FireSendDataIndexInLButtonUp(nRowIndex, nColumnIndex, nDataIndex, dValue);
			else
				m_lFlagEvent &= ~EVENT_SENDDATAINDEXINLBUTTONUP;
		} 
	}
}

//-----------------------------------------------------------------------------
//sy 2005.12.05
// ��ü�ʱ�ȭ �� ��� ȣ��.
//-----------------------------------------------------------------------------
void CKTBChartCtrl::NeoReturnOrgChart(const int nOnePageDataCount)
{
	if( m_pBaseDesk && ( m_lFlagEvent & EVENT_RETURNORGCHART))
	{
		// (2005/8/1 - Seung-Won, Bae) Modify for Hanhwa Platform
		CString szFire;
		szFire.Format( "%d", nOnePageDataCount);
		if( !m_pBaseDesk->InvokeEvent( m_hWnd, _T("ReturnOrgChart"), szFire))
			m_lFlagEvent &= ~EVENT_RETURNORGCHART;
	}
	if( !m_pBaseDesk || !( m_lFlagEvent & EVENT_RETURNORGCHART))
		FireReturnOrgChart(nOnePageDataCount);
}
