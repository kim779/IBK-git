// MousePositionExportingAddInImp.cpp: implementation of the CMousePositionExportingAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chartcommonaddin.h"
#include "MousePositionExportingAddInImp.h"

#include <math.h>	
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMousePositionExportingAddInImp::CMousePositionExportingAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_pIBlock = NULL;

	// (2007/1/17 - Seung-Won, Bae) Support Data Index with Default.
	m_bXDataExport = FALSE;

	// (2007/1/19 - Seung-Won, Bae) Support Not-Change Data Filter
	m_bSameDataFilter = TRUE;
}


CMousePositionExportingAddInImp::~CMousePositionExportingAddInImp()
{
	if( m_pIBlock )
	{
		m_pIBlock->Release();
		m_pIBlock = NULL;
	}
}


// ( 2006 / 11 / 08 - Sang-Woo, Cho ) - Multiple Item in DLL
CString CMousePositionExportingAddInImp::m_strAddInItemName = "MOUSE_POSITION";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CMousePositionExportingAddInImp )
	ONADDINEVENT( OnMouseMove)
END_ADDIN_EVENT_MAP()


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Modifier		: 
// Comments		: Mouse�� �����϶� FireAddInEvent()�� Map���� ���� Mouse Point��
//				: X���� Y���� Map���� �����ش�.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// (2006/3/2 - Seung-Won, Bae) Manage State for Resource Loading of Cursor 
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	// 
	if( !GetScaleData( point ) )
		return FALSE;
	
	CString strXScaleAndYScaleData = m_strXScaleFormatedData + "|" + m_strYScaleFormatedData;
	
	// Fire Event for Asking Price Data
	if( !m_bSameDataFilter || m_dData != m_dYScaleOriginalData || m_lData != (long)m_dXScaleOriginalData)
		m_pIChartOCX->FireAddInEvent( m_strAddInItemName, 0, strXScaleAndYScaleData, m_dYScaleOriginalData, (long)m_dXScaleOriginalData);
	else
	{
		m_dData = m_dYScaleOriginalData;
		m_lData = (long)m_dXScaleOriginalData;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: ���� Mouse�� point�� X���� Y���� ���´�.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetScaleData( CPoint ptPoint )
{
	// 1. Block���� ���´�.
	if( !GetIBlock( ptPoint ) )
		return FALSE;


	// 2. Graph�� ������ ���´�.
	if( !GetGraphBlockRegion( ptPoint ) )
		return FALSE;
	

	if( !GetXScaleDataInPoint(ptPoint) )
		return FALSE;
	
	if( !GetYScaleDataInPoint(ptPoint) )
		return FALSE;

	IPacket *pIPacket = NULL;
	double dDataType = 0.0L;
	BOOL bResult = m_pIBlock->GetVScaleGroupHeadPacketAndDataType( m_nVScaleGroupIndex, pIPacket, dDataType);
	if( !bResult)
	{
		if( pIPacket) pIPacket->Release();
		return FALSE;
	}

	if( pIPacket)
	{
		// ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
		int nDecimal = 0;

		if( dDataType != 0)
		{
			m_dYScaleOriginalData = int( m_dYScaleOriginalData / dDataType + 0.5) * dDataType;
			// double�� Int�� ��ȯ�ϴ� �������� ��Ȯ�� Int�� ��ȯ���� �ʴ� ������ �ذ��ϱ� ���� 100�� ��ȿ�ڸ����� �򵵷� �����Ѵ�.
			nDecimal = ( int)( ( log10( dDataType) * 100) / 100);	
		}
	
		CString strFormat;
		if( 0 <= nDecimal)
			strFormat = "%.0f";
		else
			strFormat.Format( "%%.%df", -nDecimal);
				

		m_strYScaleFormatedData.Format( strFormat, m_dYScaleOriginalData);
		
#ifdef _DEBUG
	TRACE( "%s : %f\r\n", m_strYScaleFormatedData, m_dYScaleOriginalData);
#endif

		pIPacket->Release();
	}

	if( m_pIBlock) m_pIBlock->Release();
	m_pIBlock = NULL;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: Mouse Position ������ IBlock�� ���´�.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetIBlock( CPoint ptPoint )
{
	// 1. Mouse Point ������ Row, Column�� ���´�.
	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( ptPoint, nRow, nColumn))
		return FALSE;

	if( m_pIBlock )
	{
		m_pIBlock->Release();
		m_pIBlock= NULL;
	}

	// 2. Row, Column������ Block�� ���´�.
	m_pIBlock = m_pIChartManager->GetBlock( nRow, nColumn );
	if( !m_pIBlock )
		return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: GraphDrawRegion, BlockRegion�� ���ϰ� ���� Cursor�� ������
//				: BlockRegion �ȿ� �ִ��� �����Ѵ�.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetGraphBlockRegion( CPoint ptPoint )
{
	// 1. IBlock ������ ������ Return�Ѵ�.
	if( m_pIBlock == NULL )
		return FALSE;

	// 2. Block�� Region�� ���Ѵ�.
	CRect rctNoFrameAndMargin;
	m_pIBlock->GetGraphRegions( m_rctBlockRegion, rctNoFrameAndMargin);

	// 3. Block������ Graph Resion�� ���Ѵ�.
	m_rctGraphDrawingRegion = m_pIBlock->GetGraphDrwingRegion();

	// 4. Block Region�� �����Ѵ�.
	m_rctBlockRegion.left = m_rctGraphDrawingRegion.left;
	m_rctBlockRegion.right = m_rctGraphDrawingRegion.right;
	
	// 5. ���� Mouse Position�� BlockRegion�ۿ� ���� ��쿡�� return�Ѵ�.
	if( !m_rctBlockRegion.PtInRect(ptPoint) ) 
		return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: Mouse Position���� Index�� ���ϰ� Packet���� ���´�.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetXScaleDataInPoint( CPoint ptPoint )
{
	// 1. ���� ȭ�鿡 ���̴� Data�� Start / End Index�� ���Ѵ�.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex ) )
		return FALSE;


	// 2. Mouse Point�� Data Index�� ���Ѵ�.
	int nDataOnPage = nEndIndex - nStartIndex + 1;
	double dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );
	int nDataIndex = nStartIndex + int( double( ptPoint.x - m_rctBlockRegion.left ) / dBarWidth );


	// 3. XScale�� Data Index���� Packet Type�� ��´�.
	IPacket *pIPacket = m_pIBlock->GetHorzScaleDataPacket();
	if( pIPacket == NULL )
		return FALSE;


	// 2. Data�� ���Ѵ�.
	ILPCSTR szXScaleData;
	ILPCSTR szType = pIPacket->GetType();
	m_strPacketType = szType;
	
	// (2007/1/17 - Seung-Won, Bae) Support Data Index with Default.
	if( !m_bXDataExport) m_dXScaleOriginalData = nDataIndex;
	else if( !strcmp( m_strPacketType, _MTEXT( C6_CHARACTER)) )
	{
		m_bPacketTypeIsString = TRUE;
		pIPacket->GetData(nDataIndex, szXScaleData);

		m_strXScaleFormatedData = szXScaleData;
		m_dXScaleOriginalData = nDataIndex;
	}

	else
	{
		m_bPacketTypeIsString = FALSE;
		if( !pIPacket->GetData(nDataIndex, m_dXScaleOriginalData) )
			pIPacket->GetDataPastandFuture( nDataIndex, m_dXScaleOriginalData );

		// 6. Span Data�� �ش�Ǵ� ���ڿ��� ���Ѵ�.
		GetStringfromDataIdx( nDataIndex );
	}

	pIPacket->Release();
	pIPacket = NULL;

	return TRUE;
}


		
BOOL CMousePositionExportingAddInImp::GetYScaleDataInPoint( CPoint ptPoint )
{ 
	double	dViewMin;					// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double	dViewMax;					// View���� ��� Graph�� View�� Maximum Scale ���̴�.

	m_nVScaleGroupIndex = m_pIBlock->GetFirstVisibleVScaleIndex();
	m_pIBlock->GetVScaleGroupMinMax( m_nVScaleGroupIndex, dViewMin, dViewMax);

	// Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	// ������Ʈ�� Block�� Log/������ �������� Ȯ���Ѵ�.
	BOOL bLog = m_pIBlock->IsLogScale();
	BOOL bReverse = m_pIBlock->IsVScaleInvert( m_nVScaleGroupIndex);

	m_dYScaleOriginalData = m_pIChartOCX->ConvertYPositionToData( ptPoint.y,
		m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, dViewMax, dViewMin, bLog, bReverse);

	return TRUE;
}


// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
BOOL CMousePositionExportingAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	m_bXDataExport = p_lData;
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 15
// Return BOOL	: 
// Param  int nDataIdx : 
// Param  CString &p_strSpan : 
// Comments		: �ش� Data Index�� �ð� ������ ���Ѵ�.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetStringfromDataIdx( int nDataIdx )
{
	if( !m_bPacketTypeIsString )
	{
		ILPCSTR szDateCompart = m_pIBlock->GetDateCompart();
		m_strDateCompartment = szDateCompart;

		ILPCSTR szTimeCompart = m_pIBlock->GetTimeCompart();
		m_strTimeCompartment = szTimeCompart;
	}

	long lXScaleOriginalData = (long)m_dXScaleOriginalData;
	CString szXScaleOriginalData;
	szXScaleOriginalData.Format("%d", lXScaleOriginalData);

	// 3. Packet Type�� ���Ѵ�.
	int nPacketTypeLength = m_strPacketType.GetLength();

	// 4. PacketType�� ���̿� Data�� ���̰� �ٸ��� '0'���� ä���ش�.
	for( int i = nPacketTypeLength - szXScaleOriginalData.GetLength() ; i ; i-- )
		szXScaleOriginalData = "0" + szXScaleOriginalData;

	char strChar = m_strPacketType.GetAt(0);
	m_strXScaleFormatedData = szXScaleOriginalData.GetAt(0);

	// 5. XScale�� ���Ǵ� ������ �� Type�� �����Ѵ�.
	CString strCompartment = m_strDateCompartment;
	BOOL bUseOneCharDateCompartment = ( m_strDateCompartment.GetLength() > 1 ) ? FALSE : TRUE;
	BOOL bUseOneCharTimeCompartment = ( m_strTimeCompartment.GetLength() > 1 ) ? FALSE : TRUE;

	
	// "��"�� "��"�� PacketType�� ���� 'M'�� ����ϱ� ������ �������ش�.
	BOOL bIsMonth = FALSE;				
	CString strPacketChar = "";
	
	// 6. PacketType�� ���̸�ŭ ���ڿ��� parsing�Ѵ�.
	ML_SET_LANGUAGE_RES();
	for( int nIdx = 1 ; nPacketTypeLength > nIdx ; nIdx++ )
	{
		strPacketChar = m_strPacketType.GetAt(nIdx);

		// 6.1 PacketType������ Data�� Type�� �ٲ�� �ش�Ǵ� �����ڸ� ���ڿ��� �߰����ش�.
		if( strChar != strPacketChar )
		{
			// 6.1.1 Date�� �����ڰ� "�� �� ��"�� ����� ��� 
			if( !bUseOneCharDateCompartment )
			{
				if( strChar == 'Y' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_YEAR);
					bIsMonth = TRUE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'D' )		strCompartment.LoadString( IDS_MONTH);
				else if( strChar == 'D' && strPacketChar == 'H' )		strCompartment.LoadString( IDS_DAY);
			}

			// 6.1.2 Time�� �����ڰ� "�� �� ��"�� ����� ��� 
			if( !bUseOneCharTimeCompartment )
			{
				if( strChar == 'H' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_HOUR);
					bIsMonth = FALSE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'S' )		strCompartment.LoadString( IDS_MINUTE);
				else if( strChar == 'S' && strPacketChar == 'N' )		strCompartment.LoadString( IDS_SECOND);
			}


			// 6.1.3 ���� �����ڰ� �Ϲ� �����ϰ�� ��¥���� �ð����� ����� �� ���� ���ڸ� �������ش�.
			if( strChar == 'H' && strPacketChar == 'M' && bUseOneCharTimeCompartment )
				strCompartment = m_strTimeCompartment;

			m_strXScaleFormatedData = m_strXScaleFormatedData + strCompartment + szXScaleOriginalData.GetAt(nIdx);

			strChar = m_strPacketType.GetAt(nIdx);
		}

		// 6.2 PacketType�� ������� ������ ���ڸ� 
		else
		{
			// 6.2.1 PacketType�� ������ ������ Span���ڸ� �����ش�.
			m_strXScaleFormatedData = m_strXScaleFormatedData + szXScaleOriginalData.GetAt(nIdx);
			
			// 6.2.2 szMarkTime�� ��� parsing�ϰ� ���� ���������� �����ڸ� �ٿ��� �� ��츦 Ȯ���ϰ� �ٿ��ش�.
			CString strComp;
			if( nPacketTypeLength == ( nIdx + 1 ) )
			{
				if( strChar == 'Y' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_YEAR);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharDateCompartment && bIsMonth )
				{
					strComp.LoadString( IDS_MONTH);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'D' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_DAY);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'H' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_HOUR);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharTimeCompartment && !bIsMonth)
				{
					strComp.LoadString( IDS_MINUTE);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'S' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_SECOND);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
			}
		}
	}

	return TRUE;
}
