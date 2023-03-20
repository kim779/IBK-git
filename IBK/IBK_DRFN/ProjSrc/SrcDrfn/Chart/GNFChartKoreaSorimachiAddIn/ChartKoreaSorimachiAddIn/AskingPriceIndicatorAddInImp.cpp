// AskingPriceIndicatorAddInImp.cpp: implementation of the CAskingPriceIndicatorAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartKoreaSorimachiAddIn.h"
#include "AskingPriceIndicatorAddInImp.h"

#include <math.h>												// for log()
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_AddIn_381931/I381931/_AddInCommand.h"		// for EAI_CUSTOM_ENABLE_BOUNDMARK
#include "../Include_AddIn_381931/I381931/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn_381931/I381931/_IPacket.h"			// for IPacket
#include "../Include_AddIn_381931/I381931/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn_381931/I381931/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn_381931/I381931/_IString.h"			// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAskingPriceIndicatorAddInImp::CAskingPriceIndicatorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CBoundMarkerAddInImp( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.10.25, ��¿�) ȣ�������� ���� Member Variable�� �����Ѵ�.
	m_nIndicatorPosY			= 0;				// Current Indicator Drawing Position			(IndicatorPosition)
	m_bOnSetCaprture			= FALSE;			// Indicator Drawing Situation Flag			(SetCaptureFlag)
	m_bOnFireAskingPriceGShare	= FALSE;			// Asking Price GShare Firing Flag			(AskingPriceGShareFireFlag)
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
	m_hAskingPriceFixDLL = LoadLibrary( "PriceUnit.dll");
	if( m_hAskingPriceFixDLL) m_fnFnGetPriceUnit = ( FnGetPriceUnit)::GetProcAddress( m_hAskingPriceFixDLL, "__GetPriceUnit");
	else m_fnFnGetPriceUnit = NULL;

}

CAskingPriceIndicatorAddInImp::~CAskingPriceIndicatorAddInImp()
{
	// (2004.11.01, ��¿�) �屸�а� ȣ������ DLL�� �����Ͽ�, ȣ�� ������ �̿��Ѵ�.
	if( m_hAskingPriceFixDLL)
	{
		FreeLibrary( m_hAskingPriceFixDLL);
		m_hAskingPriceFixDLL = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CAskingPriceIndicatorAddInImp::m_strAddInItemName = "ASKED_PRICE_CKSA";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CAskingPriceIndicatorAddInImp)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2004.10.25, ��¿�) ȣ�������� ���� Library Routine�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.25, ��¿�) ȣ������ ó�� Mark�� Clear��Ű�� Routine�� �����Ѵ�.
void CAskingPriceIndicatorAddInImp::ClearAskingPriceGShareFireFlag( void)
{
	// 1. ȣ������ ó�� Flag�� �����Ѵ�.
	m_bOnFireAskingPriceGShare = FALSE;

	// 2. ȣ������ ���� Cursor�� �����ش�.
	// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
	//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
	//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
	AfxSetResourceHandle( ML_DEFAULT_RES);
	m_hAPCursor = ::LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_AP_INDICATOR));
	ASSERT( m_hAPCursor);
	m_pIChartOCX->SetCursorHandle( m_hAPCursor);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.25, ��¿�) Bound���� �����ϴ� Interface�� �����Ѵ�.
//		Ȯ��� �����ڵ� ������ �޾� �����Ͽ� ��������ÿ� �̿��Ѵ�.
//////////////////////////////////////////////////////////////////////
void CAskingPriceIndicatorAddInImp::ResetBound( const char *p_szBound)
{
	// 0. ���� Base�� Bound Info�� ���� �����Ѵ�.
	CBoundMarkerAddInImp::ResetBound( p_szBound);

	// 0.5 ������ ��츦 ����Ͽ� �̸� Item Code�� Clear��Ų��.
//	m_strItemCode.Empty();

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
//	m_strItemCode = szItemCode;
	// (2004.11.01, ��¿�) �����ڵ�� RTime�� ��Ų��. Server������ �⺻ 7�ڸ��� �����Ѵٰ� �Ѵ�.
//	m_strItemCode.TrimRight();
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
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
//////////////////////////////////////////////////////////////////////

// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
void CAskingPriceIndicatorAddInImp::OnResetChart( void)
{
	// (2005/9/29 - Seung-Won, Bae) Reset BoundMark Flag
	OnAddInToolCommand( EAI_CUSTOM_ENABLE_BOUNDMARK, TRUE);
}

// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
void CAskingPriceIndicatorAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	// 0. ���� Base�� Drawing�� ó���Ѵ�.
	CBoundMarkerAddInImp::OnDrawBeforeGraph( p_hDC);

	// 1. BoundMark�� ������ Ȯ���Ͽ�, Indicator Area�� ���Ѵ�.
	m_rctAskingPriceMark = m_rctBoundMark;
	m_rctAskingPriceMark.InflateRect( 2, 0);
}

// RQ�� Packet : �������� - ojtaso (20070111)
void CAskingPriceIndicatorAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// 0. ���� Base�� ���� Message�� ó����� �Ѵ�.
	CBoundMarkerAddInImp::OnPacketDataMultiItem(lpszRQ, p_szPacketNames, p_bWithReset);
}

// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
void CAskingPriceIndicatorAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	CBoundMarkerAddInImp::OnLoadEnvironment( p_pIAddInEnvMap);
}
void CAskingPriceIndicatorAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	CBoundMarkerAddInImp::OnSaveEnvironment( p_pIAddInEnvMap);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
RECT CAskingPriceIndicatorAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	if( 0 == m_nIndicatorPosY) return rct;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	// 2. Price Value�� �����Ѵ�. (View�� Data Min/Max�� Graph Region�� �̿��Ѵ�.)
	// (2004.10.27, ��¿�) Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
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
	m_strAskingPrice.Format( strFormat, m_dIndicatedPrice);
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
		int i, j;
		for( i = 0, j = nFixedDecimalLen - 1; j >= 0; i++)
		{
			szBuffer2[ i] = szBuffer1[ j--];
			if( i % 4 == 2 && j >= 0 && szBuffer1[ j] != '-') szBuffer2[ ++i] = ',';
		}
		szBuffer2[ i] = '\0';
		// 3.1.4 �ٽ� Data�� Revere��Ű�鼭 �����Ѵ�.
		int nIntLen = i;
		CString strBuffer3;
		char *szFormatData = strBuffer3.GetBuffer( m_strAskingPrice.GetLength() * 2);
		szBuffer2 += nIntLen - 1;
		for( i = 0; i < nIntLen; i++) *( szFormatData++) = *( szBuffer2--);
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

BOOL CAskingPriceIndicatorAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_CUSTOM_ENABLE_BOUNDMARK != p_eCommandID) return FALSE;

	// (2004.10.22, ��¿�) ��/���Ѱ� ǥ�ñ�ɿ� ȣ������ ����� �����Ѵ�.
	EnableBoundMark( p_lParam);	

	// (2004.10.06, ��¿�) ������ �ٷ� ȭ�鿡 �ݿ��ǵ��� Redraw��Ų��.
	// (2004.12.13, ��¿�) NULL Pointer�� Ȯ���Ѵ�.
	m_pIChartOCX->InvalidateControl();

	return TRUE;
}

BOOL CAskingPriceIndicatorAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
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

// (2004.10.25, ��¿�) Mouse�� ��ġ�ÿ�, Indicator Cursor�� ����ϵ��� �Ѵ�.
BOOL CAskingPriceIndicatorAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// 1. ���� ȣ�������� ������ ��츦 ó���Ѵ�. (ȣ������ Mark�� Clear�Ѵ�.)
	if( m_bOnFireAskingPriceGShare) ClearAskingPriceGShareFireFlag();

	// 2. ȣ������ ������ Mouse�� ��ġ�� ��츦 ó���Ѵ�.
	if( m_rctAskingPriceMark.PtInRect( point))
	{
		// 2.1 ó�� �����ϴ� ��츦 ó���Ѵ�.
		//	(ó�� �����ϸ鼭, ���� Mouse Button�� ������ ���� �����̸�, Ư���� Cursor Mode�� �ƴ� ���)
		//		Ư��, ���� ������ ���� ���� ���� �������� �ʴ´�.)
		if( !m_bOnSetCaprture && !nFlags && !m_strItemCode.IsEmpty())
		{
			// 2. Mouse Capturing Flag�� TRUE�� �����Ѵ�.
			m_bOnSetCaprture = TRUE;

			// 3. ȣ������ Cursor�� ����Ѵ�.
			// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
			//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
			//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
			AfxSetResourceHandle( ML_DEFAULT_RES);
			m_hAPCursor = AfxGetApp()->LoadCursor( IDC_AP_INDICATOR);
			ASSERT( m_hAPCursor);
		}

		// 2.2 ������ �̵��ϴ� ��츦 ó���Ѵ�.
		if( m_bOnSetCaprture) m_nIndicatorPosY = point.y;	// Set Indicator Drawing Position			(IndicatorPosition)
	}

	// 3. ȣ������ ���� �ۿ� Mouse�� ��ġ�� ��츦 ó���Ѵ�. (�̹� ������ ���, ����� ������ ó���Ѵ�.)
	else
	{
		// (2004.10.29, ��¿�) Capturing�� Release�� Window Deactivate�ÿ��� ��Ÿ���Ƿ�,
		//		�� ��ó���� ���Ͽ� Capturing�� Release�� ó���� OnCaptureChanged()���� ó����� �Ѵ�.
		//		Ư�� ReleaseCapture()�ÿ� ���� ClearPreviousIndicatorDraw()�� �� �ֵ��� �Ѵ�.
		// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �̿��Ѵ�.
		if( m_bOnSetCaprture)
		{
			// (2004.10.25, ��¿�) ȣ������ Indicator�� ����� �������Ǿ�, ��� Flag�� Reset�Ѵ�.
			// (2004.10.29, ��¿�) Capturing�� Release�� Window Deactivate�ÿ��� ��Ÿ���Ƿ�,
			//		�� ��ó���� ���Ͽ� Capturing�� Release�� ó���� OnCaptureChanged()���� ó����� �Ѵ�.
			// 1. Mouse Capturing Flag�� FALSE�� �����Ѵ�.
			m_bOnSetCaprture = FALSE;

			// 3. ȣ������ Cursor�� �����Ѵ�.
			// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
			//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
			//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
			m_hAPCursor = NULL;
			if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);

			// 6. Clear Y Position for DrawCursor();
			m_nIndicatorPosY = 0;
		}
	}

	// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
	//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
	//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
	if( m_hAPCursor) m_pIChartOCX->SetCursorHandle( m_hAPCursor);

	// (2006/1/22 - Seung-Won, Bae) return FALSE to pass the Event to a next AddIn Routine
	return FALSE;
}

// (2005/8/18 - Seung-Won, Bae) Define AddIn Event ID
typedef enum {	m_eAskingPriceValue = 0};

// (2004.10.25, ��¿�) ȣ�� ���� ���ý�, ������ �߻���Ų��.
BOOL CAskingPriceIndicatorAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. ���� ȣ������ ������ ��ġ�� ��Ȳ���� Ȯ���Ѵ�.
	if( !( m_rctAskingPriceMark.PtInRect( point) && m_bOnSetCaprture)) return FALSE;

	// 2. ȣ������ ó�� Cursor�� �����ش�.
	// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
	//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
	//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
	AfxSetResourceHandle( ML_DEFAULT_RES);
	m_hAPCursor = AfxGetApp()->LoadCursor( IDC_AP_INDICATOR);
	ASSERT( m_hAPCursor);
	m_pIChartOCX->SetCursorHandle( m_hAPCursor);

	// 3. ȣ������ ó�� Flag�� �����Ѵ�.
	m_bOnFireAskingPriceGShare = TRUE;

	// 4. ȣ�� ���� ������ �߻���Ų��.
	CString strShareData = m_strItemCode + "?" + m_strAskingPrice;
	m_pIChartOCX->FireGSharedData( _MTEXT( C6_ASKED_PRICE_CHART), strShareData);

	// (2005/8/18 - Seung-Won, Bae) Modify to Fire Shared Data, even if without Asking Price Unit.
	double dFixedAskingPrice = _FnGetPriceUnit_ERROR;
	// (2004.11.01, ��¿�) �屸���� �����Ͽ�, �⺻ ȣ�� ������ �̿��Ѵ�.
	if( m_nMarketType != -1 && m_fnFnGetPriceUnit && m_dYClosePrice && m_nRatioByYCP)
	{
		// FnGetPriceUnit( �屸��,  ����,  ��������,  ����/���������[nFlag 1:����, 2:�����] )
		//		Error Value -99999999
		dFixedAskingPrice = m_fnFnGetPriceUnit( m_nMarketType, m_dIndicatedPrice, m_dYClosePrice, m_nRatioByYCP);
	}
	if( dFixedAskingPrice == _FnGetPriceUnit_ERROR) dFixedAskingPrice = m_dIndicatedPrice;

	CString strFixedAskingPrice;
	// (2004.10.26, ��¿�) ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
	int nDecimal = 0;
	// (2004.10.26, ��¿�, ����!) double�� Int�� ��ȯ�ϴ� �������� ��Ȯ�� Int�� ��ȯ���� �ʴ� ������ �ذ��ϱ� ���� 100�� ��ȿ�ڸ����� �򵵷� �����Ѵ�.
	nDecimal = ( int)( ( log10( dFixedAskingPrice) * 100) / 100);	
	CString strFormat;
	if( 0 <= nDecimal) strFormat = "%.0f";
	else strFormat.Format( "%%.%df", -nDecimal);
	strFixedAskingPrice.Format( strFormat, dFixedAskingPrice);

	m_pIChartOCX->FireGSharedData( _MTEXT( C6_CODE_ASKED_PRICE), m_strItemCode + "?" + strFixedAskingPrice);
	// (2005/8/18 - Seung-Won, Bae) Fire Event for Asking Price Data
	m_pIChartOCX->FireAddInEvent( AfxGetApp()->m_pszAppName, m_eAskingPriceValue, strFixedAskingPrice, 0, 0);

	return TRUE;	// ȣ������ ó���� �ٸ� Mouse Action�� �̾����� �ʵ��� �Ѵ�.
}

// (2004.10.25, ��¿�) ȣ�� ���� ���ý��� Marking�� �����Ѵ�.
BOOL CAskingPriceIndicatorAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( m_bOnFireAskingPriceGShare) ClearAskingPriceGShareFireFlag();

	return FALSE;
}

BOOL CAskingPriceIndicatorAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	if (strcmp(p_szCommandName, "1") == 0)
	{
		m_pmapOrder = (CMapStringToOb *)atoi(p_szData);
	}
	else if (strcmp(p_szCommandName, "2") == 0)
	{
		m_strItemCode = p_szData;
	}
	else if (strcmp(p_szCommandName, "3") == 0)
	{
		m_bDraw = atoi(p_szData);
	}
	return TRUE;
}
