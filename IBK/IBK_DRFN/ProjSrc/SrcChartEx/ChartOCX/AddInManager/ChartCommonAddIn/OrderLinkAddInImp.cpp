// OrderLinkAddInImp.cpp: implementation of the COrderLinkAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "OrderLinkAddInImp.h"

#include <math.h>											// for log()

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/Conversion.h"					// for CDataConversion
#include "../Include_AddIn/AddInCommand.h"					// for EAI_CUSTOM_ENABLE_BOUNDMARK
#include "../Include_AddIn/I000000/IWBlock.h"				// for IChartManager
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IPacketListManager.h"	// for IPacketListManager
#include "../../Include_Chart/Conversion.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COrderLinkAddInImp::COrderLinkAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CBoundMarkerAddInImp( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.10.25, ��¿�) ȣ�������� ���� Member Variable�� �����Ѵ�.
	m_bRun						= false;
	m_nIndicatorPosY			= 0;				// Current Indicator Drawing Position			(IndicatorPosition)
	m_bOnSetCaprture			= FALSE;			// Indicator Drawing Situation Flag			(SetCaptureFlag)
	m_dIndicatedPrice			= 0.0;				// Indicated Price							(IndicatedPrice)

	// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
	//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
	//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
	m_hAPCursor = NULL;

	// (2004.10.26, ��¿�) ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
	m_dPacketUnitValue = 0;

	// (2004.11.01, ��¿�) �屸�а� ȣ������ DLL�� �����Ͽ�, ȣ�� ������ �̿��Ѵ�.
	m_nMarketType	= -1;
	m_nRatioByYCP	= 0;			// (2004.11.02, ��¿�) �����(�����)���� �������� Flag�� �޾� �����Ѵ�.
	m_dYClosePrice	= 0;
}

COrderLinkAddInImp::~COrderLinkAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
// 20080901	������	ȣ������ ��� ( Contailner�� ���õ� ��ġ�� ���� Event�� �����Ѵ� )
CString COrderLinkAddInImp::m_strAddInItemName = "ORDER_LINK";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( COrderLinkAddInImp)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnToolCommand)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2004.10.25, ��¿�) Bound���� �����ϴ� Interface�� �����Ѵ�.
//		Ȯ��� �����ڵ� ������ �޾� �����Ͽ� ��������ÿ� �̿��Ѵ�.
//////////////////////////////////////////////////////////////////////
void COrderLinkAddInImp::ResetBound( const char *p_szBound)
{
	// 0. ���� Base�� Bound Info�� ���� �����Ѵ�.
	CBoundMarkerAddInImp::ResetBound( p_szBound);

	// 0.5 ������ ��츦 ����Ͽ� �̸� Item Code�� Clear��Ų��.
	m_strItemCode.Empty();

	// 0.6 Packet�� Packet Unit Value (Precision)�� ���Ѵ�.
	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( m_strRQ);
	if( pIPacketManager)
	{
		IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
		// 0.6 Packet�� Packet Unit Value (Precision)�� ���Ѵ�.
		m_dPacketUnitValue = 0;
		if( pIPacket)
		{
			m_dPacketUnitValue = pIPacket->GetPacketTypeUnitValue();
			pIPacket->Release();
		}
		pIPacketManager->Release();
	}

	// 1. Bound Info�� ���������� ������ ��쿡, ���� ������ �����Ѵ�.
	if( m_strIndicatorName.IsEmpty()) return;

	// (2004.11.01, ��¿�) �����ڵ�, �屸�а� ���������� ������ ������ �̿��Ѵ�.
	// 2.1 Buffer�� �Ҵ�޴´�.
	CString strMsg;
	char *szBuffer = strMsg.GetBuffer( m_strExtMsg.GetLength());
	strcpy( szBuffer, m_strExtMsg);
	// 2.2 �����ڵ带 ���Ѵ�.
	char *szItemCode = szBuffer;
	szBuffer = strchr( szBuffer, ':');
	if( szBuffer) *szBuffer++ = '\0';
	m_strItemCode = szItemCode;
	// (2004.11.01, ��¿�) �����ڵ�� RTime�� ��Ų��. Server������ �⺻ 7�ڸ��� �����Ѵٰ� �Ѵ�.
	m_strItemCode.TrimRight();
	// 2.3 �屸�и� ���Ѵ�.
	m_nMarketType = -1;
	if( szBuffer)
	{
		char *szMarketType = szBuffer;
		szBuffer = strchr( szBuffer, ':');
		if( szBuffer) *szBuffer++ = '\0';
		m_nMarketType = atol( szMarketType);
	}
	// 2.4 ���������� ���Ѵ�.
	m_dYClosePrice = 0;
	if( szBuffer)
	{
		char *szYClosePrice = szBuffer;
		szBuffer = strchr( szBuffer, ':');
		if( szBuffer) *szBuffer++ = '\0';
		m_dYClosePrice = atof( szYClosePrice);
	}
	// (2004.11.02, ��¿�) �����(�����)���� �������� Flag�� �޾� �����Ѵ�.
	m_nRatioByYCP = 0;
	if( szBuffer)
	{
		char *szRatioByYCP = szBuffer;
		szBuffer = strchr( szBuffer, ':');
		if( szBuffer) *szBuffer++ = '\0';
		m_nRatioByYCP = atol( szRatioByYCP);
	}

	// 3. ��/���� ǥ���� �ش� Graph�� �̿��ϴ� Packet�� �ϳ� ã��,
	//		�ش� ���е��� �����Ͽ�, TEXT ��¿� �̿��Ѵ�.
	// 3.1 ���� ù Packet ���� ���Ѵ�.
	ILPCSTR szPacketName = m_pIChartManager->GetPacketNameStringOfGraph( m_strIndicatorName);
	CString strPacketName( szPacketName);
	int nNextPacketIndex = strPacketName.Find( ',');
	if( 0 < nNextPacketIndex) strPacketName = strPacketName.Left( nNextPacketIndex);

/*
	// 3.2 ù Packet���� Packet Object�� ���Ѵ�.
	IPacket *pIPacket = NULL;
	if( !strPacketName.IsEmpty())
	{
		// 3.3 Packet�� Packet Type Unit Value�� ���Ѵ�.
		pIPacket = m_pIDefaultPacketManager->GetPacket( strPacketName);
	}

	// 3.3 Packet�� Packet Unit Value (Precision)�� ���Ѵ�.
	m_dPacketUnitValue = 0;
	if( pIPacket)
	{
		m_dPacketUnitValue = pIPacket->GetPacketTypeUnitValue();
		pIPacket->Release();
	}
*/
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
//////////////////////////////////////////////////////////////////////

BOOL COrderLinkAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	// Load Chart Data Info I
	if( stricmp(p_szData, "on") == 0 )
	{
		m_bRun = true;
		// (2008/9/13 - Seung-Won, Bae) Set Mouse Exclusive
		m_pIAddInToolMgr->GetAllMouseExclusive( m_pIAddInDllBase);
	}
	else
	{
		m_bRun = false;
		// (2008/9/13 - Seung-Won, Bae) Set Mouse Exclusive
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// ȣ�� �ֹ�â ���� ��ư�� �������� �ش�Ǵ� ������ �����Ѵ�. 
// Param  const int p_nToolType : Button�� ��
// Param  const int p_nCmdOption : �������� ( 0 : �������� ����.	1 : ���� )
//-----------------------------------------------------------------------------
void COrderLinkAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	// (2007/1/29 - Seung-Won, Bae) Re-Notify the State to Tool UI
	if( CToolOptionInfo::T_ORDER_LINK == p_nToolType)
	{
		if( p_nCmdOption == 1 )			m_bRun = true;
		else							m_bRun = false;
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINK, m_bRun);
	}
}

// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
void COrderLinkAddInImp::OnResetChart( void)
{
	// (2005/9/29 - Seung-Won, Bae) Reset BoundMark Flag
	OnAddInToolCommand( EAI_CUSTOM_ENABLE_BOUNDMARK, TRUE);
}

// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
void COrderLinkAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	if( !m_bRun )	return;

	// 0. ���� Base�� Drawing�� ó���Ѵ�.
	CBoundMarkerAddInImp::OnDrawBeforeGraph( p_hDC);

	// 1. BoundMark�� ������ Ȯ���Ͽ�, Indicator Area�� ���Ѵ�.
	m_rctAskingPriceMark = m_rctBoundMark;
	m_rctAskingPriceMark.InflateRect( 2, 0);
}

// RQ�� Packet : �������� - ojtaso (20070111)
void COrderLinkAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// 0. ���� Base�� ���� Message�� ó����� �Ѵ�.
	CBoundMarkerAddInImp::OnPacketDataMultiItem(lpszRQ, p_szPacketNames, p_bWithReset);
}

// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
void COrderLinkAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	CBoundMarkerAddInImp::OnLoadEnvironment( p_pIAddInEnvMap);
}
void COrderLinkAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	CBoundMarkerAddInImp::OnSaveEnvironment( p_pIAddInEnvMap);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
RECT COrderLinkAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	if( !m_bRun )	return rct;

	if( 0 == m_nIndicatorPosY) return rct;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	// 2. Price Value�� �����Ѵ�. (View�� Data Min/Max�� Graph Region�� �̿��Ѵ�.)
	// (2004.10.27, ��¿�) Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	TRACE( "\r\n[ORDER_LINK] : %d", m_nIndicatorPosY);
	m_dIndicatedPrice = m_pIChartOCX->ConvertYPositionToData( m_nIndicatorPosY, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

	// 3. Asking Price Text�� ��� ������ �����Ѵ�.
	// (2004.10.26, ��¿�) ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
	int nDecimal = 0;
	// (2004.10.26, ��¿�, ����!) double�� Int�� ��ȯ�ϴ� �������� ��Ȯ�� Int�� ��ȯ���� �ʴ� ������ �ذ��ϱ� ���� 100�� ��ȿ�ڸ����� �򵵷� �����Ѵ�.
	if( m_dPacketUnitValue != 0) nDecimal = ( int)( ( log10( m_dPacketUnitValue) * 100) / 100);	
	CString strFormat;
	if( 0 <= nDecimal) strFormat = "%.0f";
	else strFormat.Format( "%%.%df", -nDecimal);
	// 3.1 Asking Price Text�� ���Ѵ�.
	// (2004.10.26, ��¿�) ��ȸ���� ȣ�� String�� �����Ͽ�, ���ý� �ٷ� ������ �߻��ϵ��� �Ѵ�.

	// 20081016 ����ǥ 8����/32���� ��ȯ >>
//	m_strAskingPrice.Format( strFormat, m_dIndicatedPrice);
	CString strPacketType;
	ILPCSTR szHelpMsg = NULL;
	IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( m_strRQ);
	if( pIPacketManager)
	{
		ILPCSTR strType;
		IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT( C0_CLOSE));
		if( pIPacket)
		{
			strType = pIPacket->GetType();
			pIPacket->Release();
		}
		strPacketType = strType;

		pIPacketManager->GetHelpMessageItem( "HBPRICE", szHelpMsg);
		pIPacketManager->Release();
	}
	// 20081106 ����ǥ >>

	CString strUnit(szHelpMsg);
	double dHoga = atof(strUnit);
	if( 0 < dHoga) m_dIndicatedPrice = long( m_dIndicatedPrice / dHoga + 0.5) * dHoga;
	// 20081106 ����ǥ <<
	m_strAskingPrice = CDataConversion::DoubleToString( m_dIndicatedPrice, strPacketType);
	// 20081016 ����ǥ <<

	// (2004.10.26, ��¿�) Comma�� �߰��Ѵ�.
	// 3.1.1 ','�� �ʿ��� ��� ������ Ȯ���Ѵ�.
	int nFixedDecimalLen = m_strAskingPrice.Find( '.');
	if( nFixedDecimalLen < 0) nFixedDecimalLen = m_strAskingPrice.GetLength();
	if( 3 < nFixedDecimalLen)
	{
		// 3.1.2 ','�� �߰��� Buffer�� Ȯ���Ѵ�. (������ �⺻ 2��� ��´�.)
		const char *szBuffer1 = m_strAskingPrice;
		CString strBuffer;
		char *szBuffer2 = strBuffer.GetBuffer( m_strAskingPrice.GetLength() * 2);
		// 3.1.3 Data�� �� '.'���� Ȯ���ϸ鼭, ','�� ����Ѵ�.
		// 20081016 ����ǥ
		if( m_strAskingPrice.Find( "'") < 0)
		{
			int i, j;
			for( i = 0, j = nFixedDecimalLen - 1; j >= 0; i++)
			{
				szBuffer2[ i] = szBuffer1[ j--];
				if( i % 4 == 2 && j >= 0 && szBuffer1[ j] != '-') szBuffer2[ ++i] = ',';
			}
			szBuffer2[ i] = '\0';
		}
		else strcpy( szBuffer2, szBuffer1);
		// 3.1.4 �ٽ� Data�� Revere��Ű�鼭 �����Ѵ�.
		int nIntLen = strlen( szBuffer2);
		CString strBuffer3;
		char *szFormatData = strBuffer3.GetBuffer( m_strAskingPrice.GetLength() * 2);
		szBuffer2 += nIntLen - 1;
		for( int i = 0; i < nIntLen; i++) *( szFormatData++) = *( szBuffer2--);
		// 3.1.5 �Ҽ��� ���ϸ� �ٽ� �߰��Ѵ�.
		do *( szFormatData++) = szBuffer1[ nFixedDecimalLen];	while( szBuffer1[ nFixedDecimalLen++] != '\0');
		strBuffer3.ReleaseBuffer();
		// 3.1.6 Buffer�� �������Ѵ�.
		strBuffer.ReleaseBuffer();
		// 3.1.7 Format�� Data�� �����Ѵ�.
		m_strAskingPrice = strBuffer3;
	}
	// 3.2 Price TEXT�� ����ũ�⸦ ���Ѵ�.
	CSize szPriceText = pDC->GetTextExtent( m_strAskingPrice);
	
	// 4. Asking Price Text�� ��� ������ Backup�Ѵ�.
	// 4.0 ���� TEXT�� ��� Position�� Ȯ���Ѵ�.
	CPoint ptPrice( m_rctBoundMark.right, m_nIndicatorPosY - szPriceText.cy / 2);
	// (2004.10.27, ��¿�) ���� Scale�� ���� ���, (�� BoundMark�� ũ�� ���� ������, Text�� ��ġ�� �������� �����Ѵ�.
	CRect rctChartOcx;
	m_pChartCWnd->GetClientRect( &rctChartOcx);
	if( !rctChartOcx.PtInRect( ptPrice + CSize( szPriceText.cx, 0)))
		ptPrice.x = m_rctBoundMark.left - szPriceText.cx - 10;

	// (2004.10.25, ��¿�) TEXT�� ������ Left �������� �����ϰ�, ũ�⸦ �����¿츦 �÷��� ������ �ǵ��� �Ѵ�.
	rct = CRect( ptPrice.x, ptPrice.y - 2, ptPrice.x + szPriceText.cx + 5, ptPrice.y + szPriceText.cy + 1);
	// (2006/11/5 - Seung-Won, Bae) Use MainOSB

	// 5. �� ��ġ�� ȣ���� ǥ���Ѵ�.
	// 5.1 NOT Line�� ����Ѵ�.
	int nPreROP = pDC->SetROP2( R2_NOT);
	pDC->MoveTo( m_rctGraphRegion.left,		m_nIndicatorPosY);
	pDC->LineTo( m_rctGraphRegion.right,	m_nIndicatorPosY);
	pDC->SetROP2( nPreROP);
	// 5.2 ȣ�� TEXT�� ����Ѵ�.
	CPen *	pOldPen		= ( CPen *)pDC->SelectStockObject( BLACK_PEN);
	CBrush *pOldBrush	= ( CBrush *)pDC->SelectStockObject( WHITE_BRUSH);
	pDC->Rectangle( &rct);
	pDC->SelectObject( pOldBrush);
	pDC->SelectObject( pOldPen);
	pDC->TextOut( rct.left + 3, rct.top + 2, m_strAskingPrice);	// Border�� �����Ͽ� ��ġ�� �����Ѵ�.

	rct.UnionRect( rct, CRect( m_rctGraphRegion.left, m_nIndicatorPosY, m_rctGraphRegion.right,	m_nIndicatorPosY + 1));
	return rct;
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL COrderLinkAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_CUSTOM_ENABLE_BOUNDMARK != p_eCommandID) return FALSE;

	// (2004.10.22, ��¿�) ��/���Ѱ� ǥ�ñ�ɿ� ȣ������ ����� �����Ѵ�.
	EnableBoundMark( p_lParam);	

	// (2004.10.06, ��¿�) ������ �ٷ� ȭ�鿡 �ݿ��ǵ��� Redraw��Ų��.
	// (2004.12.13, ��¿�) NULL Pointer�� Ȯ���Ѵ�.
	m_pIChartOCX->InvalidateControl();

	return TRUE;
}

BOOL COrderLinkAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
{
	switch( p_eCommandID)
	{
		case EAI_CUSTOM_GET_BOUNDMARK_FLAG:	p_lResult = GetBoundMarkFlag();
											break;
		case EAI_CUSTOM_GET_EXISTENCE_BOUNDMARK:	p_lResult = IsExistenceBoundMark();
													break;
		default:	return FALSE;
	}
	return TRUE;
}

// Chart View Info Collection
BOOL COrderLinkAddInImp::GetChartBasicData( void)
{
	// �� Data View�� Start/End Index�� ���Ѵ�.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex)) return FALSE;

	int nR = 0, nC = 0, nV = 0, nG = 0;
	// ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( pIBlock)
	{
		m_bLog = pIBlock->IsLogScale();
		m_bReverse = pIBlock->IsVScaleInvert( nV);

		pIBlock->GetVScaleGroupMinMax( nV, m_dViewMin, m_dViewMax);
//	   	pIBlock->GetGraphRegions( m_rctGraphRegion, m_rctBlockRegion);
//		m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();

		pIBlock->Release();
		pIBlock = NULL;
	}

	return TRUE;
}


void COrderLinkAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	GetChartBasicData();
}

BOOL COrderLinkAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. �ֹ�â ���� ���� ������ Ȯ��
	if( !m_bRun) return FALSE;
	if( !m_rctGraphRegion.PtInRect(point) )		return FALSE;

	// 2. �ֹ�â ���� ó�� Cursor�� �����ش�.
	// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
	//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
	//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
	ML_SET_DEFAULT_RES();
	m_hAPCursor = AfxGetApp()->LoadCursor( IDC_AP_INDICATOR);
	ASSERT( m_hAPCursor);
	m_pIChartOCX->SetCursorHandle( m_hAPCursor);

	m_dIndicatedPrice = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

	// (2004.10.26, ��¿�) ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
	int nDecimal = 0;
	// (2004.10.26, ��¿�, ����!) double�� Int�� ��ȯ�ϴ� �������� ��Ȯ�� Int�� ��ȯ���� �ʴ� ������ �ذ��ϱ� ���� 100�� ��ȿ�ڸ����� �򵵷� �����Ѵ�.
	if( m_dPacketUnitValue != 0) nDecimal = ( int)( ( log10( m_dPacketUnitValue) * 100) / 100);	

	if( CChartInfo::WORLD_ON == m_eChartMode)
	{
		// 20081020 ����ǥ >>
		CString strPacketType;
		IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( m_strRQ);
		if( pIPacketManager)
		{
			ILPCSTR strType;
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT( C0_CLOSE));
			if( pIPacket)
			{
				strType = pIPacket->GetType();
				pIPacket->Release();
			}
			strPacketType = strType;

			ILPCSTR szHelpMsg = NULL;
			pIPacketManager->GetHelpMessageItem( "HBPRICE", szHelpMsg);
			CString strUnit( szHelpMsg);
			double dHoga = atof( strUnit);
			if( 0 < dHoga) m_dIndicatedPrice = long( m_dIndicatedPrice / dHoga + 0.5) * dHoga;
			m_strAskingPrice = CDataConversion::DoubleToString( m_dIndicatedPrice, strPacketType);
			// 20081020 ����ǥ <<
			
			pIPacketManager->Release();
		}
		
		// 20081031 ����ǥ >>
		const char *szItemCode = NULL, *szItemName = NULL;	
		m_pIPacketListManager->GetItemCode( m_strRQ, szItemCode, szItemName);
		CString strData;
		strData.Format( "%s,%s", szItemCode, m_strAskingPrice);
		// 20081031 ����ǥ <<

		// (2005/8/18 - Seung-Won, Bae) Fire Event for Asking Price Data
		m_pIChartOCX->FireAddInEvent( "ChartCommonAddIn.dll:ORDER_LINK", 1, strData, 0, 0);
	}
	else
	{
		CString strFormat;
		if( 0 <= nDecimal) strFormat = "%.0f";
		else strFormat.Format( "%%.%df", -nDecimal);

		// 3.1 Asking Price Text�� ���Ѵ�.
		m_strAskingPrice.Format( strFormat, m_dIndicatedPrice);
		m_dIndicatedPrice = atof(m_strAskingPrice);

		// (2005/8/18 - Seung-Won, Bae) Fire Event for Asking Price Data
		m_pIChartOCX->FireAddInEvent( "ChartCommonAddIn.dll:ORDER_LINK", 1, "", m_dIndicatedPrice, 0);
	}

	return TRUE;	// ȣ������ ó���� �ٸ� Mouse Action�� �̾����� �ʵ��� �Ѵ�.
}

// (2004.10.25, ��¿�) Mouse�� ��ġ�ÿ�, Indicator Cursor�� ����ϵ��� �Ѵ�.
BOOL COrderLinkAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	if( !m_bRun)	return FALSE;

	// 20081031 ����ǥ >>
	int nRow, nCol;
	m_pIChartManager->GetBlockIndex(point, nRow, nCol);
	if( nRow < 0 || nCol < 0) return FALSE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nCol);
	if( !pIBlock) return FALSE;

	ILPCSTR szRQ = m_pIChartManager->GetMainRQofBlock( nRow, nCol);
	m_strRQ = szRQ;

	int p_nV = -1, p_nG = -1;
	ILPCSTR szGraphName = pIBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), p_nV, p_nG, m_strRQ);
	if( szGraphName.IsEmpty())
	{
		pIBlock->Release();
		return FALSE;
	}

	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( 0);
	pIBlock->GetVScaleGroupMinMax( 0, m_dViewMin, m_dViewMax);
	pIBlock->GetGraphRegions( m_rctGraphRegion, m_rctBlockRegion);
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	
	pIBlock->Release();
	pIBlock = NULL;
	// 20081031 ����ǥ <<

	if( !m_rctGraphRegion.PtInRect(point))		return FALSE;

	// 2.1 ó�� �����ϴ� ��츦 ó���Ѵ�.
	//	(ó�� �����ϸ鼭, ���� Mouse Button�� ������ ���� �����̸�, Ư���� Cursor Mode�� �ƴ� ���)
	//		Ư��, ���� ������ ���� ���� ���� �������� �ʴ´�.)
	if( !m_bOnSetCaprture && !nFlags )//&& !m_strItemCode.IsEmpty())
	{
		// 2. Mouse Capturing Flag�� TRUE�� �����Ѵ�.
		m_bOnSetCaprture = TRUE;

		// 3. ȣ������ Cursor�� ����Ѵ�.
		// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
		//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
		//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
		ML_SET_DEFAULT_RES();
		m_hAPCursor = AfxGetApp()->LoadCursor( IDC_AP_INDICATOR);
		ASSERT( m_hAPCursor);
	}

	// 2.2 ������ �̵��ϴ� ��츦 ó���Ѵ�.
	TRACE( "\r\n[ORDER_LINK2] : %d", point.y);
	if( m_bOnSetCaprture) m_nIndicatorPosY = point.y;	// Set Indicator Drawing Position			(IndicatorPosition)
	// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
	//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
	//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
	if( m_hAPCursor) m_pIChartOCX->SetCursorHandle( m_hAPCursor);

	// (2008/9/13 - Seung-Won, Bae) Set Mouse Exclusive
	return TRUE;
}

