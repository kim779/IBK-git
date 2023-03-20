// NumericalInquiryDataQueryer.cpp: implementation of the CNumericalInquiryDataQueryer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NumericalInquiryDataQueryer.h"

#include "../Include_Chart/Conversion.h"								// for CDataConversion
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"		// for IPacketListManager
#include "../Include_AddIn_133101/I133101/_IStringList.h"				// for IStringList

#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumericalInquiryDataQueryer::CNumericalInquiryDataQueryer( IChartOCX *p_pIChartOCX)
{
	// (2006/6/24 - Seung-Won, Bae) Manage the OCX Interfaces
	m_pIChartOCX				= p_pIChartOCX;
	m_pIChartManager			= NULL;
	m_pIDefaultPacketManager	= NULL;
	m_pIPacketListManager		= NULL;
	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		m_pIChartManager			= p_pIChartOCX->GetIChartManager();
		m_pIDefaultPacketManager	= p_pIChartOCX->GetIDefaultPacketManager();
		m_pIPacketListManager		= p_pIChartOCX->GetIPacketListManager();
	}

	// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in DongBu
	m_bWithHorizontalLine = TRUE;
	m_hOcxWnd = ( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);

	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = ( CChartInfo::CHART_MODE)m_pIChartOCX->GetChartMode();
}

CNumericalInquiryDataQueryer::~CNumericalInquiryDataQueryer()
{
	// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
	ClearInquiryDataList();
	
	// (2006/6/24 - Seung-Won, Bae) Manage the OCX Interfaces
	if( m_pIChartOCX)				m_pIChartOCX->Release();
	if( m_pIChartManager)			m_pIChartManager->Release();
	if( m_pIDefaultPacketManager)	m_pIDefaultPacketManager->Release();
	if( m_pIPacketListManager)		m_pIPacketListManager->Release();
}


//////////////////////////////////////////////////////////////////////
// (2006/6/24 - Seung-Won, Bae) Overations
//////////////////////////////////////////////////////////////////////
// XScale Text ����ǥ�� - ojtaso (20080429)
BOOL CNumericalInquiryDataQueryer::ParsingInquiryHorzScaleTextData(CString szRQ, CPoint point, CString& strScaleText)
{
	CInquiryItem *plistData = NULL;
	if(!szRQ.IsEmpty())
	{
		POSITION psnViewData = NULL;
		psnViewData = m_pIChartManager->GetSubGraphViewData_First(szRQ);
		if( !psnViewData) return FALSE;

		ILPCSTR szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnViewData);
		CString strHorzScaleName( szHorzScaleName);

		IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strHorzScaleName);
		if( pPacket == NULL) return FALSE;

		int nDataIndex = -1;

		POSITION pos = NULL;
		double dData = 0;

		if( m_pIPacketListManager->IsBasePacketString())
		{
			nDataIndex = FindInquiryDataIndex(point, psnViewData);

			if( nDataIndex < 0)
			{
				m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
				return FALSE;
			}

			ILPCSTR szData = NULL;
			pPacket->GetData( nDataIndex, szData);
			if( !szData.IsEmpty()) strScaleText = szData;
		}
		else
		{
			nDataIndex = m_pIChartManager->FindViewDataIndex_Pt(szRQ, point.x);

			if( nDataIndex < 0)
			{
				m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
				return FALSE;
			}
			
			dData = 0.0;
			if( !pPacket->GetData( nDataIndex, dData))
				pPacket->GetDataPastandFuture( nDataIndex, dData);

			if(dData == 0.0)
			{
				pPacket->Release();
				return FALSE;
			}

			// �ڷ����� �ߺ� ǥ�� ���� - ojtaso (20080125)
			if(strHorzScaleName.Find("�ڷ�����") < 0)
				strScaleText = CDataConversion::DoubleToString(dData);
			else 
			{
				// ���콺��ġ�Ѱ��� Block Index�� �����´�.
				int nRow, nColumn;
				if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn))
				{
					m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
					return FALSE;
				}

				// ��ġ��ȸâ�� �ڷ����� Format, X Scale �������� ���� ó�� - ojtaso (20070827)
				ILPCSTR szDateCompart, szTimeCompart;
				m_pIChartManager->GetHorzScaleCompart( nRow, nColumn, szDateCompart, szTimeCompart);
				CString strCompartDate( szDateCompart);
				CString strCompartTime( szTimeCompart);

				ILPCSTR szPacketType = pPacket->GetType();
				CString strPacketType( szPacketType);
				strScaleText = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString((int)dData), strPacketType, strCompartDate, strCompartTime);
			}

		}
	
		pPacket->Release();
	}

	return TRUE;
}

// 1. Get the Data
/*-------------------------------------------------------------------------------
 - Function    :  ParsingInquiryData
 - Created at  :  2005-03-22   09:23
 - Author      :  ������
 - Parameters  :  point - �ش� ���콺 ������.
				  bToolTip - �������� ����Ÿ�� ��ȸ�� ���.(Data��ȸ���͸��Լ��� �پ�Ѿ�� �Ѵ�.)
 - Return Value:  BOOL - �Ľ̼���, FALSE - ����.
 - Description :  point�� ��ġ�� ��ġ��ȸ����Ÿ�� ���Ͽ� list�����̳ʿ� ��� �����Ѵ�.
				  �� _LISTInquiryData ��ġ��ȸ����Ÿ'����Ʈ'�� ���� �����̳�.(h���� typedef ����)
				  �� pair<CString, CString>��  <Ÿ��Ʋ, ����Ÿ>�� ����.
				  �� list< pair<CStirng, CString> >�� <Ÿ��Ʋ, ����Ÿ>'����Ʈ'�� ���� (list�ϳ��� �ڷ����� �ϳ��� ��Ī��)
				  �� list< list<.......> >�� ��ġ��ȸ����Ÿ'����Ʈ'�� '����Ʈ'�ν�, �ش������� 5�ϰ��� ��ȸ����Ÿ list��.	(Multi Line Dlg��.)
 - Update	   :  (05/09/05) �ϳ��� ĵ��ȿ��� ���콺�� �����϶��� ��Ÿ�� ��ġ��ȸ����Ÿ�� ������ �ʿ䰡 ������
							����밪�� �������־�� �Ѵ�. �۾�ó����.
 -------------------------------------------------------------------------------*/
//<<[A00000441]ǳ������ ����� ��ü ��ȸ �ɼ� �߰�
//BOOL CNumericalInquiryDataQueryer::ParsingInquiryData(CString szRQ, CPoint point, bool bToolTip, BOOL bPairTick)
BOOL CNumericalInquiryDataQueryer::ParsingInquiryData(CString szRQ, CPoint point, bool bToolTip/*=FALSE*/, BOOL bPairTick/*=FALSE*/, BOOL bViewAll/*=TRUE*/)
//>>
{
	m_nMaxDecimal = 0;	// ��ġ��ȸ����Ÿ �Ҽ��� �ִ� �ڸ��� �ʱ�ȭ

	// ����.
	// 1. MainBlock���� ViewData�� ��������... �ű⼭ MousePoint�� ��ġ�� ����
	// 2. RowViewData�� �����Ѵ�.
	// 3. MousePoint�� ��ġ�Ѱ��� ����Ÿ�ε����� ���Ѵ�.
	// 4. ����Ÿ�ε����� ������ ��ȸ�� ����Ÿ�ε����� ���� ���, ���� �ٽ� ��ȸ�ؿ� �ʿ䰡 ����.
	// 5. 1�������� '�ڷ�����'�� ����Ÿ���� �����ؼ� Container�� �����Ѵ�.
	// 6. �׸��� �ݺ��������� RowViewData���� SubGraphViewData�� �����ͼ� Container�� ����.
	// 7. ���������� '������Ʈ'�뿡 MousePoint�� ��ġ�������� ����밪�� ���ؼ��ͼ� Container�� ����.

	CInquiryItem *plistData = NULL;
	CInquiryItem *plistPrevData = NULL;
	if(!szRQ.IsEmpty())
	{
		// ViewData�� �����´�. (���⼭ ��Ŷ���� ���´�.)
		POSITION psnViewData = NULL;
		psnViewData = m_pIChartManager->GetSubGraphViewData_First(szRQ);
		if( !psnViewData) return FALSE;

		// ���콺��ġ�Ѱ��� Block Index�� �����´�.
		int nRow, nColumn;
		if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// ����Ÿ�ε��� ���ؿ���.
		// �ε��� ���ؿ��⿡ �ʿ��� RowViewData�� �����´�.
	/*	if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, nRow, nColumn)) return FALSE;*/
		if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, 0, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		ILPCSTR szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnViewData);
		CString strHorzScaleName( szHorzScaleName);

		// �ε��� ����.	
		// XScale : ������ ����/�� �ε��� ���ϱ� - vntsorl(20070327)
	//	int nDataIndex = FindInquiryDataIndex(point, psnViewData);
		int nDataIndex = 0;
		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		BOOL bInverseLine = ( strHorzScaleName == _MTEXT( C0_INVERSETIMELINE_VOLUME));
		if( bInverseLine) nDataIndex = m_pIChartManager->GetDataIndexOfInverseLine( point);
		// ������ ����Ÿ���� : xScaleManager - ojtaso (20070703)
		else if( m_pIPacketListManager->IsBasePacketString())
			nDataIndex = FindInquiryDataIndex(point, psnViewData);
		else nDataIndex = m_pIChartManager->FindViewDataIndex_Pt(szRQ, point.x);

		if( nDataIndex < 0)
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// ����(05/09/05) MouseMove������ �ϳ��� ĵ����̶��ص� ����밪�� ���ؾ��Ѵ�.
		// ���Ӱ� ����Ÿ�� �־�� �ϴϱ� �����̳ʸ� �ϴ� ����.
		// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		if( !bPairTick)
		{
			ClearInquiryDataList();
			plistData = new CInquiryItem;
			plistPrevData = new CInquiryItem;
		}
		else
		{
			if( m_listInquiryData.IsEmpty()) m_listInquiryData.AddTail( new CInquiryItem);
			if( m_listPrevInquiryData.IsEmpty()) m_listPrevInquiryData.AddTail( new CInquiryItem);
			plistData = m_listInquiryData.GetHead();
			plistPrevData = m_listPrevInquiryData.GetHead();

			// (2008/11/9 - Seung-Won, Bae) Delete Horizontal Value.
			if( !plistData->IsEmpty()) delete plistData->RemoveTail();	// �����
			if( !plistData->IsEmpty()) delete plistData->RemoveTail();	// ����
		}
		
		// ���������� ����Ÿ�� �Ľ��Ͽ� Container�� �����Ѵ�.
		BOOL bHorzScaleData = FALSE;
		bool bScaleData_Date = false;
		if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, 0, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		if( !bInverseLine)
		{
			// ��ġ��ȸâ�� �ڷ����� Format, X Scale �������� ���� ó�� - ojtaso (20070827)
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if( !bPairTick)	bHorzScaleData = SetHorzScaleDataToContainer(szRQ, strHorzScaleName, nDataIndex, plistData, nRow, nColumn);
			else			bHorzScaleData = TRUE;
			SetHorzScaleDataToContainer(szRQ, strHorzScaleName, max(0,nDataIndex-1), plistPrevData, nRow, nColumn);
			// �����ϵ���Ÿ�� '�ڷ�����'�� ���� 1��¥�� ��ġ��ȸâ������ ���͸��� �Ѵ�.
			if( strHorzScaleName == _MTEXT( C0_DATE_TIME)) bScaleData_Date = true;
		}

		int nNextPacketIndex = -1;
		CString strTitle, strPacketNames;
		bool bSetTitle = false;

		// (2008/10/7 - Seung-Won, Bae) Show Info for only One Block.
		CRect rctGraphRegion;
		IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
		if( pIBlock)
		{
			CRect rctNoFrameAndMargin;
			pIBlock->GetGraphRegions( rctGraphRegion, rctNoFrameAndMargin);
			pIBlock->Release();
		}

		BOOL bNoGraph = TRUE;
		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		if( bHorzScaleData || bInverseLine) while( psnViewData)
		{
			//>>[A00000441]ǳ������ ����� ��ü ��ȸ �ɼ� �߰�
			if( bViewAll == FALSE )
			{
				// (2008/10/7 - Seung-Won, Bae) Show Info for only One Block.
				CRect rctSubGraphRegion = m_pIChartManager->GetSubGraphViewData_GraphRegion( psnViewData);
				CRect rctUnionRegion;
				if( !rctUnionRegion.IntersectRect( rctGraphRegion, rctSubGraphRegion))
				{
					m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
					continue;
				}
			}	//<< 
			//<< 

			bNoGraph = FALSE;
			ILPCSTR szTitle, szPacketNames, szRQ;
			m_pIChartManager->GetSubGraphViewData_TitleAndPacketNameAndRQString( psnViewData, szTitle, szPacketNames, szRQ);
			strTitle = szTitle;
			strPacketNames = szPacketNames;

			// ����Ÿ�� �����̳ʿ� ����.
			strTitle.TrimRight(' ');
			if( strTitle.IsEmpty())
			{
				nNextPacketIndex = strPacketNames.Find( ',');
				if( nNextPacketIndex < 0)	strTitle = strPacketNames;
				else						strTitle = strPacketNames.Left( nNextPacketIndex);
			}

			if(bSetTitle == false)
			{
				SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);

				//110116alzioyes:���� �ŷ����� ����߰�.
				if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0 || strTitle == _MTEXT( C0_VOLUME))
					SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
				
				bSetTitle = true;
				m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
				continue;
			}
			SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);

			//110116alzioyes:���� �ŷ����� ����߰�.
			if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0 || strTitle == _MTEXT( C0_VOLUME))
				SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
			
			m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
		}
		bSetTitle = false;

		if( bNoGraph)
		{
			while( !m_listInquiryData.IsEmpty())
			{
				plistData = m_listInquiryData.RemoveHead();
				while( !plistData->IsEmpty()) delete plistData->RemoveHead();
				delete plistData;
				plistData = NULL;
			}
			return FALSE;
		}

		// ������Ʈ�� ���콺�����Ͱ� ��ġ�� ���� ���� '�����'���� �߰������ش�.
		// ��ġ��ȸâ�� �Ϲ�������Ÿ���϶��� �����Ѵ�.
		// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in DongBu
		if( m_bWithHorizontalLine && nDataIndex != -1)
		{ 
			ML_SET_LANGUAGE_RES();
			
			CString strHorzTitle;
			strHorzTitle.LoadString(IDS_HORIZONTALZONE);
			CString strHorzData = _T("");	// ����Ÿ

			int nR, nC, nV, nG;
			ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, szRQ);
			if( !szGraphName.IsEmpty() && nR == nRow && nC == nColumn)
			{
				IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
				if( pIBlock)
				{
					double dMin, dMax;
					if( pIBlock->GetVScaleGroupMinMax( nV, dMin, dMax))
					{
						POSITION psnViewData = m_pIChartManager->GetSubGraphViewData_First( NULL);
						if( psnViewData)
						{
							if( m_pIChartManager->GetSubGraphViewData_Index( psnViewData, nRow, nColumn))
							{
								CRect rectGraphRegion = m_pIChartManager->GetSubGraphViewData_GraphRegion( psnViewData);

								// (2008/2/26 - Seung-Won, Bae) Get invert type form Price Chart VScale
								BOOL bLog = pIBlock->IsLogScale();
								BOOL bReverse = pIBlock->IsVScaleInvert( nV);

								double dHorScale = m_pIChartOCX->ConvertYPositionToData( point.y, rectGraphRegion.top, rectGraphRegion.bottom, dMax, dMin, bLog, bReverse);

								IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, _MTEXT( C0_CLOSE));
								if( pPacket)
								{
									ILPCSTR szPacketType = pPacket->GetType();
									CString strPacketType( szPacketType);
									strHorzData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHorScale), strPacketType);
									pPacket->Release();
								}
								else
								{
									strHorzData = CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dHorScale));
								}

								if(strHorzTitle != _T("") && strHorzData != _T(""))
								{
									// ���� �ϳ� �ְ� ����� ������ �ִ´�.
									
									plistData->AddTail( new CRowData( "", ""));
									plistData->AddTail( new CRowData( strHorzTitle, strHorzData));
								}
							}

							m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
						}
					}
					pIBlock->Release();
				}
			}
		}//end of if(nDataIndex != -1)
	}
	else
	{
		// ��ü ViewData�� ������
		POSITION psnViewData = NULL;
		psnViewData = m_pIChartManager->GetSubGraphViewData_First("");
		if( !psnViewData) return FALSE;

		int nRow, nColumn;
		if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// ������ �ε����� ���� �� �ִ��� Ȯ��
		if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, 0, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		ILPCSTR szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnViewData);
		CString strHorzScaleName( szHorzScaleName);
		m_pIChartManager->GetSubGraphViewData_Close( psnViewData);

		// �����̳� �ʱ�ȭ
		// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
		ClearInquiryDataList();

		// ���������� ����Ÿ�� �Ľ��Ͽ� Container�� �����Ѵ�.
		BOOL bHorzScaleData = FALSE;
		bool bScaleData_Date = false;

		// �����̳� ����
		plistData = new CInquiryItem;
		plistPrevData = new CInquiryItem;

		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		int nDataIndex = 0;
		BOOL bInverseLine = ( strHorzScaleName == _MTEXT( C0_INVERSETIMELINE_VOLUME));
		// �����ϵ���Ÿ�� '�ڷ�����'�� ���� 1��¥�� ��ġ��ȸâ������ ���͸��� �Ѵ�.
		if( strHorzScaleName == _MTEXT( C0_DATE_TIME)) bScaleData_Date = true;

		CString strRQFromAr;
		LPCTSTR lpcszCode = NULL;
		LPCTSTR lpcszName = NULL;
		bool bSetTitle = false;
		// ù ������ ���
		int nNextPacketIndex = -1;
		CString strTitle, strPacketNames, strSubGraphRQ;

		IStringList *pislRQs = m_pIPacketListManager->GetAllRQ();
		if( pislRQs)
		{
			pislRQs->AddHead( "DEFAULT");

			ILPCSTR szRQ = NULL;
			POSITION psn = pislRQs->GetHeadPosition();
			while( psn)
			{
				//2007.07.05 by LYH �ڷ����� ����(ù��° RQ�� �ε����� ������ ���� ã��
				pislRQs->GetNext( psn, szRQ);
				strRQFromAr = szRQ;
				if( bInverseLine) nDataIndex = m_pIChartManager->GetDataIndexOfInverseLine( point);
				else nDataIndex = m_pIChartManager->FindViewDataIndex_Pt(strRQFromAr, point.x);
				if( nDataIndex < 0) continue;

				// ��ġ��ȸâ�� �ڷ����� Format, X Scale �������� ���� ó�� - ojtaso (20070827)
				bHorzScaleData = SetHorzScaleDataToContainer(strRQFromAr, strHorzScaleName, nDataIndex, plistData, nRow, nColumn);
				SetHorzScaleDataToContainer(strRQFromAr, strHorzScaleName, max(0,nDataIndex-1), plistPrevData, nRow, nColumn);
				break;
			}

			psn = pislRQs->GetHeadPosition();
			while( psn)
			{
				pislRQs->GetNext( psn, szRQ);
				strRQFromAr = szRQ;

				psnViewData = m_pIChartManager->GetSubGraphViewData_First("");
				m_pIPacketListManager->GetItemCode( strRQFromAr, lpcszCode, lpcszName);

				nNextPacketIndex = -1;
				if( bHorzScaleData || bInverseLine) while( psnViewData)
				{
					ILPCSTR szTitle, szPacketNames, szGraphRQ;
					m_pIChartManager->GetSubGraphViewData_TitleAndPacketNameAndRQString( psnViewData, szTitle, szPacketNames, szGraphRQ);
					strTitle = szTitle;
					strPacketNames = szPacketNames;
					strSubGraphRQ = szGraphRQ;

					// ���� RQ�� ���ؼ��� ���� �����͸� ������ ����
					if( strRQFromAr != szGraphRQ)
					{
						m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
						continue;
					}

					//2007.07.05 by LYH RQ���� ������ �ε��� ���� ����
					if( bInverseLine) nDataIndex = m_pIChartManager->GetDataIndexOfInverseLine( point);
					else nDataIndex = m_pIChartManager->FindViewDataIndex_Pt( strRQFromAr, point.x);
				
					if(nDataIndex>0)
					{
						// ����Ÿ�� �����̳ʿ� ����.
						strTitle.TrimRight(' ');
						if( strTitle.IsEmpty())
						{
							nNextPacketIndex = strPacketNames.Find( ',');
							if( nNextPacketIndex < 0)	strTitle = strPacketNames;
							else						strTitle = strPacketNames.Left( nNextPacketIndex);
						}

						if(bSetTitle == false)
						{
							SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);
							if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0)
								SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
							
							bSetTitle = true;
							m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
							continue;
						}
						SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);
						if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0)
							SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
					}
					m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
				}
				bSetTitle = false;
			}

			pislRQs->Release();
		}
	}
 
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( !bPairTick)
	{
		m_listInquiryData.AddTail( plistData);	
		m_listPrevInquiryData.AddTail( plistPrevData);
	}
	return TRUE;
}

CInquiryItem *CNumericalInquiryDataQueryer::GetHead( void)
{
	if( m_listInquiryData.IsEmpty()) return NULL;
	return m_listInquiryData.GetHead();
}

// ��ġ��ȸ, ������ ����� �߰� ǥ�� - ojtaso (20080710)
CInquiryItem *CNumericalInquiryDataQueryer::GetPrevHead( void)
{
	if( m_listPrevInquiryData.IsEmpty()) return NULL;
	return m_listPrevInquiryData.GetHead();
}

/*-------------------------------------------------------------------------------
 - Function    :  GetInquiryDataMaxLength
 - Created at  :  2005-03-31   09:50
 - Author      :  ������
 - Parameters  :  pListData               - [in] ��ġ��ȸ����Ÿ
 -                nFirstColumnMaxLength  - [out] ù��° Į���� �ִ����.
 -                nSecondColumnMaxLength - [out] �ι�° Į���� �ִ����.
 - Return Value:  bool - true: �Լ�ȣ�⼺��, false: �Լ�ȣ�����.
 - Description :  ��ġ��ȸ������ ���ڿ��� �°� �����ϱ� ���� �ִ� ���ڿ����̸� ���Ѵ�.
 -------------------------------------------------------------------------------*/
BOOL CNumericalInquiryDataQueryer::GetInquiryDataMaxLength( CInquiryItem *pListData, int& nFirstColumnMaxLength, int& nSecondColumnMaxLength)
{
	CRowData *pRowData = NULL;
	POSITION posRow = pListData->GetHeadPosition();
	while( posRow)
	{
		pRowData = pListData->GetNext( posRow);
		
		if(nFirstColumnMaxLength < pRowData->m_strName.GetLength())
			nFirstColumnMaxLength = pRowData->m_strName.GetLength();
		
		if(nSecondColumnMaxLength < pRowData->m_strData.GetLength())
			nSecondColumnMaxLength = pRowData->m_strData.GetLength();
	}
	return TRUE;
}



//////////////////////////////////////////////////////////////////////
// (2006/6/24 - Seung-Won, Bae) Library Functions
//////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------
 - Function    :  FindInquiryDataIndex
 - Created at  :  2005-03-29   10:29
 - Author      :  ������
 - Parameters  :  point        - ���콺 ����Ʈ
 -                pRowViewData - �ش� RowViewData�� �׷��������� ���ϱ� ����.
 - Return Value:  UINT ���콺�� ��ġ�� ���� ����Ÿ �ε���.
 - Description :  ���� ���콺�� ��ġ�� ���� ����Ÿ�ε����� ���ؿ´�. 
				  �ر��ؿ� ������
				  �� ���콺��ġ�Ѱ��� ����Ÿ����(�ε���) : ���������� ����Ÿ���� = ���콺.x : �׷����� ���ο���
 -------------------------------------------------------------------------------*/
UINT CNumericalInquiryDataQueryer::FindInquiryDataIndex( CPoint p_ptPoint, POSITION p_psnViewData)
{
// (2006/7/10 - Seung-Won, Bae) Use new Logic for Data Index of Mouse Point (TimeMarker version. BaeSeungWon.)
/*
	// 0. �����Ŀ� ���� �������� ���ؾ� �Ѵ�. ���������� ����Ÿ����, ���콺����Ʈ�� X��, 
	//	  �׸��� �׷����� ���ο����̴�.

	// 0.1. ���������� ����Ÿ ������ ���ؿ´�.
	int nStartIndex, nEndIndex;
	if( !m_pIChartManager->GetDataIndexRangeInView( nStartIndex, nEndIndex)) return -1;
	int nPageDataCount = nEndIndex - nStartIndex + 1;
	if( nPageDataCount <= 0) return -1;

	// 0.2. ���콺 ����Ʈ�� X��. 
	// --> �Լ��Ķ���ͷ� �̹� ����!

	// 0.3. RowViewData�� �׷��������� Width���� �������� ���� �׷��������� ���ؿ´�.
	CRect rectGraphRegion;
	m_pIChartManager->GetSubGraphViewData_GraphRegion( p_psnViewData, rectGraphRegion);
	//sy 2005.11.01.
	// graph region ���ʿ� ���� ��� -> ������ ùdata
	// graph region �����ʿ� ���� ��� -> �������� ùdata
	if(rectGraphRegion.left >= point.x)
		return nStartIndex;
	else if(rectGraphRegion.right <= point.x)
		return GetIndexInRightMargin( nEndIndex);
	//sy end
	
	// Index���ؿ���. �Լ��ּ��� ������ ����.
	int nIndex = int((double)((point.x - rectGraphRegion.left) * nPageDataCount) / (double)rectGraphRegion.Width());
	// ����ѳ��� ������ ����ó�����ִ� �뾲!
	if(nIndex < 0 || nIndex > nEndIndex)
		return -1;

	//sy 2005.11.01.
	// nIndex�� �ܼ��� ���� ���̴� �׷������������� �ε����̹Ƿ� 
	// ���� ����Ÿ������ �ε����� Start Index�� ������� �Ѵ�.
	return GetIndexInRightMargin( nStartIndex + nIndex);
*/

	// 1.1 �־��� Point�� �ش��ϴ� Block�� Graph Region�� ���Ѵ�.
	// (2004.06.14, ��¿�) Block ������ Height��, Graph�� Width ������ Input �������� Ȯ���Ѵ�.
	CRect rctGraphRegion = m_pIChartManager->GetSubGraphViewData_GraphRegion( p_psnViewData);

	// 1.2 ���� ȭ�鿡 ���̴� Data�� Start/End Index�� ���Ѵ�.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return -1;

	//if( p_ptPoint.x <= rctGraphRegion.left || rctGraphRegion.right <= p_ptPoint.x) return -1;
	if( p_ptPoint.x <= rctGraphRegion.left) return nStartIndex;
	if(rctGraphRegion.right <= p_ptPoint.x) return nEndIndex;	

	// 1.3 ������ Mouse Point�� Data Index�� ���Ѵ�.
	return nStartIndex + ( ( p_ptPoint.x - rctGraphRegion.left) * ( nEndIndex - nStartIndex + 1)) / rctGraphRegion.Width();
}

// (2006/7/10 - Seung-Won, Bae) Use new Logic for Data Index of Mouse Point (TimeMarker version. BaeSeungWon.)
/*
//sy 2005.11.01.
int CNumericalInquiryDataQueryer::GetIndexInRightMargin(const int nIndex) const
{
	if(nIndex <= 0)
		return nIndex;

	// ������ ������ ���� ���
	int nRightMargin = 0;
	nRightMargin = m_pIChartOCX->GetBongCountMargin();
	if(nRightMargin <= 0)
		return nIndex;

	int nMaxRange;
	if( !m_pIChartOCX->GetMaxScrollRange( nMaxRange)) nMaxRange = 0;
	int nIndexMax = nMaxRange - nRightMargin;
	if(nIndex < nIndexMax)
		return nIndex;

	return (nIndexMax - 1);
}
//sy end
*/

/*-------------------------------------------------------------------------------
 - Function    :  SetHorzScaleDataToContainer
 - Created at  :  2005-03-24   14:53
 - Author      :  ������
 - Parameters  :  strHorzScaleName   - �����ϸ�.
 -                nDataIndex - ����Ÿ �ε���
 -				  listData - 
 - Return Value:  BOOL - TRUE: ����, FALSE: ����.
 - Description :  �����ϵ���Ÿ�� �����̳ʿ� �����Ѵ�.
 -------------------------------------------------------------------------------*/
// ��ġ��ȸâ�� �ڷ����� Format, X Scale �������� ���� ó�� - ojtaso (20070827)
BOOL CNumericalInquiryDataQueryer::SetHorzScaleDataToContainer(CString szRQ, const CString& strHorzScaleName, int nDataIndex, CInquiryItem *listData, const int nRow, const int nColumn)
{
	IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strHorzScaleName);
	if( pPacket == NULL) return FALSE;

	POSITION pos = NULL;
	CString strScaleData = _T("");	// Scale Data.
	double dData = 0;

	ILPCSTR szPacketType = pPacket->GetType();
	CString strPacketType = szPacketType;
	
	// 2011.01.25 by SYS >> ����Ʈ YYYY(��) : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	int nHourType = 0;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacket->GetDateUnitType();
	if (strPacketType == _T("YYYYMMDDHHMMSS"))
	{
		if( CScaleBaseData::HORZ_TIME_ONE	== eDateUnit || 
			CScaleBaseData::HORZ_TIME_TWO	== eDateUnit ||
			CScaleBaseData::HORZ_TICK		== eDateUnit ||
			CScaleBaseData::HORZ_TIME_SECOND== eDateUnit)
		{
			if (strPacketType.Find("HH") >= 0)
				nHourType = 1;
		}
	}
	// 2011.01.25 by SYS <<
	
	ILPCSTR szScaleData;
	if( strPacketType == _MTEXT( C6_CHARACTER))
	{
		pPacket->GetData( nDataIndex, szScaleData);
		strScaleData = szScaleData;
	}
	else
	{
		dData = 0.0;
		if( !pPacket->GetData( nDataIndex, dData))
			pPacket->GetDataPastandFuture( nDataIndex, dData);
		if( dData == 0.0)
		{
			pPacket->Release();
			return FALSE;
		}

		if(strHorzScaleName.Compare(_MTEXT( C0_DATE_TIME)) != 0)
			strScaleData = CDataConversion::DoubleToString(dData);
		else 
		{
			// ��ġ��ȸâ�� �ڷ����� Format, X Scale �������� ���� ó�� - ojtaso (20070827)
			ILPCSTR szCompartDate;
			ILPCSTR szCompartTime;
			m_pIChartManager->GetHorzScaleCompart( nRow, nColumn, szCompartDate, szCompartTime);
			CString strCompartDate( szCompartDate);
			CString strCompartTime( szCompartTime);
			
			//ILPCSTR szPacketType = pPacket->GetType();
			//CString strPacketType( szPacketType);
			
			CString strData = CDataConversion::DoubleToString(dData);

			// 2011.01.25 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
			if (strPacketType == _T("YYYYMMDDHHMMSS"))
			{
				// nDataUnit (0:nontype, 1:��, 2:��, 3:��, 4:��, 5:ƽ, 6:Text)
				CString strFormat = _T("YYYYMMDD");	// ����Ʈ����
				switch(eDateUnit)
				{
				case CScaleBaseData::HORZ_DAILY:		// ��
				case CScaleBaseData::HORZ_WEEKLEY:		// �� 
					strFormat = _T("YYYYMMDD");		
					break;
				case CScaleBaseData::HORZ_MONTHLY:		// �� 
					strFormat = _T("YYYYMM");		
					break;
				case CScaleBaseData::HORZ_TIME_ONE:		// ��
				case CScaleBaseData::HORZ_TIME_TWO:
					strFormat = _T("MMDDHHMM");
					break;
				case CScaleBaseData::HORZ_TICK:			// ƽ 
				case CScaleBaseData::HORZ_TIME_SECOND:
					strFormat = _T("DDHHMMSS");
					break;
				}
				strScaleData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, strFormat, strCompartDate, strCompartTime);
			}
			else
				strScaleData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, strPacketType, strCompartDate, strCompartTime);
			// 2011.01.25 by SYS <<
		}
	}

	CString strScaleName;
	if(strHorzScaleName.Compare(_MTEXT( C0_DATE_TIME)) != 0)
	{
		strScaleName = _MTOL0( strHorzScaleName);
	}
	else
	{
		ML_SET_LANGUAGE_RES();

		// 2011.01.25 by SYS >> ����Ʈ YYYY(��) : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		// ILPCSTR szPacketType = pPacket->GetType();
		if (strPacketType == _T("YYYYMMDDHHMMSS"))
		{
			if (nHourType == 1)
				strScaleName.LoadString(IDS_TIME);
			else
				strScaleName.LoadString(IDS_DATE);
		}
		else
		{
			if( strPacketType.Find("HH") >= 0)	
				strScaleName.LoadString(IDS_TIME);
			else
				strScaleName.LoadString(IDS_DATE);
		}
		// 2011.01.25 by SYS <<
	}

	listData->AddTail( new CRowData( strScaleName, strScaleData));

	pPacket->Release();
	return TRUE;
}



/*-------------------------------------------------------------------------------
 - Function    :  SetInquiryDataToContainer
 - Created at  :  2005-03-24   14:22
 - Author      :  ������
 - Description :  ��ȸ����Ÿ�� �����̳ʿ� �����ϴ� �������� �κ�.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDataQueryer::SetInquiryDataToContainer(CString szRQ, const CString& strTitle, const CString& strPacketNames, int nDataIndex, CInquiryItem *listData, bool bToolTip, bool bScaleData_Date, bool bSetTitle)
{
	CString strRealPacketNames = GetRealPacketNames(strTitle, strPacketNames);
	int nPacketCount = CDataConversion::GetStringDataCount(strRealPacketNames, _T(","));

	CString strName, strData;
	if(nPacketCount == 1)
	{
		if(bSetTitle == false)
		{
			LPCTSTR lpcszCode = NULL;
			LPCTSTR lpcszName = NULL;
			m_pIPacketListManager->GetItemCode( szRQ, lpcszCode, lpcszName);

			if(lpcszName && strlen(lpcszName) > 0)
			{
				ML_SET_LANGUAGE_RES();

				CString strIssueName;
				strIssueName.LoadString(IDS_ISSUENAME);

				listData->AddTail( new CRowData( "", ""));
				listData->AddTail( new CRowData( strIssueName, lpcszName));
			}
		}

		strName = CDataConversion::GetStringData(strRealPacketNames, _T(","));
		IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strName);
		if( !strTitle.IsEmpty()) strName = strTitle;

		if( strName !=  _MTEXT( C2_OVERHANGING_SUPPLY) && pPacket && (pPacket->GetIndexOfRealDataStart() <= nDataIndex))
		{
			//////////////////////////////////////////////////////////////////////////
			CString strMetaText(strName);
			CString strUIText;
			if(strMetaText == _MTEXT( C0_DATE_TIME) || strMetaText == _MTEXT( C0_OPEN) || 
				strMetaText == _MTEXT( C0_HIGH) || strMetaText == _MTEXT( C0_LOW) ||
				strMetaText == _MTEXT( C0_CLOSE))
				strUIText = _MTOL0(strMetaText);
			else
			{
				strUIText = _MTOL1_N( strMetaText);
				if( strUIText.IsEmpty()) strUIText = _MTOL3(strMetaText);
			}
			strName = strUIText;
			//////////////////////////////////////////////////////////////////////////

			ILPCSTR szPacketType = pPacket->GetType();
			if( CString( szPacketType) == _MTEXT( C6_CHARACTER))
			{
				ILPCSTR szData;
				if( pPacket->GetData( nDataIndex, szData))
					listData->AddTail( new CRowData( strName, szData));
			}
			else
			{
				double dData = 0.0;
				if( pPacket->GetData( nDataIndex, dData))
				{
					ILPCSTR szPacketType = pPacket->GetType();
					CString strType = szPacketType;
					int nLength = strType.GetLength();
					int nDecimalPt =  strType.Find(".") + 1;
					int nDecimal;
				
					if(nDecimalPt == 0)
						nDecimal = 0;
					else
						nDecimal = nLength - nDecimalPt;

					// �ִ� �Ҽ��� �ڸ��� ����صα�.(���߿� ��ġ��ȸâ ǥ���Ҷ� �Ҽ����ڸ��� ���߱�����)
					if(nDecimal > m_nMaxDecimal)
						m_nMaxDecimal = nDecimal;

					// 20080910 JS.Kim		8����/32���� ó��
					//strData = CDataConversion::DoubleToString(dData, nDecimal);
					strData = CDataConversion::DoubleToString(dData);

					// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
					if (strType == _T("YYYYMMDDHHMMSS"))
					{
						int nDataUnit = pPacket->GetDateUnitType();
						strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans2( strData, nDataUnit);
					}
					else
						strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strData, strType);
					// 2011.01.20 by SYS <<

					listData->AddTail( new CRowData( strName, strData));
				}
			}
		}//end of else...

		if( pPacket) pPacket->Release();

	}//end of if(nPacketCount...
	else
	{
		if(bSetTitle == false)
		{
			LPCTSTR lpcszCode = NULL;
			LPCTSTR lpcszName = NULL;
			m_pIPacketListManager->GetItemCode(szRQ, lpcszCode, lpcszName);

			if(lpcszName && strlen(lpcszName) > 0)
			{
				ML_SET_LANGUAGE_RES();

				CString strIssueName;
				strIssueName.LoadString(IDS_ISSUENAME);

				listData->AddTail( new CRowData( "", ""));
				listData->AddTail( new CRowData( strIssueName, lpcszName));
			}
		}

		while(strRealPacketNames.IsEmpty() == false)
		{
			strName = CDataConversion::GetStringData(strRealPacketNames, _T(","));

			IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strName);
			if( !pPacket) return;
			if( pPacket->GetIndexOfRealDataStart() <= nDataIndex)
			{
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

				ILPCSTR szPacketType = pPacket->GetType();
				if( CString( szPacketType) == _MTEXT( C6_CHARACTER))
				{
					ILPCSTR szData = NULL;
					if( pPacket->GetData( nDataIndex, szData))
						listData->AddTail( new CRowData( strName, szData));
				}
				else
				{
					double dData = 0.0;
					if( pPacket->GetData( nDataIndex, dData))
					{
						CString strPacketType = szPacketType;
						ILPCSTR szPacketType = pPacket->GetType();
						int nLength = strPacketType.GetLength();
						int nDecimalPt = strPacketType.Find(".") + 1;
						int nDecimal;
					
						if(nDecimalPt == 0)	nDecimal = 0;
						else				nDecimal = nLength - nDecimalPt;

						// �ִ� �Ҽ����ڸ��� ����صα�.(���߿� ��ġ��ȸâ ǥ���Ҷ� �Ҽ����ڸ��� ���߱�����)
						if(nDecimal > m_nMaxDecimal) m_nMaxDecimal = nDecimal;

						// 20080910 JS.Kim		8����/32���� ó��
						//strData = CDataConversion::DoubleToString(dData, nDecimal);
						strData = CDataConversion::DoubleToString(dData);

						// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
						if (strPacketType == _T("YYYYMMDDHHMMSS"))
						{
							int nDataUnit = pPacket->GetDateUnitType();
							strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans2( strData, nDataUnit);
						}
						else
							strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strData, strPacketType);
						// 2011.01.20 by SYS <<

						listData->AddTail( new CRowData( strName, strData));
					}
				}
			}

			pPacket->Release();
		}//end of while(...
	}//end of else
}

/*-------------------------------------------------------------------------------
 - Function    :  GetRealPacketNames
 - Created at  :  2005-03-24   14:08
 - Author      :  ������
 - Parameters  :  strTitle       - "����"�̰�..
 -                strPacketNames - "�ð�,��,����,����"�� ���...
 - Return Value:  CString - ���Ӱ� �ٲ� ��Ŷ��.
 - Description :  1ƽ�ϰ��� �˻��Ͽ� ���ο� ��Ŷ���� �������ش�.
 -------------------------------------------------------------------------------*/
CString CNumericalInquiryDataQueryer::GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const
{
	CString strRealPacketNames = strPacketNames;
	if(strRealPacketNames.Right(1) != ",")
		strRealPacketNames += ",";

	// title = "����", packetName = "�ð�,��,����,����", "1ƽ"�� ��� 
	// -> packetName : "����"
	if(strTitle == _MTEXT( C3_PRICE) && strPacketNames.Find(_MTEXT( C0_CLOSE)) > 0)
	{
		if( m_pIDefaultPacketManager->GetBaseTimeDataType() == CScaleBaseData::HORZ_TICK && m_pIDefaultPacketManager->GetBaseTimeUnit() == 1)
		{
			strRealPacketNames = _MTEXT( C0_CLOSE);
			strRealPacketNames += ",";
		}
	}

	return strRealPacketNames;
}

// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
void CNumericalInquiryDataQueryer::ClearInquiryDataList( void)
{
	// (2006/6/24 - Seung-Won, Bae) Manage the Data with Group (?)
	CInquiryItem *plistData = NULL;
	while( !m_listInquiryData.IsEmpty())
	{
		plistData = m_listInquiryData.RemoveHead();
		while( !plistData->IsEmpty()) delete plistData->RemoveHead();
		delete plistData;
	}
	while( !m_listPrevInquiryData.IsEmpty())
	{
		plistData = m_listPrevInquiryData.RemoveHead();
		while( !plistData->IsEmpty()) delete plistData->RemoveHead();
		delete plistData;
	}
}
