// MultiChartDataViewAddInImp.cpp: implementation of the CMultiChartDataViewAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "MultiChartDataViewAddInImp.h"

#include <math.h>											// for HUGE_VAL
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/Conversion.h"					// for CDataConversion
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IStringList.h"			// for IStringList
#include "../Include_AddIn/I000000/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IPacketListManager.h"	// for IIPacketListManager

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiChartDataViewAddInImp::CMultiChartDataViewAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{

}

CMultiChartDataViewAddInImp::~CMultiChartDataViewAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CMultiChartDataViewAddInImp::m_strAddInItemName = "MCHART_DVIEW";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CMultiChartDataViewAddInImp)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnManualRealData)
	ONADDINEVENT( OnUserIndiatorConditionLoaded)
	ONADDINEVENT( OnMouseMove)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// Override
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
BOOL CMultiChartDataViewAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	if( 0 == p_nCommandType)
	{
		// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
		ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
		m_OutsideCommonInterfaceFunction.SetOutsideCommonInterface( (COutsideCommonInterface *)p_lData, CString( szDataPath));
	}		
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
//////////////////////////////////////////////////////////////////////

// RQ�� Packet : �������� - ojtaso (20070111)
void CMultiChartDataViewAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2008/5/13 - Seung-Won, Bae) Refresh with Mouse Move Message
	Refresh();
}

// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
void CMultiChartDataViewAddInImp::OnUserIndiatorConditionLoaded( void)
{
	// (2008/5/13 - Seung-Won, Bae) Refresh with Mouse Move Message
	Refresh();
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CMultiChartDataViewAddInImp::OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption)
{
	// (2008/5/13 - Seung-Won, Bae) Refresh with Mouse Move Message
	Refresh();
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CMultiChartDataViewAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	// (2008/5/13 - Seung-Won, Bae) Refresh with Mouse Move Message
	Refresh();
}


// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
BOOL CMultiChartDataViewAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// (2008/5/13 - Seung-Won, Bae) Do not call MouseMove directly in refresh time. Indicator is not calculated yet.
	CPoint ptCursor( point);
	if( ptCursor.x < 0 || ptCursor.y < 0)
	{
		CRect rctOCX = m_pIChartOCX->GetOcxRegion();
		CPoint ptCenter = rctOCX.CenterPoint();
		if( ptCursor.x < 0) ptCursor.x = ptCenter.x;
		if( ptCursor.y < 0) ptCursor.y = ptCenter.y;
	}

	// ����(05/06/28) ��Ƽ��ƮDLL�� ���콺Move���� Point���� �����Ѵ�.
	// ��Ƽ��ƮDLL���� DataListViewer�� ���� ������ ���콺�� ��ġ�� ���� ����Ÿ��
	// ��Ŀ���ϱ� ���� 1�� �ܰ��̴�. 2���ܰ�� ��Ƽ��ƮDLL���� DataListViewer�� ���� ������
	// GetDataIndexInViewer()�޼ҵ带 ȣ���Ͽ� Data Index�� ���ؼ� �ش絥��Ÿ�� ��Ŀ���Ѵ�.
	SetDataWindowStart( ptCursor);
	// if(m_pOutsideCommonInterface)
	//  m_pOutsideCommonInterface->MoveMouse(point);
	return FALSE;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataWindowStart
 - Created at  :  2005-07-22   09:21
 - Author      :  ������
 - Description :  ��Ƽ��Ʈ ����Ÿâ�� ��ȸ����Ÿ ������ ������.
 - Update	   :  (05/09/08) ��Ʈ����̿��� �������γ�, Ư����ǥ��ȸ�ÿ��� ��ȸ����Ÿ�� ������ �ʴ´�.
 -----------------------------------------------------------------------------------------*/
void CMultiChartDataViewAddInImp::SetDataWindowStart(CPoint point)
{
	// NOTE: ����(05/07/22) ���߿� �� �߰���ų��.
	if(!m_OutsideCommonInterfaceFunction.MChart_IsActiveDataWindow())
		return;

	m_listDataWindow.RemoveAll();

	// Ư����Ʈ(�Ｑ��ȯ��, P&F, ���ð�)�� �ڷ����ڿʹ� ������ ��ǥ���̴�.
	// �׷��Ƿ� ��Ƽ��Ʈ ����Ÿâ ����� ���ǹ��ϴ�.
	ILPCSTR szGraphName = m_pIChartManager->GetAllIndicatorNameString();
	CString strGraphName( szGraphName);
	if( g_iMetaTable.IsSpecialGroupGraph(strGraphName.Left(strGraphName.Find(_T(","))))) return;

	// �ش翵�� ��Ͽ����� �׷��������� ������ ����.
	int nR, nC;
	if( !m_pIChartManager->GetBlockIndex( point, nR, nC)) return;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock) return;
	RECT rctNoFrame, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctNoFrame, rctNoFrameAndMargin);
	pIBlock->Release();
	if( CRect( rctNoFrame) == CRect( 0, 0, 0, 0)) return;

	ILPCSTR strRQ = m_pIChartOCX->GetCurrentRQ();
	SetDataWindowHeader(strRQ, point);
	SetDataWindowData(strRQ, point);

	// MultiChart�� ����Ÿâ�� ������.
	m_OutsideCommonInterfaceFunction.MChart_SetDataWindowData(&m_listDataWindow);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataWindowHeader
 - Created at  :  2005-07-21   15:12
 - Author      :  ������
 - Description :  ��Ƽ��Ʈ�� ����Ÿâ�� �ǳ��� ��ȸ����Ÿ�� Header�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CMultiChartDataViewAddInImp::SetDataWindowHeader(const char *szRQ, CPoint point)
{
	CString strHeader = _T("");
	
	int nColumnIndex;	// Block�� Į���ε���
	if(point.x == -1 && point.y == -1)
		nColumnIndex = 0;
	else
	{
		int nR;
		if( !m_pIChartManager->GetBlockIndex( point, nR, nColumnIndex)) return;
		if( nColumnIndex == -1) return;
	}

	BOOL bStart = TRUE;
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	CString strRQ( szRQ);
	if( strRQ.Right( 5) == "_PAIR") strRQ = strRQ.Left( strRQ.GetLength() - 5);
	for( int i = 0; i < 2; i++)
	{
		ILPCSTR szTitle = NULL;
		ILPCSTR szPacketNames = NULL;
		ILPCSTR szRQ = NULL;
		POSITION psnSGVD =  m_pIChartManager->GetSubGraphViewData_First( strRQ);
		ML_SET_LANGUAGE_RES();
		
		while( psnSGVD)
		{
			if( bStart)
			{
				// ù��° ���: �ڷ�����
				CString strHorzTitle;
				ILPCSTR szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnSGVD);
				IPacket *pIPacket = m_pIPacketListManager->GetPacket( strRQ, szHorzScaleName);
				if( pIPacket == NULL) continue;

				ILPCSTR szType = pIPacket->GetType();

				// 2011.01.25 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
				// ��Ʈ ȭ���ϴ� �ڷ���ȸ�� ��¥ ó��...
				CString strType( szType);
				CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pIPacket->GetDateUnitType();
				if (strType == _T("YYYYMMDDHHMMSS"))
				{
					int nHourType = 0;
					if( CScaleBaseData::HORZ_TIME_ONE	== eDateUnit || 
						CScaleBaseData::HORZ_TIME_TWO	== eDateUnit ||
						CScaleBaseData::HORZ_TICK		== eDateUnit ||
						CScaleBaseData::HORZ_TIME_SECOND== eDateUnit)
					{
						if (strType.Find("HH") >= 0)
							nHourType = 1;
					}
					else
					{
						nHourType = 0;
					}

					if (nHourType == 1)
						strHorzTitle.LoadString( IDS_TIME);
					else							
						strHorzTitle.LoadString( IDS_DATE);
				}
				else
				{
					if( strstr( szType, _T("HH")))	
						strHorzTitle.LoadString( IDS_TIME);
					else							
						strHorzTitle.LoadString( IDS_DATE);
				}
				// 2011.01.25 by SYS <<

				pIPacket->Release();

				//------------ADD
				strHeader = strHeader + strHorzTitle + _T("|");

				bStart = FALSE;
			}

			m_pIChartManager->GetSubGraphViewData_TitleAndPacketNameAndRQString( psnSGVD, szTitle, szPacketNames, szRQ);

			CString strRealPacketNames = GetDataWindowRealPacketNames( szTitle, szPacketNames);
			
			if(1 == CDataConversion::GetStringDataCount(strRealPacketNames, _T(",")) && !szTitle.IsEmpty())
			{
				// (2008/11/10 - Seung-Won, Bae) Support Sell/Buy Item Title.
				CString strNewTitle;
				if( !strcmp( szTitle, _MTEXT( C3_PRICE)))
				{
					LPCTSTR szItemCode = NULL, szItemName = NULL;
					m_pIPacketListManager->GetItemCode( strRQ, szItemCode, szItemName);
					if( szItemCode) if( *szItemCode == '|')
					{
						if( szItemCode[ 1] == 'B') strNewTitle.LoadString( IDS_BUY);
						if( szItemCode[ 1] == 'S') strNewTitle.LoadString( IDS_SELL);
					}
				}
				if( strNewTitle.IsEmpty())
				{
					strNewTitle = _MTOL1_N( szTitle);
					if( strNewTitle.IsEmpty()) strNewTitle = _MTOL3( szTitle);
				}
				strHeader = strHeader + strNewTitle + _T("|");
			}
			else
			{
				// ���Ź� ��Ʈ�� Skip!!
				if( strRealPacketNames.Find( _MTEXT( C2_OVERHANGING_SUPPLY)) < 0) while(strRealPacketNames.IsEmpty() == false)
				{
					CString strName = CDataConversion::GetStringData(strRealPacketNames, _T(","));
					// (2008/3/21 - Seung-Won, Bae) Check Dynamic Packet Name
					// 1. Rainbow Chart
					int nPrefixLen = 0;
					if( !strncmp( strName, _MTEXT( C0_RAINBOW_PACKET), nPrefixLen = strlen( _MTEXT( C0_RAINBOW_PACKET))))
						strName = _LTEXT0( C0_RAINBOW_PACKET) + strName.Mid( nPrefixLen);
					else
					// 2. Box Chart
					if( !strncmp( strName, _MTEXT( C2_BOX_CHART), nPrefixLen = strlen( _MTEXT( C2_BOX_CHART))))
							strName = CString( _LTEXT2( C2_BOX_CHART)) + "_" + _MTOL0( strName.Mid( nPrefixLen));
					else
					// 3. Others
						strName = _MTOL0( strName);

					strHeader = strHeader + strName + _T("|");
				}
			}

			m_pIChartManager->GetSubGraphViewData_Next( psnSGVD);
		}
		if( strRQ.Right( 5) == "_PAIR") strRQ = strRQ.Left( strRQ.GetLength() - 5);
		else							strRQ = strRQ + "_PAIR";
	}

	m_listDataWindow.AddTail(strHeader);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataWindowData
 - Created at  :  2005-07-21   15:12
 - Author      :  ������
 - Parameters  :  point - ���콺�� ��ġ.
 - Return Value:  None
 - Description :  ��Ƽ��Ʈ�� ����Ÿâ�� �ǳ��� ��ȸ����Ÿ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CMultiChartDataViewAddInImp::SetDataWindowData(const char *szRQ, CPoint point)
{
	int nBlockColumnIndex;	// ���콺�� ��ġ�Ѱ��� ���Į���ε���.
	if(point.x == -1 && point.y == -1)
	{
		nBlockColumnIndex = 0;
	}
	else
	{
		int nR;
		if( !m_pIChartManager->GetBlockIndex( point, nR, nBlockColumnIndex)) return;
		if( nBlockColumnIndex == -1) return;
	}

	int nHighlightIndex;
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	CString strRQ( szRQ);
	if( strRQ.Right( 5) == "_PAIR") strRQ = strRQ.Left( strRQ.GetLength() - 5);
	CStringArray saViewData;
	saViewData.SetSize( 5);
	for( int j = 0; j < 2; j++)
	{
		//{{ 2007.03.26 by LYH XScale ����
		//int nDataIndex = GetViewDataIndex(point);
		int nDataIndex = m_pIChartManager->FindViewDataIndex_Pt( strRQ, point.x);
		//}}
		
		// ListCtrl�� ǥ���� ����Ÿ�� ���ε����� ���ؿ´�.
		int nDataEndIndex = GetDataWindowEndIndex( nDataIndex, nHighlightIndex);

		ILPCSTR szTitle = NULL;
		ILPCSTR szPacketNames = NULL;
		ILPCSTR szRQ = NULL;
		ILPCSTR szHorzScaleName = NULL;
		CString strViewData;
		POSITION psnSGVD =  m_pIChartManager->GetSubGraphViewData_First(strRQ);
		for(int i = 0; i < 5; i++)	// 5��.
		{
			if( j == 0) saViewData[ i] = _T("");
			
			// ���ν����ϵ���Ÿ ���Ͽ� ListCtrl�� �����Ѵ�.
			if( !m_pIChartManager->GetSubGraphViewData_Index( psnSGVD, 0, nBlockColumnIndex)) continue;

			if( j == 0)
			{
				szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnSGVD);
				SetDataWindowScaleData( strRQ, szHorzScaleName, nDataEndIndex, saViewData[ i]);
			}

			while( psnSGVD)
			{
				m_pIChartManager->GetSubGraphViewData_TitleAndPacketNameAndRQString( psnSGVD, szTitle, szPacketNames, szRQ);
				// ���Ź� ��Ʈ�� Skip!!
				if( !strstr( szPacketNames, _MTEXT( C2_OVERHANGING_SUPPLY)))
					SetDataWindowViewData( strRQ, szTitle, szPacketNames, nDataEndIndex, saViewData[ i]);
				if( !m_pIChartManager->GetSubGraphViewData_NextInColumn( psnSGVD, FALSE)) break;
			}//end of for(j		

			nDataEndIndex--;

		}//end of for(int i = 0...
		m_pIChartManager->GetSubGraphViewData_Close( psnSGVD);
		
		if( strRQ.Right( 5) == "_PAIR") strRQ = strRQ.Left( strRQ.GetLength() - 5);
		else							strRQ = strRQ + "_PAIR";
	}
	m_listDataWindow.AddTail( saViewData[ 0]);
	m_listDataWindow.AddTail( saViewData[ 1]);
	m_listDataWindow.AddTail( saViewData[ 2]);
	m_listDataWindow.AddTail( saViewData[ 3]);
	m_listDataWindow.AddTail( saViewData[ 4]);

	CString strHighlightIndex;
	strHighlightIndex.Format("%d", nHighlightIndex);
	m_listDataWindow.AddTail(strHighlightIndex);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataWindowViewData
 - Created at  :  2005-07-22   09:12
 - Author      :  ������
 - Description :  ��ȸ����Ÿ�� �����̳ʿ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CMultiChartDataViewAddInImp::SetDataWindowViewData(const char *szRQ, const char *szTitle, const char *szPacketNames, const int nDataIndex, CString& strViewData)
{
	CString strRealPacketNames = GetDataWindowRealPacketNames( szTitle, szPacketNames);

	CDataConversion dataConversion( m_hOcxWnd, m_eChartMode);
	int nPacketCount = dataConversion.GetStringDataCount(strRealPacketNames, _T(","));
	CString strData;
	if(nPacketCount == 1)
	{
		CString strPacketName = dataConversion.GetStringData(strRealPacketNames, _T(","));
		IPacket* pIPacket = m_pIPacketListManager->GetPacket(szRQ, strPacketName);
		if( !pIPacket) return;

		ILPCSTR szType = pIPacket->GetType();
		if( !strcmp( szType, _MTEXT( C6_CHARACTER)))
		{
			IStringList *pIStringList = pIPacket->GetStrDataList();
			if( pIStringList)
			{
				POSITION pos = pIStringList->FindIndex( nDataIndex);
				ILPCSTR szData = NULL;
				if( pos) pIStringList->GetNext( pos, szData);
				pIStringList->Release();
				strData = szData;
			}
		}
		else
		{
			IDoubleList *pIDoubleList = pIPacket->GetDataList();
			if( pIDoubleList)
			{
				POSITION pos = pIDoubleList->FindIndex( nDataIndex);
				double dData = HUGE_VAL;
				if( pos)
				{
					pIDoubleList->GetNext( pos, dData);
					ILPCSTR szType = pIPacket->GetType();
					CString strType( szType);

					// 20080910 JS.Kim		8����/32���� ó��
					/*
					int nLength = strType.GetLength();
					int nDecimalPt = strType.Find(".") + 1;
					int nDecimal = 0;
					if( nDecimalPt) nDecimal = nLength - nDecimalPt;

					strData = CDataConversion::DoubleToString(dData, nDecimal);
					*/

					strData = CDataConversion::DoubleToString(dData);

					// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
					if (strType == _T("YYYYMMDDHHMMSS"))
					{
						int nDataUnit = pIPacket->GetDateUnitType();
						strData = dataConversion.GetStringFormatTrans2( strData, nDataUnit);
					}
					else
						strData = dataConversion.GetStringFormatTrans( strData, strType);
					// 2011.01.20 by SYS <<
				}
				pIDoubleList->Release();
			}
		}
		pIPacket->Release();

		// ------- ADD
		strViewData = strViewData + strData + _T("|");
	}//end of if(nPacketCount...
	else
	{
		while(strRealPacketNames.IsEmpty() == false)
		{
			CString strPacketName = dataConversion.GetStringData(strRealPacketNames, _T(","));
			
			IPacket *pIPacket = m_pIPacketListManager->GetPacket(szRQ, strPacketName);
			if( !pIPacket) continue;
			
			ILPCSTR szType = pIPacket->GetType();
			if( !strcmp( szType, _MTEXT( C6_CHARACTER)))
			{
				IStringList *pIStringList = pIPacket->GetStrDataList();
				if( pIStringList)
				{
					POSITION pos = pIStringList->FindIndex( nDataIndex);
					ILPCSTR szData = NULL;
					if( pos) pIStringList->GetNext( pos, szData);
					pIStringList->Release();
					strData = szData;
				}
			}
			else
			{
				IDoubleList *pIDoubleList = pIPacket->GetDataList();
				if( pIDoubleList)
				{
					POSITION pos = pIDoubleList->FindIndex( nDataIndex);
					double dData = HUGE_VAL;
					if( pos)
					{
						pIDoubleList->GetNext( pos, dData);

						ILPCSTR szType = pIPacket->GetType();
						CString strType( szType);
						// 20080910 JS.Kim		8����/32���� ó��
						/*
						int nLength = strType.GetLength();
						int nDecimalPt = strType.Find(".") + 1;
						int nDecimal = 0;
						if( nDecimalPt) nDecimal = nLength - nDecimalPt;

						strData = CDataConversion::DoubleToString(dData, nDecimal);
						*/
						strData = CDataConversion::DoubleToString(dData);

						// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
						if (strType == _T("YYYYMMDDHHMMSS"))
						{
							int nDataUnit = pIPacket->GetDateUnitType();
							strData = dataConversion.GetStringFormatTrans2( strData, nDataUnit);
						}
						else
							strData = dataConversion.GetStringFormatTrans(strData, strType);
						// 2011.01.20 by SYS <<

					}
					pIDoubleList->Release();
				}
			}
			pIPacket->Release();
			// ------ ADD
			strViewData = strViewData + strData + _T("|");
		}//end of while(
	}//end of else
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetDataWindowRealPacketNames
 - Created at  :  2005-07-21   17:19
 - Author      :  ������
 - Description :  1ƽ�ϰ��� �˻��Ͽ� ���ο� ��Ŷ���� �������ش�.
 -----------------------------------------------------------------------------------------*/
CString CMultiChartDataViewAddInImp::GetDataWindowRealPacketNames(const char *szTitle, const char *szPacketNames) const
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

/*-----------------------------------------------------------------------------------------
 - Function    :  GetViewDataIndex
 - Created at  :  2005-07-07   08:51
 - Author      :  ������
 - Description :  ���콺�� ��ġ�� �ش��ϴ� Data�� Index�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
int CMultiChartDataViewAddInImp::GetViewDataIndex(CPoint ptMouse) 
{
	// ���콺 ��ġ�Ѱ��� Block Index�� �����´�.
	int nBlockColumnIndex;
	int nBlockRowIndex;
	// CPoint(-1, -1)�̸� �ǳ� �ε����� �����Ѵ�.
	if( ptMouse.x == -1 && ptMouse.y == -1) return GetViewEndDataIndex(true);
	else
	{
		if( !m_pIChartManager->GetBlockIndex( ptMouse, nBlockRowIndex, nBlockColumnIndex)) return -1;
		if( nBlockRowIndex == -1 || nBlockColumnIndex == -1) return -1;
	}

	IBlock *pIBlock = m_pIChartManager->GetBlock( nBlockRowIndex, nBlockColumnIndex);
	if( !pIBlock) return -1;
	CRect rctGraphRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;
	if( !rctGraphRegion.PtInRect( ptMouse)) return -1;
	// 1.2 ���� ȭ�鿡 ���̴� Data�� Start/End Index�� ���Ѵ�.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return FALSE;
	// 1.3 ������ Mouse Point�� Data Index�� ���Ѵ�.
	return nStartIndex + ( ( ptMouse.x - rctGraphRegion.left) * ( nEndIndex - nStartIndex + 1)) / rctGraphRegion.Width();
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetDataWindowEndIndex
 - Created at  :  2005-07-21   18:06
 - Author      :  ������
 - Parameters  :  nIndex - ���콺�� ��ġ�Ѱ��� ����Ÿ �ε���
 - Return Value:  int - ListCtrl�� ǥ���� ����Ÿ�� ù��°�� �� ����Ÿ�� �ε���
 - Description :  ListCtrl�� ǥ���� ����Ÿ�� ù��° �ε����� ���Ѵ�.
 - Update	   :  (08/23) ��Ƽ��ƮDLL�� DataList���� Highlight�� �ε����� ���Ѵ�.
 -----------------------------------------------------------------------------------------*/
int CMultiChartDataViewAddInImp::GetDataWindowEndIndex(const int nDataIndex, int& nHighlightIndex)
{
	// �պκ� ��ġ���� ��� ó��
	if(nDataIndex - 2 < 0)
	{
		if(nDataIndex == 0)
			nHighlightIndex = 5;
		else if(nDataIndex == 1)
			nHighlightIndex = 4;
		else
			nHighlightIndex = 3;

		return 4;
	}

	// ���κ� ��ġ���� ��� ó��
	if(nDataIndex + 2 > GetViewEndDataIndex(true))
	{
		if(nDataIndex == GetViewEndDataIndex(true))
			nHighlightIndex = 1;
		else if(nDataIndex == GetViewEndDataIndex(true) - 1)
			nHighlightIndex = 2;
		else
			nHighlightIndex = 3;

		return GetViewEndDataIndex(true);
	}

	nHighlightIndex = 3;
	return nDataIndex + 2;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataWindowScaleData
 - Created at  :  2005-07-22   09:11
 - Author      :  ������
 - Description :  �ڷ����� ����Ÿ�� �����̳ʿ� ��´�.
 -----------------------------------------------------------------------------------------*/
void CMultiChartDataViewAddInImp::SetDataWindowScaleData(const char *szRQ, const char *szHorzScaleName, const int nDataIndex, CString& strViewData)
{
	IPacket* pIPacket = m_pIPacketListManager->GetPacket( szRQ, szHorzScaleName);
	if( !pIPacket) return;

	CString strScaleData = _T("");	// Scale Data(string type)
	double dScaleData = 0.0;		// Scale Data(double type)
	ILPCSTR szType = pIPacket->GetType();

	// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	int nDateUnit = pIPacket->GetDateUnitType();
	// 2011.01.20 by SYS <<

	if( !strcmp( szType, _MTEXT( C6_CHARACTER)))
	{
		IStringList *pIStringList = pIPacket->GetStrDataList();
		if( pIStringList)
		{
			POSITION pos = pIStringList->FindIndex( nDataIndex);
			ILPCSTR szData = NULL;
			if( pos) pIStringList->GetNext( pos, szData);
			pIStringList->Release();
			strScaleData = szData;
		}
	}
	else
	{
		double dScaleData = HUGE_VAL;
		IDoubleList *pIDoubleList = pIPacket->GetDataList();
		if( pIDoubleList)
		{
			POSITION pos = pIDoubleList->FindIndex( nDataIndex);
			if( pos) pIDoubleList->GetNext( pos, dScaleData);
			else	 pIPacket->GetDataPastandFuture( nDataIndex, dScaleData);
			pIDoubleList->Release();
		}
		if( strcmp( szHorzScaleName, _MTEXT( C0_DATE_TIME))) 
			strScaleData = CDataConversion::DoubleToString(dScaleData);
		else
		{
			CString strType( szType);

			// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
			CString strData = CDataConversion::DoubleToString(dScaleData);
			
			if (strType == _T("YYYYMMDDHHMMSS"))
			{
				CString strFormat = _T("YYYYMMDD");
				switch(nDateUnit)
				{
				case 1:	// ��
				case 2:	// �� 
					strFormat = _T("YYYYMMDD");		
					break;
				case 3:	// �� 
					strFormat = _T("YYYYMM");		
					break;
				case 4:	// ��
					strFormat = _T("MMDDHHMM");
					break;
				case 5:	// ƽ 
					strFormat = _T("DDHHMMSS");
					break;
				}
				//strScaleData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans2( strData, nDateUnit, _T(""));
				strScaleData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strData, strFormat, _T(""));
			}
			else
			{
				strScaleData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strData, strType, _T(""));
			}
			// 2011.01.06 by SYS <<
		}
	}
	pIPacket->Release();

	// -------- ADD
	strViewData = strViewData + strScaleData + _T("|");
	return;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetViewEndDataIndex
 - Created at  :  2005-07-05   14:56
 - Author      :  ������
 - Parameters  :  bData - true�϶��� �ǵ���Ÿ�� �ε�����, false�϶��� �ǵ���Ÿ�����ϴ� ������ �ε�����..
 - Return Value:  int - ������ ����Ÿ�� �ε����� ����.
 - Description :  ���� ��Ʈ�� ���̴� ������ ����Ÿ(�Ǵ� ����)�� �ش��ϴ� �ε����� �����Ѵ�.
 - Note		   :  bData��� �Ķ���ʹ� ������Ʈ���� ������ ���� ��� ����������Ÿ�ε�����
				  ����Ÿ������ �����Ұ����� ������� �����Ұ������� �����ϴ� ���̴�.
 -----------------------------------------------------------------------------------------*/
int CMultiChartDataViewAddInImp::GetViewEndDataIndex(const bool bData)
{
	if(bData)
	{
		return m_pIDefaultPacketManager->GetMaximumDataCount() - 1;	// Zero Base.
	}
	else
	{	// NOTE: �ϴ� -1�� �����ϰ� ���Ŀ� ��������!
		return -1;
	}
}

void CMultiChartDataViewAddInImp::Refresh( void)
{
	// (2004.11.05, ��¿�) TR�̳� Real ó������ ToolTip�� ���Ͽ� Mouse ��ġ�� ������� �ʰ�,
	//		�׶��׶� Mouse ��ġ�� �ľ��Ͽ� ó���ϴ� ������ �����Ѵ�.
	CPoint ptCursor;
	GetCursorPos( &ptCursor);
	::ScreenToClient( m_pIChartOCX->GetOcxHwnd(), &ptCursor);
		
	// (2008/5/13 - Seung-Won, Bae) Do not call MouseMove directly in refresh time. Indicator is not calculated yet.
	::PostMessage( m_pIChartOCX->GetOcxHwnd(), WM_MOUSEMOVE, 0, MAKELONG( ptCursor.x, ptCursor.y));
}
