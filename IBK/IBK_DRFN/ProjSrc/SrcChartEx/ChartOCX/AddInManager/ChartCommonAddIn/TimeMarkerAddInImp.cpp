// TimeMarkerAddInImp.cpp: implementation of the CTimeMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "TimeMarkerAddInImp.h"

#include <Mmsystem.h>									// for timeGetTime

#include "../../include/SharedBase.h"					// for DE_SHARDSET
#include "../../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTEXT()
#include "../Include_AddIn/AddInCommand.h"				// for EAI_CUSTOM_CLEAR_TIMEMARK
#include "../Include_Addin/I000000/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"			// for IPacket
#include "../Include_Addin/I000000/_IChartManager.h"	// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"			// for IBlock
#include "../Include_AddIn/I000000/_IChartOCX.h"		// for IChartOCX
#include "../Include_AddIn/I000000/_IString.h"			// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeMarkerAddInImp::CTimeMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// 1. ���� Mark ó������ �Ͻð��� �����Ѵ�.
	m_lMarkTime = 0;

	// (2004.09.14, ��¿�) ��� �Ҹ��� �����Ѵ�.
	m_pnPen.CreatePen(		PS_SOLID,	3, RGB( 0, 255, 0));
	// (2004.09.30, ��¿�) �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
	//		�߽��� �������� �ʴ´�.
	m_bEnableReceivingTimeShare = TRUE;

	// (2006/11/16 - Seung-Won, Bae) Manage the Disable Flag of TimeMark
	m_bEnableFiringTimeShare = TRUE;

	// (2007/1/17 - Seung-Won, Bae) Manage Double Click Time Interval for LBtnDblClick with force.
	m_uDoubleClickTime = GetDoubleClickTime();
}

CTimeMarkerAddInImp::~CTimeMarkerAddInImp()
{
	// (2004.09.14, ��¿�) ��� �Ҹ��� �����Ѵ�.
	m_pnPen.DeleteObject();
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CTimeMarkerAddInImp::m_strAddInItemName = "TIME_MARKER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CTimeMarkerAddInImp)
	ONADDINEVENT( OnGSharedDataChange)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonDblClk)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.11.22, ��¿�, ����) �ð������� ���޹޾�, �ð�����ǥ�ø� ����ϴ� ����� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.06.10, ��¿�) �ð� ������ �޾Ƽ� TimeMark�� �����Ѵ�.	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
void CTimeMarkerAddInImp::SetTimeMarkString( const char *p_szMarkTime, BOOL p_bWithScroll)
{
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	if( m_pIChartManager->IsEmpty()) return;

	// (2004.11.19, ��¿�) �̹� ������ �ð������� ���� �ð��� ��� �����ϴ� ������� ó���Ѵ�.
	//		Ư��, Scroll�� ������� �ʵ��� ó���Ѵ�.
	//		����, ���� TimeMark ���� Ʋ���ٸ� ���� Drawing�� �ϵ��� �Ѵ�.
	//		�̷� ����� ���Ͽ� ���� TimeMark Value�� BackUp�� ���´�.

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long lPreMarkTime = m_lMarkTime;
	__int64 lPreMarkTime = m_lMarkTime;
	// 2011.01.06 by SYS <<

	// 1.1.1 �������� ������ ��츦 ����Ͽ� TimeMarkerAddInImp�� Clear��Ų��.
	//	(�����ؾ� �� �������� ���� �ʱ�ȭ���� �ʵ��� �Ѵ�.)
	ClearTimeMark();

	// 5.3 TimeMark�� �����Ѵ�.	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
	int nTimeIndex = 0;
	m_lMarkTime = GetTimeValue( p_szMarkTime, nTimeIndex);

	if( m_lMarkTime)
	{
		// 6. Mark��ġ�� Scroll ó���Ѵ�.
		// (2004.07.05, ��¿�) Time Mark�� ���� ȭ�鿡 ���� �� �ִ� �����̸� Scroll ���� �ʴ´�.
		// 6.0.1 �� Data View�� Start/End Index�� ���Ѵ�.
		int nStartIndex, nEndIndex;
		if( m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex))
			// 6.0.2 ������ View ������ ��� Scroll�� ó������ �ʴ´�.
			if( nTimeIndex < nStartIndex || nEndIndex < nTimeIndex)
				// 6.1 Scrolling�� ó���Ѵ�.
				// (2004.11.22, ��¿�) Start Index���� Scroll�� ������ Interface�� ���Ͽ� ó����� �Ѵ�.
				if( p_bWithScroll) m_pIChartOCX->ScrollTo( nTimeIndex - ( nEndIndex - nStartIndex + 1) / 2, FALSE);
	}

	// (2004.09.14, ��¿�) Time Mark�� ������ ���, Repaint �ǵ��� �����Ѵ�.
	//		����! �� �Լ� ���� ȣ������ �ʴ� ������? (�߰��� Return�� �����ϱ�...)
	// (2004.11.19, ��¿�) ���� TimeMark ���� Ʋ���ٸ� ���� Drawing�� �ϵ��� �Ѵ�.
	if( lPreMarkTime != m_lMarkTime) m_pIChartOCX->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.22, ��¿�, ����) �ð������� �߻���Ű�� Interface�� Clear ��Ű�� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.06.11, ��¿�) Block�� Double Click���� �ð� ������ �߻���Ű�鼭 TimeMarkerAddInImp�� �����ϴ� Interface�� �����Ѵ�.
void CTimeMarkerAddInImp::SetTimeMarkPoint( CPoint p_ptPoint)
{
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	if( m_pIChartManager->IsEmpty()) return;
	// 4.1 �ð������� �߻������� �Ǿ� �ִ��� Ȯ���Ѵ�.
	if( !m_pIChartOCX->CheckGSharedSetting( _MTEXT( C6_TIME_CHART), FALSE, FALSE, TRUE)) return;

	// (2004.11.22, ��¿�) �ð� ���� ���� �����ϴ� Routine�� �����Ͽ�, ������ �ð� �������� Ȯ���ϴ� ����� �����Ѵ�.
	CString strMarkTime;
	if( !GetMarkTimeInString( p_ptPoint, strMarkTime)) return;

	// 5. ��ü Chart���� TimeMark�� ǥ���ϵ��� ó���Ѵ�.
	// (2004.09.14, ��¿�) ���������� OCX�� Invalidate ��Ű���� �Ѵ�.
	SetTimeMarkString( strMarkTime, FALSE);

	// (2006/3/4 - Seung-Won, Bae) Support "NN" Format
	if( strMarkTime.GetLength() == 16) strMarkTime = strMarkTime.Left( 14);

	// 4. ������ �ð� ������ ������ �߻���Ų��.
	// (2004.11.19, ��¿�) �̹� ������ �ð������� ���� �ð��� ��� �����ϴ� ������� ó���Ѵ�.
	//		Ư��, ������(�Ǵ� ������ ������) ��쿡�� ������ �߻���Ű�� �ʴ´�.
	if( m_lMarkTime) m_pIChartOCX->FireGSharedData( _MTEXT( C6_TIME_CHART), strMarkTime);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.22, ��¿�) Mouse Click�� ���� �ð����� ��� ���� ����� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.11.22, ��¿�) �ð� ���� ���� �����ϴ� Routine�� �����Ͽ�, ������ �ð� �������� Ȯ���ϴ� ����� �����Ѵ�.
BOOL CTimeMarkerAddInImp::GetMarkTimeInString( CPoint p_ptPoint, CString &p_strMarkTime)
{
	// 1. ���� Block���κ��� ���õ� Data Index�� ���Ѵ�.
	// 1.1 �־��� Point�� �ش��ϴ� Block�� Graph Region�� ���Ѵ�.
	// (2004.06.14, ��¿�) Block ������ Height��, Graph�� Width ������ Input �������� Ȯ���Ѵ�.
	// (2006/6/16 - Seung-Won, Bae) Use Block Interface
	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( p_ptPoint, nRow, nColumn)) return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
	if( !pIBlock) return FALSE;
	CRect rctBlockRegion, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctBlockRegion, rctNoFrameAndMargin);
	CRect rctGraphRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;
	rctBlockRegion.left = rctGraphRegion.left;
	rctBlockRegion.right = rctGraphRegion.right;
	if( !rctBlockRegion.PtInRect( p_ptPoint)) return FALSE;
	// 1.2 ���� ȭ�鿡 ���̴� Data�� Start/End Index�� ���Ѵ�.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return FALSE;
	// 1.3 ������ Mouse Point�� Data Index�� ���Ѵ�.
	//{{ 2007.03.26 by LYH XScale ����
	//int nDataIndex = nStartIndex + ( ( p_ptPoint.x - rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / rctBlockRegion.Width();
	int nDataIndex = m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", p_ptPoint.x);
	//}}
	// 2. �ش� Data Index�� �ð� ������ ���Ѵ�.
	// 2.1 �ڷ����� Packet�� ���Ѵ�.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return FALSE;
	try
	{
		// 2.2 Data�� ���Ѵ�.
		// (2004.07.02, ��¿�) �û翵��� �浹���� Logic�� �����Ѵ�.
		// (2004.09.24, ��¿�) ScrollBar�� Zero Base�� ����Ǿ� 1�� ������ �ʴ´�.
		double dMarkTime;
		if( !pIPacket->GetData( nDataIndex, dMarkTime)) throw this;
		long lMarkTime = ( long)dMarkTime;

		// 3. �ð� ������ "YYYYMMDDHHMMSS"�� Full Data�� �����.
		// 3.1 ���� Packet Type�� ���Ѵ�.
		ILPCSTR szPacketType = pIPacket->GetType();
		CString strPacketType( szPacketType);

		// 3.2 Start Index�� ���Ѵ�.
		int nStart;
		// 3.2.1 YYYY�� �����ϴ� Type�� ��츦 ó���Ѵ�.
		if( strPacketType.Left( 4) == "YYYY")	nStart = 0;
		// 3.2.2 MMDD�� �����ϴ� Type�� ��츦 ó���Ѵ�. (MM�� �ִ� ��� ���� MM�� ȥ���Ǿ� �������� �ʴ´�.)
		else if( strPacketType.Left( 4) == "MMDD")	nStart = 4;
		// (2005/9/9 - Seung-Won, Bae) Process the case of Starting with 'DD'
		else if( strPacketType.Left( 2) == "DD")	nStart = 6;
		// 3.2.3 HH�� �����ϴ� Type�� ��츦 ó���Ѵ�.
		else if( strPacketType.Left( 2) == "HH")	nStart = 8;
		// 3.2.4 MMSS�� �����ϴ� Type�� ��츦 ó���Ѵ�.
		else if( strPacketType.Left( 2) == "MM")	nStart = 10;
		else throw this;
		// 3.3 Packet Type�� ���Ͽ�, Mark Time�� �����Ѵ�.
		// 3.3.0 ���� ���� �Ͻ��� ���Ѵ�.
		SYSTEMTIME stTime;
		GetSystemTime( &stTime);
		// 3.3.1 �ð��� ������ �ʿ䰡 �ִ��� Ȯ���Ѵ�.
		// (2006/3/4 - Seung-Won, Bae) Support "NN" Format
		char *szMarkTime = p_strMarkTime.GetBufferSetLength( 16);
		strcpy( szMarkTime, "                ");
		int nEnd = nStart + strPacketType.GetLength();
		if( 8 < nEnd)
		{
			// (2006/3/4 - Seung-Won, Bae) Support "NN" Format
			int nNSecond;
			if( nEnd == 16)
			{
				nNSecond = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nNSecond = stTime.wMilliseconds / 10;
			szMarkTime[ 14] = '0' + nNSecond / 10;
			szMarkTime[ 15] = '0' + nNSecond % 10;

			// 3.3.2 ���� �ʸ� ����Ѵ�.
			int nSecond;
			if( nStart <= 12 && 14 <= nEnd)
			{
				nSecond = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nSecond = stTime.wSecond;
			szMarkTime[ 12] = '0' + nSecond / 10;
			szMarkTime[ 13] = '0' + nSecond % 10;
			// 3.3.3 ���� �����Ѵ�.
			int nMinute;
			if( nStart <= 10 && 12 <= nEnd)
			{
				nMinute = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nMinute = stTime.wMinute;
			szMarkTime[ 10] = '0' + nMinute / 10;
			szMarkTime[ 11] = '0' + nMinute % 10;
			// 3.3.4 �ð��� �����Ѵ�.
			int nHour;
			if( nStart <= 8 && 10 <= nEnd)
			{
				nHour = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nHour = stTime.wHour;
			szMarkTime[ 8] = '0' + nHour / 10;
			szMarkTime[ 9] = '0' + nHour % 10;
		}
		// 3.3.5 ���� ������ �ʿ����� Ȯ���Ѵ�.
		if( nStart < 8)
		{
			// 3.3.6 �ϸ� ����Ѵ�.
			int nDay;
			if( nStart <= 6 && 8 <= nEnd)
			{
				nDay = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nDay = stTime.wDay;
			szMarkTime[ 6] = '0' + nDay / 10;
			szMarkTime[ 7] = '0' + nDay % 10;
			// 3.3.7 ���� ����Ѵ�.
			int nMonth;
			if( nStart <= 4 && 6 <= nEnd)
			{
				nMonth = lMarkTime % 100;
				lMarkTime /= 100;
			}
			else nMonth = stTime.wMonth;
			szMarkTime[ 4] = '0' + nMonth / 10;
			szMarkTime[ 5] = '0' + nMonth % 10;
			// 3.3.8 ���� ����Ѵ�.
			int nYear;
			if( nStart == 0) nYear = lMarkTime;
			else nYear = stTime.wYear;
			szMarkTime[ 3] = '0' + nYear % 10;
			szMarkTime[ 2] = '0' + ( ( nYear / 10) % 10);
			szMarkTime[ 1] = '0' + ( ( nYear / 100) % 10);
			szMarkTime[ 0] = '0' + nYear / 1000;
		}
	}
	catch( CTimeMarkerAddInImp *)
	{
		pIPacket->Release();
		return FALSE;
	}
	pIPacket->Release();
	return TRUE;
}

// (2004.11.22, ��¿�) �ð� ���� ���� ����Ʈ�� �ð��࿡ ���� Data�� ��ȯ�ϴ� Interface�� �����Ѵ�.
//		������ 0�� Return�Ѵ�.	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS  or YYYYMMDDHHMMSSNN
// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰� <<
//long CTimeMarkerAddInImp::GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex)
__int64 CTimeMarkerAddInImp::GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex)
{
	// 0. Return Value�� �ʱ�ȭ�Ѵ�.
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long lMarkTime = 0;
	__int64 lMarkTime = 0;
	// 2011.01.06 by SYS <<

	// 1. ���� ��Ȯ�� Data������ Ȯ���Ѵ�.
	// 1.1 Data Length�� 6/8/14 �߿� �� �������� Ȯ���Ѵ�.
	CString strMarkTime( p_szMarkTime);

	// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (16 Byte)
	BOOL bNNData = ( strMarkTime.GetLength() == 16 && strMarkTime.Right( 2) != "  ");

	strMarkTime.TrimLeft();
	strMarkTime.TrimRight();
	int nLength = strMarkTime.GetLength();
	// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (16 Byte)
	if( nLength != 6 && nLength != 8 && nLength != 14 && nLength != 16) return 0;

	// 1.2 ���� �ڷ��Ͻ� ������ ������ ��츦 �����Ѵ�.
	// 1.2.1 ���� Chart�� �ڷ����� Data Format�� ���Ѵ�.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return 0;
	try
	{
		ILPCSTR szPacketType = pIPacket->GetType();
		CString strPacketType( szPacketType);

		// 1.2.2 ���� Data Start Index�� ���Ѵ�.
		int nStart;
		// 1.2.2.1 YYYY�� �����ϴ� Type�� ��츦 ó���Ѵ�.
		if( strPacketType.Left( 4) == "YYYY")	nStart = 0;
		// 1.2.2.2 MMDD�� �����ϴ� Type�� ��츦 ó���Ѵ�. (MM�� �ִ� ��� ���� MM�� ȥ���Ǿ� �������� �ʴ´�.)
		else if( strPacketType.Left( 4) == "MMDD")	nStart = 4;
		// (2005/9/9 - Seung-Won, Bae) Process the case of Starting with 'DD'
		else if( strPacketType.Left( 2) == "DD")	nStart = 6;
		// 1.2.2.3 HH�� �����ϴ� Type�� ��츦 ó���Ѵ�.
		else if( strPacketType.Left( 2) == "HH")	nStart = 8;
		// 1.2.2.4 MMSS�� �����ϴ� Type�� ��츦 ó���Ѵ�.
		else if( strPacketType.Left( 2) == "MM")	nStart = 10;
		else throw this;
		// 1.2.3 ���� �ڷ��Ͻÿ� ���Ͽ� ������ ��츦 �����Ѵ�.
		// 1.2.3.1 ������ �����̸鼭, ��Ʈ�� �ð��� ��츦 �����Ѵ�.
		// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (Can be 8 Byte)
		if( ( nLength == 8 && !bNNData) && 8 <= nStart) throw this;
		// 1.2.3.2 ������ �ð��̸鼭, ��Ʈ�� ������ ��츦 �����Ѵ�.
		if( nLength == 6 && nStart + strPacketType.GetLength() <= 8) throw this;

		// 2. ���ŵ� Data�� Type�� ����, ǥ���� Data Format���� �����Ѵ�.
		// (2006/3/4 - Seung-Won, Bae) Support "NN" Format - Nomalize to YYYYMMDDHHmmSSNN
		if( nLength != 16)
		{
			// 2.1 ���� �ð��� ���ڸ� ���Ѵ�.
			SYSTEMTIME st;
			GetLocalTime( &st);

			// 2.2 �ð� ���� Data�� �����Ѵ�.
			CString strOldValue( strMarkTime);
			// (2006/3/4 - Seung-Won, Bae) Support "NN" Format (Can be 8 Byte)
			switch( nLength)
			{
				case 6:		strMarkTime.Format( "%04d%02d%02d%s%02d", st.wYear, st.wMonth, st.wDay, strOldValue, st.wMilliseconds);
							break;
				case 8:		if( bNNData)	strMarkTime.Format( "%04d%02d%02d%s", st.wYear, st.wMonth, st.wDay, strOldValue);
							else			strMarkTime.Format( "%s%02d%02d%02d%02d", strOldValue, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds / 10);
							break;
				case 14:	strMarkTime.Format( "%s%02d", strOldValue, st.wMilliseconds);
							break;
			}
		}

		// 4. Packet Data Type�� ����, GShare Data�� �����Ѵ�.
		// 4.2 Packet Type�� ���Ͽ�, GShare Data�� �����Ѵ�.
		strMarkTime = strMarkTime.Mid( nStart, strPacketType.GetLength());

		// 5. TimeMarkerAddInImp Object�� Marking �Ͻø� �����Ѵ�.
		// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
		//lMarkTime = atol( strMarkTime);
		lMarkTime = _atoi64( strMarkTime);
		// 2011.01.06 by SYS <<


		// (2004.07.02, ��¿�) ���������� �����鼭, ������ Data������ �ð������� ���,
		//		�̸� �������鿡���� ��� Scale�� ������ ������� ���ϹǷ� �����Ѵ�.
		// 5.1 Mark Time�� Data Index�� ���Ѵ�.
		// 5.1.2 Time Mark�� ��ġ�� Ȯ���Ѵ�.
		// (2004.06.18, ��¿�) ��Ȯ�� Data�� Index�� �������� ���� ��� �� ���� Data Index�� ���Ѵ�.
		double dSelectedDate;
		if( !pIPacket->GetCurOrPrevIndexFromData( lMarkTime, p_nTimeIndex, dSelectedDate)) throw this;
		// 5.2 TimeMark�� ������ Data������ Index�� ������ Ȯ���Ѵ�.
		// 5.2.1 TimeMark�� ������ ���� ����Ͽ��� �ϴ� ������� Ȯ���Ѵ�.
		int nLastDataIndex = m_pIChartOCX->GetLastRealDataIndex();
		if( nLastDataIndex == p_nTimeIndex)
		{
			// 5.2.2 �ٻ� Index���� Ȯ���Ѵ�.
			if( lMarkTime != ( long)dSelectedDate)
			{
				// 5.2.3 ���� ������ �ִ� ��쿡�� Skip�Ѵ�.
				int nScrollRange;
				if( !m_pIChartOCX->GetMaxScrollRange( nScrollRange)) throw this;
				if( nLastDataIndex < nScrollRange) throw this;
			}
		}

		// (2004.07.05, ��¿�) Time Mark�� ���� �ٸ� ���񿡼��� ��ȿ�ϵ���, 
		//		Packet�� Type�� �����Ͽ�, ����� ��찡 �ƴϸ� ��� Draw�ϵ��� �Ѵ�.
		m_strPacketType = strPacketType;
	}
	catch( CTimeMarkerAddInImp *)
	{
		pIPacket->Release();
		return 0;
	}
	pIPacket->Release();

	return lMarkTime;
}

// (2004.11.22, ��¿�) Mouse ��ġ�� �ð�����ǥ�ð� ������ ���, �̸� �ּҽ�Ų��.
BOOL CTimeMarkerAddInImp::ClearTimeMark( CPoint p_ptPoint)
{
	// 0. ���� ������ �ð� ���� ǥ�ð� �ִ��� Ȯ���Ѵ�.
	if( !m_lMarkTime) return FALSE;

	// 1. ���� Mouse ��ġ�� �ð��������� �����Ѵ�.
	// 1.1 �ð� ���� String�� ���Ѵ�.
	CString strMarkTime;
	if( !GetMarkTimeInString( p_ptPoint, strMarkTime)) return FALSE;
	// 1.2 �ð� ���� ���� ���Ѵ�.
	int nTimeIndex = 0;
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long lPointTime = GetTimeValue( strMarkTime, nTimeIndex);
	__int64 lPointTime = GetTimeValue( strMarkTime, nTimeIndex);
	// 2011.01.06 by SYS <<

	if( !lPointTime) return FALSE;

	// 2. ������ �ð����� ���� �̹� ������ �ð��������� ������,
	//		Clear��Ű�� �����Ѵ�.
	// 2.1 �ð����� ��ġ���� Ȯ���Ѵ�.
	// 2.1.1 �̶� ������ �ð����� �ð��� �����ð��� �� �����Ƿ� (�ش�ð��� ���� ���)
	//		�ش� ��ġ�� ��Ȯ�� �ð��� ���Ͽ� Ȯ���Ѵ�.
	long lMarkTime = 0;
	IPacket *pIPacket = NULL;
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( pIPacket)
	{
		double dSelectedDate;
		if( !pIPacket->GetCurOrPrevIndexFromData( m_lMarkTime, nTimeIndex, dSelectedDate)) dSelectedDate = 0;
		pIPacket->Release();
		lMarkTime = ( long)dSelectedDate;
	}
	if( !lMarkTime) return FALSE;
	// 2.1.2 ������ �ð����� �ð��� ������ Ȯ���Ѵ�.
	if( lPointTime != lMarkTime) return FALSE;
	// 2.2 ������ �ð��������� Clear ��Ų��.
	ClearTimeMark();
	m_pIChartOCX->InvalidateControl();
	return TRUE;
}

// (2004.09.30, ��¿�) �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
//		�߽��� �������� �ʴ´�.
void CTimeMarkerAddInImp::OnEnableReceivingTimeShare (BOOL p_bEnable)
{
	m_bEnableReceivingTimeShare = p_bEnable;
	// (2004.09.30, ��¿�) �ð������� ������ ���ѵǴ� ���������� ������ �ð��������� Clear��Ų��.
	if( !m_bEnableReceivingTimeShare)
	{
		ClearTimeMark();
		// (2004.12.13, ��¿�) NULL Pointer�� Ȯ���Ѵ�.
		m_pIChartOCX->InvalidateControl();
	}
}

// (2004.10.11, ��¿�) TimeMark�� �����ϴ� Interface�� �����Ѵ�.
void CTimeMarkerAddInImp::OnSetTimeMark( const char *p_szMarkTime, BOOL p_bWithScroll)
{
	// 1. Null String�� ��쿡�� TimeMark�� Clear��Ų��.
	if( !p_szMarkTime || !p_szMarkTime[ 0])
	{
		ClearTimeMark();
		return;
	}

	// 2. TimeMark String�� �����Ͽ� ������ �õ��Ѵ�. (���ο��� ��ȿ���� �˻��Ѵ�.)
	// (2004.09.14, ��¿�) ���������� OCX�� Invalidate ��Ű���� �Ѵ�.
	SetTimeMarkString( p_szMarkTime, p_bWithScroll);
}

//////////////////////////////////////////////////////////////////////
// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
BOOL CTimeMarkerAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	switch( p_nCommandType)
	{
		case 0:	m_bEnableFiringTimeShare = ( p_lData != 0);
				break;
		case 1:	m_bEnableReceivingTimeShare = ( p_lData != 0);
				break;
		case 2:	m_bEnableFiringTimeShare = ( p_lData != 0);
				m_bEnableReceivingTimeShare = ( p_lData != 0);
				break;
	}

	if( 0 < p_nCommandType && !m_bEnableReceivingTimeShare)
	{
		ClearTimeMark();
		// (2004.12.13, ��¿�) NULL Pointer�� Ȯ���Ѵ�.
		m_pIChartOCX->InvalidateControl();
	}

	// (2006/11/24 - Seung-Won, Bae) Notify Tool State.
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ENABLE_TIMEMARK, m_bEnableFiringTimeShare && m_bEnableReceivingTimeShare);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
//////////////////////////////////////////////////////////////////////

// [04/10/11] Global ������ �����ϴ� Interface�� �����Ѵ�.
// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
void CTimeMarkerAddInImp::OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
{
	// (2004.09.30, ��¿�) �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
	//		�߽��� �������� �ʴ´�.
	if( !m_bEnableReceivingTimeShare) return;

	// 0. �׸����� �ð� ������ ���� ó������ �����Ѵ�.
	if( strnicmp( p_szGShareKey, _MTEXT( C6_TIME_GRID), 11)) return;
	if( p_szGShareKey[ 11] != '\0' &&  !( p_szGShareKey[ 11] == ';' && p_szGShareKey[ 12] == '\0')) return;

	// 1. TimeMarker���� �ð� ������ ������Ų��.
	// (2004.09.14, ��¿�) ���������� OCX�� Invalidate ��Ű���� �Ѵ�.
	SetTimeMarkString( p_szGShareValue);
}

// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
void CTimeMarkerAddInImp::OnResetChart( void)
{
	// (2004.07.26, ��¿�) �ð� ������ TimeMark�� Clear��Ų��.
	ClearTimeMark();
}

// (2004.06.10, ��¿�) Drawing ������ �������� ����Ѵ�.
void CTimeMarkerAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	// (2004.09.15, ��¿�) Ư����Ʈ�� ��쿡�� ������ �ʵ��� �Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 0. Drawing�� ������ ��Ȳ���� Ȯ���Ѵ�.
	if( !m_lMarkTime) return;

	// (2004.07.05, ��¿�) Time Mark�� ���� �ٸ� ���񿡼��� ��ȿ�ϵ���, 
	//		Packet�� Type�� �����Ͽ�, ����� ��찡 �ƴϸ� ��� Draw�ϵ��� �Ѵ�.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return;
	try
	{
		ILPCSTR szPacketType = pIPacket->GetType();
		if( m_strPacketType != szPacketType) throw this;

		// 1. Mark Time�� Data Index�� ���Ѵ�.
		// 1.1 �� Data View�� Start/End Index�� ���Ѵ�.
		int nStartIndex, nEndIndex;
		if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) throw this;	
		// 1.2 Time Mark�� ��ġ�� Ȯ���Ѵ�.
		// (2004.06.18, ��¿�) ��Ȯ�� Data�� Index�� �������� ���� ��� �� ���� Data Index�� ���Ѵ�.
		int nTimeIndex;
		double dSelectedDate;
		if( !pIPacket->GetCurOrPrevIndexFromData( m_lMarkTime, nTimeIndex, dSelectedDate)) throw this;
		// (2004.09.24, ��¿�) ScrollBar�� ZeroBase���� 1�� ������Ű�� �ʴ´�.
		// nTimeIndex++;

		// (2004.09.14, ��¿�) ��� �Ҹ��� �����Ѵ�.
		CPen *pOldPen = pDC->SelectObject( &m_pnPen);

		// 2. �ش� Data Index�� View Point�� ���Ѵ�.
		// 2.1 Block Count�� Ȯ���Ѵ�.
		int nBlockRow		= m_pIChartManager->GetRowCount();
		int nBlockColumn	= m_pIChartManager->GetColumnCount();
		// 2.2 �� Block���� TimeMark�� �׸���.
		int			nX, nDataOnPage = nEndIndex - nStartIndex + 1;
		int			nLeftBorder, nRightBorder, nTimeOffset = nTimeIndex - nStartIndex;
		CRect		rctBlockRegion, rctNoFrameAndMargin, rctGraphRegion;
		double		dCandleWidth;
		for( int nR = 0; nR < nBlockRow; nR++) for( int nC = 0; nC < nBlockColumn; nC++)
		{
			// 2.2.1 Block�� Region�� ���Ѵ�.
			// (2006/6/16 - Seung-Won, Bae) Use Block Interface
			IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
			if( !pIBlock) continue;
			pIBlock->GetGraphRegions( rctBlockRegion, rctNoFrameAndMargin);
			rctGraphRegion = pIBlock->GetGraphDrwingRegion();
			pIBlock->Release();
			pIBlock = NULL;

			// 2.2.2 TimeMark�� X Position�� ���Ѵ�. (Graph�� X Position�� Graph Region�� �������� �����Ѵ�.)
			// (2004.09.14, ��¿�) Graph�� ��Ȯ�� �߽��� ���Ͽ� Drawing�ϵ��� �Ѵ�.
			//		��Ȯ�� �߽��� ���ϱ� ���ؼ��� ���� ������輱�� ������輱�� �߽�(����)�̴�.
			//		���� �ش� ���� �°��� ������ ��ġ�� �����ϰ�, �̸� ��ճ���.
			// 2.2.2.1 ���� ���� ���Ѵ�.
			dCandleWidth = double( rctGraphRegion.Width()) / double( nDataOnPage);
			// 2.2.2.2 ���� �°�踦 ���Ѵ�. (�� ���� �������� 1 Pixel(����) �����̴�.)
			nLeftBorder = rctGraphRegion.left + int( dCandleWidth * double( nTimeOffset) + 0.5) + 1;
			// 2.2.2.3 ���� ���踦 ���Ѵ�. (���� �� ���� �°�躸�� 1 Pixel(����) �����̴�.)
			nRightBorder = rctGraphRegion.left + int( dCandleWidth * double( nTimeOffset + 1) + 0.5) - 1;
			// 2.2.2.4 ���� �߽��� ��´�. (�������� ����Ѵ�.)
			//{{ 2007.03.26 by LYH XScale ����
			//nX = ( nLeftBorder + nRightBorder) / 2;
			nX = m_pIChartManager->GetXPosition("DEFAULT", nTimeIndex, 0);
			//}}
			// (2004.08.25, ��¿�) Mark Line�� Graph ���� ������ ������ ��츦 Ȯ���Ѵ�.
			if( nX <= rctGraphRegion.left || rctGraphRegion.right <= nX) continue;
			// 2.2.3 Line�� �׸���. (�ð� ���� Line�� Block Region�� �׸���. Ư�� Block ������ �׷��� �ణ �ð� �������� ����ǵ��� �Ѵ�.)
			// (2004.09.14, ��¿�) Pen�� ���Ⱑ 3 Pixel�� �Ǹ鼭, ��輱���� �׸��� �ʾƵ� ��輱���� ������ ��ģ��.
			
			// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
			//if( m_lMarkTime == ( long)dSelectedDate)
			if( m_lMarkTime == ( __int64)dSelectedDate)
			// 2011.01.06 by SYS <<
			{
				pDC->MoveTo( nX, rctBlockRegion.top);
				pDC->LineTo( nX, rctBlockRegion.bottom);
			}
			else
			{
				const int nUnitLineLen = 10;
				const int nGapLen = 5;
				for( int nL = int( rctBlockRegion.top / nUnitLineLen) * nUnitLineLen; nL < rctBlockRegion.bottom; nL += nUnitLineLen)
				{
					pDC->MoveTo( nX, max( rctBlockRegion.top, nL));
					pDC->LineTo( nX, min( rctBlockRegion.bottom, nL + nUnitLineLen - nGapLen));
				}
			}
		}

		// (2004.09.14, ��¿�) ��� �Ҹ��� �����Ѵ�.
		pDC->SelectObject( pOldPen);
	}
	catch( CTimeMarkerAddInImp *)
	{
		pIPacket->Release();
		return;
	}
	pIPacket->Release();
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CTimeMarkerAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( p_nToolType != CToolOptionInfo::T_ENABLE_TIMEMARK) return;
	InvokeAddIn( 2, p_nCmdOption);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CTimeMarkerAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( EAI_CUSTOM_CLEAR_TIMEMARK != p_eCommandID) return FALSE;
	ClearTimeMark();
	return TRUE;
}

BOOL CTimeMarkerAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( EAI_CUSTOM_SET_TIMEMARK != p_eCommandID) return FALSE;
	OnSetTimeMark( p_szParam, p_lParam);
	return TRUE;
}

BOOL CTimeMarkerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_CUSTOM_ENABLE_RECEIVING_TIMESHARE != p_eCommandID) return FALSE;
	OnEnableReceivingTimeShare( p_lParam);
	return TRUE;
}

// (2006/2/18 - Seung-Won, Bae) Do not support deleting with Analysis Tool.
//		Use EAI_CUSTOM_CLEAR_TIMEMARK with OCX Method for Item Changing
/*
BOOL CTimeMarkerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	// (2004.07.27, ��¿�) �м����� ��� ����⿡�� TimeMark�� ���쵵�� �Ѵ�.
	// (2004.08.12, ��¿�) �м������� ��� ����� ���� ������ ����Ǵ� ��쿡�� �ڵ�ó���Ǿ�� �Ѵ�.
	//		�׷��� �� ���������� TimeMark�� �������� �ʵ��� �м����� Message���� ���� �����ϵ��� �Ѵ�.
	if( EAI_OOA_ON_TOOL_FUNCTION == p_eCommandID && CToolOptionInfo::T_DELETE_ALL == ( CToolOptionInfo::TOOLOPTION)p_lParam1)
		ClearTimeMark();

	return FALSE;	// CToolOptionInfo::T_DELETE_ALL is for All
}
*/

// [04/10/25] Left Mouse Button Down Event�� ó���ϴ� Interface�� �����Ѵ�.
BOOL CTimeMarkerAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLButtonTime < m_uDoubleClickTime && m_ptPrevious.x - point.x < 3 && m_ptPrevious.y - point.y < 3)
		return OnLButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID);

	m_ptPrevious = point;
	m_dwLButtonTime = dwTime;
	// (2004.11.22, ��¿�) Mouse ��ġ�� �ð�����ǥ�ð� ������ ���, �̸� �ּҽ�Ų��.
	return ClearTimeMark( point);
}

// [04/10/11] Left Mouse Button Double Click Event�� ó���ϴ� Interface�� �����Ѵ�.
BOOL CTimeMarkerAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	m_dwLButtonTime = 0;

	// (2006/11/16 - Seung-Won, Bae) Manage the Disable Flag of TimeMark
	if( !m_bEnableFiringTimeShare) return FALSE;

	// (2004.06.11, ��¿�) �ð������� �߻���Ų��. (�ٴ��� Double Click�� ��쿩�� �Ѵ�.)
	SetTimeMarkPoint( point);
	return TRUE;
}
