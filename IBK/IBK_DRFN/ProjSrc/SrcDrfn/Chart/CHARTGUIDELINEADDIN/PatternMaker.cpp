 // PatternMaker.cpp : implementation file
//

#include "stdafx.h"
#include "PatternMaker.h"

#include "../chart_common/include_Addin/ChartDefines.h"				// for UWM_FIRECODESHARE
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IMasterDataMng.h"

#include "../COMMONTR/KB_d1017.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct __tagGData{
	char date[4];
	char time[4];
	char jmcode[6];
	char mmgb[1];
	char price[6];
}GData;

typedef struct __tagChartDate
{
	int	   nIndex;
	double dTime;
}ChartDate;

/////////////////////////////////////////////////////////////////////////////
// CPatternMaker
typedef LPCSTR (* FpGetUnitPrice2)(LPSTR, LPSTR);
FpGetUnitPrice2 pGetUnitPrice2 = NULL;


CPatternMaker::CPatternMaker()
{
	m_strCode			= _T("");
	
	m_pPatternEnvData	= NULL;	// ���� ȯ�漳�� ������
	
	m_bLog				= FALSE;	// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	m_bReverse			= FALSE;	// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	m_bSpecialChart		= FALSE;
	m_bDrawChegyulLine	= FALSE;

	m_dViewMin			= 0;		// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	m_dViewMax			= 0;		// View���� ��� Graph�� View�� Maximum Scale ���̴�.
		
	m_pnHighLowLinePen	.CreatePen( PS_SOLID,	2, RGB( 128, 0, 0));	// �ֿ���� ���� ������� ����
	m_pnMaPen			.CreatePen(	PS_SOLID,	2, RGB( 255, 0, 0));	// �̵���� ����
	m_pnHighLinePen		.CreatePen(	PS_SOLID,	1, RGB( 0, 0, 255));	// �ֿ���� ���� ����
	m_pnLowLinePen		.CreatePen(	PS_SOLID,	1, RGB( 255, 0, 0));	// �ֿ����� ���� ����
	
	Init_GuideLineValue();
//	CreateLinePen();	

	ReadOneLineForSamgungDemo();

	m_hwndChartItem = NULL;
	m_hSelChartItem = NULL;

	m_bWonChart = TRUE;

	//{{ ���(����) ó�� ���
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		int nID = 2;
		m_pITrCommManager = (IAUTrCommManager*)pGateMng->GetInterface(nID);
		m_TRComm = m_pITrCommManager->AdviseTrComm();
		m_TRComm->ChangeCallback(&m_xTrCommSite);
	}
	//}}
	m_nKeySize = 6;
}

CPatternMaker::~CPatternMaker()
{
	if(m_pITrCommManager && m_TRComm)
		m_pITrCommManager->UnAdviseTrComm(m_TRComm);

	// ���� ����
	DeleteLinePen();

	// �ֿ���� ���� ������� ����
	m_pnHighLowLinePen.DeleteObject();

	// �̵���� ���� 
	m_pnMaPen.DeleteObject();

	// �ֿ���� ���� ����
	m_pnHighLinePen.DeleteObject();

	// �ֿ����� ���� ����
	m_pnLowLinePen.DeleteObject();


	// �ֿ����, �ֿ����� ������ ����
	DeleteHighLowPtData();

	// �̵���� ������ ����
	DeleteMAData();

	// ����, ������ ������ ����
	DeleteRegistNSupportData();

	// ü�� ������ ����
	DeleteCheChartData();

	m_strRQ = "DEFAULT";
}

// (2004.06.10, ��¿�) Drawing ������ �������� ����Ѵ�.
void CPatternMaker::OnDraw( CDC *p_pDC)
{
	// ���� ȯ�漳�� �����Ͱ� ������ ����
	//if ( !m_pPatternEnvData) return;

	// (2004.09.15, ��¿�) Ư����Ʈ�� ��쿡�� ������ �ʵ��� �Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	if( m_p_pIChartOCX->IsOnSpecialChart())
	{
		m_bSpecialChart		= TRUE;
		return;
	}
	else
		m_bSpecialChart		= FALSE;

	// Drawing�� ������ ��Ȳ���� Ȯ���Ѵ�.
	if( !m_p_pIChartManager || !m_p_pIChartOCX || !m_p_pIPacketManager) return;

	// (2004.07.05, ��¿�) Time Mark�� ���� �ٸ� ���񿡼��� ��ȿ�ϵ���, 
	//		Packet�� Type�� �����Ͽ�, ����� ��찡 �ƴϸ� ��� Draw�ϵ��� �Ѵ�.
	//IPacket *pIPacket = m_p_pIPacketManager->GetPacket( "�ڷ�����");
	IPacket *pIPacket = m_p_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return;



	try
	{
		// ��Ʈ�� �⺻���� --------------------------------------------------------------------->>
		// �� Data View�� Start/End Index�� ���Ѵ�.
		int nStartIndex, nEndIndex;
		//if( !m_p_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) throw this;
		if( !m_p_pIChartManager->GetStartEndDrawIdx_Cur(m_strRQ, nStartIndex, nEndIndex)) throw this;

		// ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
		int nR, nC, nV, nG;
		CRect		rctBlockRegion, rctGraphRegion;	
		//if( !m_p_pIChartManager->GetIndexOfGraph( "������Ʈ", nR, nC, nV, nG)) return;
		if( !m_p_pIChartManager->GetIndexOfGraph( m_strRQ, _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG)) return;
		
		IBlock *pIBlock = m_p_pIChartManager->GetBlock( nR, nC);
		if( pIBlock)
		{
			// 2.2.1 ������Ʈ�� Block�� Log/������ �������� Ȯ���Ѵ�.
//			m_bLog = pIBlock->IsLogScale();
//			m_bReverse = pIBlock->IsVScaleInvert( nV);
			pIBlock->GetLogAndReverseScaleFlag(m_bLog, m_bReverse);
			// 2.2.0 View�� Min/Max ���� ���Ѵ�.
			pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
	   		pIBlock->GetGraphRegions(rctGraphRegion, rctBlockRegion );
			rctGraphRegion = pIBlock->GetGraphDrwingRegion();		
			pIBlock->Release();
			pIBlock = NULL;
		}
/*		if( !m_p_pIChartManager->GetBlockIndexOfGraph( "������Ʈ", nR, nC)) throw this;
		
		// Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
		m_p_pIChartManager->GetVerticalScaleConversionType( nR, nC, m_bLog, m_bReverse);

		// View�� Min/Max ���� ���Ѵ�.
		m_p_pIChartManager->GetMinMaxDataInViewWithGraph( "������Ʈ", m_dViewMin, m_dViewMax);

		rctBlockRegion = m_p_pIChartManager->GetNoFrameGraphRegion( nR, nC);		// Frame ����
		rctGraphRegion = m_p_pIChartManager->GetDrawingGraphRegion( nR, nC);		// Graph ����
*/				
		int		nDataOnPage = nEndIndex - nStartIndex + 1;	// �� Data View�� Data ����
		double	dCandleWidth = double( rctGraphRegion.Width()) / double( nDataOnPage);	// ���� ���� ���Ѵ�
		// <<--------------------------------------------------------------------------------------
		CPen *pOldPen = NULL; 
//@��������		LOGPEN logpen;
		int nYHigh = 0; 

		//#########################################################################################
		//ü�� ���ؼ� �� ü������� ǥ���Ѵ�.
		//#########################################################################################
		if(m_bDrawChegyulLine)
		{
//{{ test ������ ���� = ����
			CString strPacketType="YYYYMMDD";   
//			pIPacket->GetPacketTypeString( strPacketType);
//			OutputDebugString(strPacketType);
//	}}

			//CList<double,double> *clistDate;
			//clistDate = NULL;
			//clistDate = pIPacket->GetDataList();

//@��������			POSITION	posDate;
//@��������			double		dDate;
			//int			nDataSequence = clistDate->GetCount();
			int			nDataSequence = pIPacket->GetDataCount();
			// �Ϻ�/�ֺ�/���� �� ��� ====================================
//@			OutputDebugString("ü�� Ÿ�� " + strPacketType);
			if( strPacketType.Left(4) == "YYYY")
			{
				

//				double dValue[] = { 999.00,
//					842.00,
//					564.00,
//					653.00,
//					765.00
//				};

				
				int nDataCnt= 0;
				int nXCrossPt = 0;
				CString sTemp, sDate, sCode, sMMGb, sPrice;
				int nSize  = m_ArrayFileChart.GetSize();
				//POSITION posDate = clistDate->GetTailPosition();
				for(int i=0;i<nSize;i++)
				{
					sTemp = m_ArrayFileChart.GetAt(i);
					if(sTemp.GetLength())
					{
//@						OutputDebugString(sTemp);
						GData *pData = (GData*)(LPTSTR)(LPCTSTR)sTemp;

						//�����ڵ� �� 
						sCode.Format("%6.6s", pData->jmcode);
						if(sCode != m_strCode) continue;
						
						

						//��¥�� ���Ѵ�.
						sDate = sTemp.Left(4);
						double dReadDate = atof(sDate);

					
						CString sDate;
						double dDate;
						pIPacket->GetData(i, dDate);
						sDate.Format("%f", dDate);
						double dLastDate = atof(sDate.Mid(4,4));
						if(dReadDate == dLastDate )
						{
							nDataCnt++;
						
							if(nDataCnt == 1)//�Ϻ��� �ѹ��� �׸���.
							{
								nDataSequence--;
								pOldPen = p_pDC->SelectObject(&m_penChegyul);
								// �ð� ���ؼ��� ������ �Ʒ��� �׸���.
								double dDataOffset = nDataSequence - nStartIndex;
								nXCrossPt = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
								
								p_pDC->MoveTo( nXCrossPt, rctGraphRegion.top);
								p_pDC->LineTo( nXCrossPt, rctGraphRegion.bottom);

								p_pDC->SelectObject(pOldPen);			
								pOldPen = NULL;
							}

							//ü�� ������ ���Ѵ�.
							sPrice.Format("%6.6s", pData->price);
							double dValue = atof(sPrice);

							nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
							rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

							sMMGb.Format("%1.1s", pData->mmgb);
							if(sMMGb == "2")
							{
								pOldPen = p_pDC->SelectObject(&m_penSell);
							}
							else if(sMMGb == "1")
							{
								pOldPen = p_pDC->SelectObject(&m_penBuy);
							}

							p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
							p_pDC->LineTo( nXCrossPt + 5, nYHigh);

							p_pDC->SelectObject(pOldPen);			
							pOldPen = NULL;
						}
					}
				}

			
				// ü����� �׸���. -----------------------------------------------------------------(2-1)
				/*for(int i = 0; i < 5; i++)
				{					
					if((i == 0) ||
					   (i == 1) ||
					   (i == 4) )
					{
						pOldPen = p_pDC->SelectObject(&m_penSell);
					}
					else if((i == 3) ||
					   (i == 2) )
					{
						pOldPen = p_pDC->SelectObject(&m_penBuy);
					}
					
					int nTick = 0;
					if( m_dViewMax > 100000)
						nTick = 5000;
					else if( m_dViewMax > 10000)
						nTick = 500;
					else if( m_dViewMax > 1000)
						nTick = 50;

					double dValue = m_dTHprice - (nTick * i);
					CString sTempTT;
					sTempTT.Format("ü�� �ܰ� %f", dValue);
					OutputDebugString(sTempTT);
					// Y��ǥ�� ���Ѵ�					
					nYHigh = p_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
						rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
								
					//ü�� ������ �׸���.
					if(dValue > m_dTLprice)
					{
						p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
						p_pDC->LineTo( nXCrossPt + 5, nYHigh);

									
					}
					p_pDC->SelectObject(pOldPen);
					pOldPen = NULL;
				}*/
			}
			else if( (strPacketType.Left( 4) == "MMDD") ||//ƽ ���� / �к��� ���
					 (strPacketType.Left( 4) == "HHMM") ) 
			{
				//################################################################################
				//##�Ｚ �ÿ���
				IPacket *pIPacketHigh, *pIPacketLow;
				pIPacketHigh = pIPacketLow= NULL;		
				pIPacketHigh	= m_p_pIPacketManager->GetPacket(m_strRQ, _MTEXT( C0_HIGH));
				pIPacketLow		= m_p_pIPacketManager->GetPacket(m_strRQ, _MTEXT( C0_LOW));
				if ( !pIPacketHigh || !pIPacketLow ) return;

				//CList<double, double>  *clistHigh, *clistLow;			
				//clistHigh = clistLow = NULL;
					
				//clistHigh	= pIPacketHigh->GetDataList();
				//clistLow	= pIPacketLow->GetDataList();			
				

				int nTotalDataCnt	= pIPacketHigh->GetDataCount();

				double *pdDate		= new double[nTotalDataCnt];	// �ð�
				double *pdHigh		= new double[nTotalDataCnt];	// ��
				double *pdLow		= new double[nTotalDataCnt];	// ����				

				for(int nNum=0; nNum< nTotalDataCnt; nNum++)
				{
					pIPacket->GetData(nNum, pdDate[nNum]);
					pIPacketHigh->GetData(nNum, pdHigh[nNum]);
					pIPacketLow->GetData(nNum, pdLow[nNum]);
				}
//				POSITION	posDate, posHigh, posLow;
//				
//				for(posDate = clistDate->GetHeadPosition(),
//					posHigh  = clistHigh->GetHeadPosition(),
//					posLow  = clistLow->GetHeadPosition(); 
//					posDate && posHigh && posLow;)
//				{
//					if (posDate == NULL)	continue;
//					
//					pdDate[nNum]	= (double)clistDate->GetNext(posDate);
//					pdHigh[nNum]	= (double)clistHigh->GetNext(posHigh);
//					pdLow[nNum]		= (double)clistLow->GetNext(posLow);
//					nNum++;
//				}			
				
//				if( (m_strCode == "016360") ||
//					(m_strCode == "006400")
//				)
//				{	
					
					CString sTemp, sDate, sCode, sMMGb, sPrice;
					int nSize  = m_ArrayFileChart.GetSize();
					for(int i=0;i<nSize;i++)
					{
						sTemp = m_ArrayFileChart.GetAt(i);
						if(sTemp.GetLength())
						{
//@							OutputDebugString(sTemp);
							GData *pData = (GData*)(LPTSTR)(LPCTSTR)sTemp;

							//�����ڵ� �� 
							sCode.Format("%6.6s", pData->jmcode);
							if(sCode != m_strCode) continue;								
							
							//��¥.
							sDate = sTemp.Left(8);
							double dReadDate = atof(sDate);
							int nNewDataSequence = 0;
							nNewDataSequence = nDataSequence;
							while(nNewDataSequence)
							{	
								nNewDataSequence--;
								if(dReadDate > pdDate[nNewDataSequence])
								{
									//���� ���ؼ� 
									pOldPen = p_pDC->SelectObject(&m_penChegyul);

									double dDataOffset = nNewDataSequence - nStartIndex;				
									int nXCrossPt = GetXPositoin( dDataOffset + 1, dCandleWidth, rctGraphRegion);
									if(nXCrossPt < rctGraphRegion.right)
									{
										p_pDC->MoveTo( nXCrossPt, rctGraphRegion.top);
										p_pDC->LineTo( nXCrossPt, rctGraphRegion.bottom);
									}

									p_pDC->SelectObject(pOldPen);			
									pOldPen = NULL;
									
									//ü�� ������ ���Ѵ�.
									sPrice.Format("%6.6s", pData->price);
									double dValue = atof(sPrice);
									nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
									rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

									sMMGb.Format("%1.1s", pData->mmgb);
									if(sMMGb == "2")
									{
										pOldPen = p_pDC->SelectObject(&m_penSell);
									}
									else if(sMMGb == "1")
									{
										pOldPen = p_pDC->SelectObject(&m_penBuy);
									}

									p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
									p_pDC->LineTo( nXCrossPt + 5, nYHigh);

									p_pDC->SelectObject(pOldPen);			
									pOldPen = NULL;
									
									break;
								}
							}
						}							
					}
					
				//}
				/*
				else
				{
					//################################################################################
					//ü�� ���ؼ��� �׸���.===================================================
						
					int nIndexArray[30] = { 0,0,1,0,1,0,0,1,0,1,
											0,1,1,0,0,1,0,0,1,0,
											0,0,1,0,1,0,0,1,0,0};

					
					for(int i = 0; i < 30 ; i++)
					{   	
						nDataSequence--;	
						// �ð� ���ؼ��� ������ �Ʒ��� �׸���.======================================
						
										
						double dDataOffset = nDataSequence - nStartIndex;				
						int nXCrossPt = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
						
						if(nIndexArray[i] == 0)
							continue;

						double dHigh = 0;
						double dLow	 = 0;	

						dHigh = pdHigh[nNum - i - 1];
						dLow  = pdLow [nNum - i - 1];	

						pOldPen = p_pDC->SelectObject(&m_penChegyul);	
						
						CString sTempTT2;						
						sTempTT2.Format("�� : %f, ���� : %f ü�� �ܰ� %f ", dHigh, dLow);
						OutputDebugString(sTempTT2);
						
						p_pDC->MoveTo( nXCrossPt, rctGraphRegion.top);
						p_pDC->LineTo( nXCrossPt, rctGraphRegion.bottom);						
						
						p_pDC->SelectObject(pOldPen);			
						pOldPen = NULL;

						//ü�� ���ؼ��� �׸���.=====================================================

						// ü����� �׸���. -----------------------------------------------------------------(2-1)
						for(int j = 0; j < 5; j++)
						{					
							if((j == 0) ||
							   (j == 1) ||
							   (j == 4) )
							{
								pOldPen = p_pDC->SelectObject(&m_penSell);
							}
							else if((j == 3) ||
							   (j == 2) )
							{
								pOldPen = p_pDC->SelectObject(&m_penBuy);
							}
							
							int nTick = 0;
							if( m_dViewMax > 100000)
								nTick = 500;
							else if( m_dViewMax > 10000)
								nTick = 100;
							else if( m_dViewMax > 1000)
								nTick = 10;

									

							double dValue = dHigh - (nTick * j);
							CString sTempTT;						
							sTempTT.Format("�� : %f, ���� : %f ü�� �ܰ� %f ", dHigh, dLow, dValue);
							OutputDebugString(sTempTT);
							// Y��ǥ�� ���Ѵ�					
							nYHigh = p_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
								rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
										
							//ü�� ������ �׸���.
							if( (dValue > dLow) && (dHigh != dLow))
							{
								p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
								p_pDC->LineTo( nXCrossPt + 5, nYHigh);										
							}
							p_pDC->SelectObject(pOldPen);
							pOldPen = NULL;
						}
					} 
				}
//====================================================================================================*/		
				delete pdDate;	// ��¥ 
				delete pdHigh;	// ��
				delete pdLow;	// ����
				//clistChartDate
			}
		}

		if( (g_clConfigSet.m_bNotDrawLine) || !m_bWonChart)
		{
			pIPacket->Release();
			return;
		}

//���� ����Ÿ ���� ======================================================================================

		m_listYGLF.RemoveAll();
		stGuideLineFormat stGLF;
		CString strTemp;

		if(g_clConfigSet.m_bChkYSprice) //���� �ð�
		{	
//@			OutputDebugString("���Ͻð� �׸���");
			CString sTemp;
			sTemp.Format("���Ͻð� HWND %d", m_hwndChartItem);
//@			OutputDebugString(sTemp);
//@
//@			sTemp.Format("���Ͻð� R:%d, G:%d, B:%d", GetRValue(g_clConfigSet.m_clrYSprice),
//@													  GetGValue(g_clConfigSet.m_clrYSprice),
//@													  GetBValue(g_clConfigSet.m_clrYSprice));
//@			OutputDebugString(sTemp);
			

			nYHigh = 0;
			pOldPen = p_pDC->SelectObject(&m_penYSprice);			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dYSprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineYSprice/2, g_clConfigSet.m_clrYSprice, "���Ͻð�", m_dYSprice);
			}

			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}


		if(g_clConfigSet.m_bChkYHprice) //���� ��
		{	
//@			OutputDebugString("���ϰ� �׸���");
			nYHigh = 0;					
			pOldPen = p_pDC->SelectObject(&m_penYHprice);
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dYHprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineYHprice/2, g_clConfigSet.m_clrYHprice, "���ϰ�", m_dYHprice);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkYLprice) //���� ����
		{	
//@			OutputDebugString("�������� �׸���");
			nYHigh = 0;			
			pOldPen = p_pDC->SelectObject(&m_penYLprice);
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dYLprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
		
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLineYLprice/2, g_clConfigSet.m_clrYLprice, "��������", m_dYLprice);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkYEprice) //���� ����
		{	
//@			OutputDebugString("�������� �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penYEprice);
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dYEprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLineYEprice/2, g_clConfigSet.m_clrYEprice, "��������", m_dYEprice);
			}	
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}


		if(g_clConfigSet.m_bChkYSHL) //���� (�� + �� + �� ) /2
		{	
//@			OutputDebugString("����(�� + �� + �� ) /2 �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penYSHL);
			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dYSHL, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineYSHL/2, g_clConfigSet.m_clrYSHL, "����(��+��+��)/3", m_dYSHL);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		DrawPriceText(p_pDC, rctGraphRegion);

//���� ����Ÿ ���� ======================================================================================
		
//���� ����Ÿ ���� ======================================================================================

		if(g_clConfigSet.m_bChkTSprice) //���� �ð�
		{	
//@			OutputDebugString("���Ͻð� �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penTSprice);

			
		
				// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dTSprice, m_dViewMax, m_dViewMin, 
								rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
		
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineTSprice/2, g_clConfigSet.m_clrTSprice, "���Ͻð�", m_dTSprice);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTHprice) //���� ��
		{	
//@			OutputDebugString("���ϰ� �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penTHprice);
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dTHprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLineTHprice/2, g_clConfigSet.m_clrTHprice, "���ϰ�", m_dTHprice);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTLprice) //���� ����
		{	
//@			OutputDebugString("�������� �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penTLprice);
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dTLprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
		
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineTLprice/2, g_clConfigSet.m_clrTLprice, "��������", m_dTLprice);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTEprice) //���� ����
		{	
//@			OutputDebugString("�������� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penTEprice);
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dTEprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineTEprice/2, g_clConfigSet.m_clrTEprice, "��������", m_dTEprice);
			}	
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTHL) //���� (�� + �� ) /2
		{	
//@			OutputDebugString("����(�� + �� ) /2 �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penTHL);
			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dTHL, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLineTHL/2, g_clConfigSet.m_clrTHL, "����(��+��)/2", m_dTHL);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		
		if(g_clConfigSet.m_bChkTSHL) //���� (�� + �� + �� ) /2
		{	
//@			OutputDebugString("����(�� + �� + �� ) /2 �׸���");
			nYHigh = 0;
			pOldPen = p_pDC->SelectObject(&m_penTSHL);
			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dTSHL, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLineTSHL/2, g_clConfigSet.m_clrTSHL, "����(��+��+��)/3", m_dTSHL);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		
		if(g_clConfigSet.m_bChkTHLE) //���� (�� + �� + �� ) /4
		{	
//@			OutputDebugString("���� {�� + ��  + (�� * 2 )}/4 �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penTHLE);
			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dTHLE, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineTHLE/2, g_clConfigSet.m_clrTHLE, "����{��+��+(��*2)}/4", m_dTHLE);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		
		if(g_clConfigSet.m_bChkHighLimit) //���Ѱ�
		{	
//@			OutputDebugString("���Ѱ� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penHighLimit);
			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dHighLimit, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineHighLimit/2, g_clConfigSet.m_clrHighLimit, "���Ѱ�", m_dHighLimit);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}


		if(g_clConfigSet.m_bChkLowLimit) //���Ѱ�
		{	
//@			OutputDebugString("���Ѱ� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penLowLimit);
			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dLowLimit, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineLowLimit/2, g_clConfigSet.m_clrLowLimit, "���Ѱ�", m_dLowLimit);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		DrawPriceText(p_pDC, rctGraphRegion);

				
//���� ����Ÿ ���� ======================================================================================

//PIVOT ����  ======================================================================================
		

		//�Ǻ� 2�� ���� 
		if(g_clConfigSet.m_bChkPivot2Rst) //�Ǻ� 2�� ���� 
		{	
//@			OutputDebugString("�Ǻ� 2�� ���׼� �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivot2Rst);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPivot2Rst, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLinePivot2Rst/2, g_clConfigSet.m_clrPivot2Rst, "�Ǻ� 2�� ����", m_dPivot2Rst);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//�Ǻ� 1�� ���� 
		if(g_clConfigSet.m_bChkPivot1Rst) //�Ǻ� 1�� ���� 
		{	
//@			OutputDebugString("�Ǻ� 1�� ���׼� �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivot1Rst);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPivot1Rst, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLinePivot1Rst/2, g_clConfigSet.m_clrPivot1Rst, "�Ǻ� 1�� ����", m_dPivot1Rst);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//Pivot ���ؼ� 
		if(g_clConfigSet.m_bChkPivotGLine) //�Ǻ� ���ؼ�
		{	
//@			OutputDebugString("�Ǻ� ���ؼ� �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivotGLine);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPivotGLine, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLinePivotGLine/2, g_clConfigSet.m_clrPivotGLine, "�Ǻ� ���ؼ�", m_dPivotGLine);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//�Ǻ� 1�� ����
		if(g_clConfigSet.m_bChkPivot1GG) //�Ǻ� 1�� ����
		{	
//@			OutputDebugString("�Ǻ� 1�� ������ �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPivot1GG);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPivot1GG, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLinePivot1GG/2, g_clConfigSet.m_clrPivot1GG, "�Ǻ� 1�� ����", m_dPivot1GG);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//�Ǻ� 2�� ����
		if(g_clConfigSet.m_bChkPivot2GG) //�Ǻ� 2�� ����
		{	
//@			OutputDebugString("�Ǻ� 2�� ������ �׸���");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivot2GG);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPivot2GG, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLinePivot2GG/2, g_clConfigSet.m_clrPivot2GG, "�Ǻ� 2�� ����", m_dPivot2GG);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		DrawPriceText(p_pDC, rctGraphRegion);
//PIVOT ����  ======================================================================================

//Demark ����  ======================================================================================
		if(g_clConfigSet.m_bChkDMKPreHPrice) //Demark �����
		{	
//@			OutputDebugString("Demark ������� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penDMKPreHPrice);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dDMKPreHPrice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLineDMKPreHPrice/2, g_clConfigSet.m_clrDMKPreHPrice, "Demark �����", m_dDMKPreHPrice);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkDMKGLine) //Demark ���ؼ�
		{	
//@			OutputDebugString("Demark ���ؼ� �׸���");
			nYHigh = 0;
			pOldPen = p_pDC->SelectObject(&m_penDMKGLine);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dDMKGLine, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLineDMKGLine/2, g_clConfigSet.m_clrDMKGLine, "Demark ���ؼ�", m_dDMKGLine);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkDMKPreLPrice) //Demark ��������
		{	
//@			OutputDebugString("Demark ���������� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penDMKPreLPrice);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dDMKPreLPrice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				AddGLF(nYHigh - g_clConfigSet.m_nLineDMKPreLPrice/2, g_clConfigSet.m_clrDMKPreLPrice, "Demark ��������", m_dDMKPreLPrice);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		DrawPriceText(p_pDC, rctGraphRegion);
//Demark ����  ======================================================================================

//�̵���� ����  ======================================================================================

		if(m_dMAPeriod1 > 0 && g_clConfigSet.m_bChk2Pyung1) //�̵���� 1��° �׸�
		{	
//			OutputDebugString(m_strCode + "  �̵���� 1��° �׸� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung1);		
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod1, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	

				strTemp.Format("�̵����(�ܼ�) %d", g_clConfigSet.m_nDay2Pyung1);
				AddGLF(nYHigh - g_clConfigSet.m_nLine2Pyung1/2, g_clConfigSet.m_clr2Pyung1, strTemp, m_dMAPeriod1);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
			CString sTemp;
			sTemp.Format(" ���� 1�� : %f, üũ : %d ", m_dMAPeriod1, g_clConfigSet.m_bChk2Pyung1);
//			OutputDebugString( m_strCode + sTemp);
		}
		
//@		OutputDebugString("                  ");

		if(m_dMAPeriod2 > 0 && g_clConfigSet.m_bChk2Pyung2) //�̵���� 2��° �׸�
		{	
//			OutputDebugString(m_strCode + "  �̵���� 2��° �׸� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung2);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod2, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				strTemp.Format("�̵����(�ܼ�) %d", g_clConfigSet.m_nDay2Pyung2);
				AddGLF(nYHigh - g_clConfigSet.m_nLine2Pyung2/2, g_clConfigSet.m_clr2Pyung2, strTemp, m_dMAPeriod2);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
			CString sTemp;
			sTemp.Format(" ���� 2�� : %f, üũ : %d ", m_dMAPeriod2, g_clConfigSet.m_bChk2Pyung2);
//			OutputDebugString( m_strCode + sTemp);
		}

//@		OutputDebugString("                  ");

		if(m_dMAPeriod3 > 0 && g_clConfigSet.m_bChk2Pyung3) //�̵���� 3��° �׸�
		{	
//			OutputDebugString(m_strCode + "  �̵���� 3��° �׸� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung3);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod3, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				strTemp.Format("�̵����(�ܼ�) %d", g_clConfigSet.m_nDay2Pyung3);
				AddGLF(nYHigh - g_clConfigSet.m_nLine2Pyung3/2, g_clConfigSet.m_clr2Pyung3, strTemp, m_dMAPeriod3);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
			CString sTemp;
			sTemp.Format(" ���� 3�� : %f, üũ : %d ", m_dMAPeriod3, g_clConfigSet.m_bChk2Pyung3);
//			OutputDebugString( m_strCode + sTemp);
		}

	
//@		OutputDebugString("                  ");

		if(m_dMAPeriod4 > 0 && g_clConfigSet.m_bChk2Pyung4) //�̵���� 4��° �׸�
		{	
//			OutputDebugString(m_strCode + "  �̵���� 4��° �׸� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung4);			
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod4, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				strTemp.Format("�̵����(�ܼ�) %d", g_clConfigSet.m_nDay2Pyung4);
				AddGLF(nYHigh - g_clConfigSet.m_nLine2Pyung4/2, g_clConfigSet.m_clr2Pyung4, strTemp, m_dMAPeriod4);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
//@			CString sTemp;
//@			sTemp.Format(" ���� 4�� : %f, üũ : %d ", m_dMAPeriod4, g_clConfigSet.m_bChk2Pyung4);
//@			OutputDebugString( m_strCode + sTemp);
		}

//@		OutputDebugString("                  ");
		
		DrawPriceText(p_pDC, rctGraphRegion);
//�̵���� ����  ======================================================================================


//��ո��Դܰ� ����  ======================================================================================
		
		if(m_dPmmDanga > 0 && g_clConfigSet.m_bChkPmmDanga) //��� ���Դܰ�.
		{	
//@			OutputDebugString("��ո��Դܰ�  �׸� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPmmDanga);		
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPmmDanga, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(m_dPmmPDanga > 0 && g_clConfigSet.m_bChkPmmPDanga) //��� ���Դܰ� ���� +% ��
		{	
//@			OutputDebugString("��� ���Դܰ� ���� +% �׸� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPmmPDanga);		
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPmmPDanga, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);

				AddGLF(nYHigh - g_clConfigSet.m_nLinePmmDanga/2, g_clConfigSet.m_clrPmmDanga, strTemp, m_dPmmDanga);
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(m_dPmmMDanga > 0 && g_clConfigSet.m_bChkPmmMDanga) //��� ���Դܰ�.
		{	
//@			OutputDebugString("��� ���Դܰ� ���� -%  �׸� �׸���");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPmmMDanga);		
			
			
			// Y��ǥ�� ���Ѵ�
			nYHigh = m_p_pIChartOCX->ConvertDataToYPosition(	m_dPmmMDanga, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		DrawPriceText(p_pDC, rctGraphRegion);

	}
	catch( CPatternMaker *)
	{
		pIPacket->Release();
		return;
	}
	
	pIPacket->Release();
}


void CPatternMaker::GetBasicData_ForGuideLine( IChartManager *p_pIChartManager, IPacketManager *p_pIPacketManager, IChartOCX *p_pIChartOCX)
{
//@��������	BOOL m_bChkYSprice; //���ϰ��� �ð�				üũ ����
//@��������	BOOL m_bChkYHprice; //���ϰ��� ��				üũ ����
//@��������	BOOL m_bChkYLprice; //���ϰ��� ����				üũ ����
//@��������	BOOL m_bChkYEprice; //���ϰ��� ����				üũ ����
//@��������	BOOL m_bChkYSHL;	//���ϰ��� (��+��+��)/3		üũ ����

	if( 
		g_clConfigSet.m_bChkYSprice ||
		g_clConfigSet.m_bChkYHprice ||
		g_clConfigSet.m_bChkYLprice ||
		g_clConfigSet.m_bChkYEprice ||
		g_clConfigSet.m_bChkYSHL ||
		g_clConfigSet.m_bChkTSprice ||
		g_clConfigSet.m_bChkTHprice ||
		g_clConfigSet.m_bChkTLprice ||
		g_clConfigSet.m_bChkTEprice ||
		g_clConfigSet.m_bChkTHL ||
		g_clConfigSet.m_bChkTSHL ||
		g_clConfigSet.m_bChkTHLE ||
		g_clConfigSet.m_bChkHighLimit ||
		g_clConfigSet.m_bChkLowLimit ||
		g_clConfigSet.m_bChkPivot2Rst ||
		g_clConfigSet.m_bChkPivot1Rst ||
		g_clConfigSet.m_bChkPivotGLine ||
		g_clConfigSet.m_bChkPivot1GG ||
		g_clConfigSet.m_bChkPivot2GG ||
		g_clConfigSet.m_bChkDMKPreHPrice||
		g_clConfigSet.m_bChkDMKGLine ||
		g_clConfigSet.m_bChkDMKPreLPrice ||
		g_clConfigSet.m_bChk2Pyung1 ||
		g_clConfigSet.m_bChk2Pyung2 ||
		g_clConfigSet.m_bChk2Pyung3 ||
		g_clConfigSet.m_bChk2Pyung4
		)
	{
		GetPMMDanga();

		// 0. ���� ȯ���� �˻��Ѵ�.
		if( !p_pIChartManager || !p_pIPacketManager || !p_pIChartOCX) return;

		// �ð����� �ڷ����ڸ� ���Ѵ� ---------------------------------------------->>
		IPacket *pIPacketTime, *pIPacketOpen, *pIPacketHigh, *pIPacketLow, *pIPacketClose;
		pIPacketTime = pIPacketOpen = pIPacketHigh = pIPacketLow = pIPacketClose = NULL;

		pIPacketTime	= p_pIPacketManager->GetPacket( m_strRQ, "�ڷ�����");
		pIPacketOpen	= p_pIPacketManager->GetPacket( m_strRQ, "�ð�");
		pIPacketHigh	= p_pIPacketManager->GetPacket( m_strRQ, "��");
		pIPacketLow		= p_pIPacketManager->GetPacket( m_strRQ, "����");
		pIPacketClose	= p_pIPacketManager->GetPacket( m_strRQ, "����");

		if ( !pIPacketTime || !pIPacketOpen || !pIPacketHigh || !pIPacketLow || !pIPacketClose ) return;

		// �̵���� �ʱ�ȭ �� �ð����� ���� ----------------------------->>
		int nMAPeriod		= 5;//m_pPatternEnvData->m_nMAPeriod;		// �̵���� �Ⱓ
		int nMAFirstData	= nMAPeriod - 1;						// ù��° �̵����(���ű���)
		int nTotalDataCnt	= pIPacketTime->GetDataCount();
		int nMinDataCnt		= nTotalDataCnt - nMAPeriod;			// �̵���� �ּҰ���

		if (nMinDataCnt < 0)		return;					// ���� ��꿡 �ʿ��� �ּҵ����� ���� üũ


		double *pdMa		= new double[nTotalDataCnt];	// �̵����
		long   *plChangPoint= new long  [nTotalDataCnt];	// �ֿ��������,��������(��ȯ��)
		
		double *pdOpen		= new double[nTotalDataCnt];	// �ð�
		double *pdHigh		= new double[nTotalDataCnt];	// ��
		double *pdLow		= new double[nTotalDataCnt];	// ����
		double *pdClose		= new double[nTotalDataCnt];	// ����


		int			nNum = 0;
		for(nNum=0; nNum< nTotalDataCnt; nNum++)
		{
			pIPacketOpen->GetData(nNum, pdOpen[nNum]);
			pIPacketHigh->GetData(nNum, pdHigh[nNum]);
			pIPacketLow->GetData(nNum, pdLow[nNum]);
			pIPacketClose->GetData(nNum, pdClose[nNum]);

			pdMa[nNum]			= VALUE_ZERO;
			plChangPoint[nNum]	= VALUE_ZERO;
		}

	//�̵� ��� ==========================================================================	
		double dTemp = 0;

		// ù��° ������ �̵����------------------		
		if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung1)
		{
			m_bDraw2Pyung1 = TRUE;
			for(int i = nTotalDataCnt-1; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung1 <= i ; i--)
			{
				dTemp += pdClose[i];		// �ֱ� nMAPeriod�ϰ� ��
			}
			m_dMAPeriod1 = dTemp/g_clConfigSet.m_nDay2Pyung1;
		}
		else 
			m_bDraw2Pyung1 = FALSE;

		
		// �ι�° ������ �̵����------------------	
		dTemp = 0;
		if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung2)
		{
			m_bDraw2Pyung2 = TRUE;
			for(int i = nTotalDataCnt-1; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung2 <= i; i--)
			{
				dTemp += pdClose[i];		// �ֱ� nMAPeriod�ϰ� ��
			}
			m_dMAPeriod2 = dTemp/g_clConfigSet.m_nDay2Pyung2;
		}
		else 
			m_bDraw2Pyung2 = FALSE;
		
		// ����° ������ �̵����------------------
		dTemp = 0;
		if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung3)
		{
			m_bDraw2Pyung3 = TRUE;
			for(int i = nTotalDataCnt-1; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung3 <= i; i--)
			{
				dTemp += pdClose[i];		// �ֱ� nMAPeriod�ϰ� ��
			}
			m_dMAPeriod3 = dTemp/g_clConfigSet.m_nDay2Pyung3;
		}
		else 
			m_bDraw2Pyung3 = FALSE;
		
		// �׹�° ������ �̵����------------------
		dTemp = 0;
		if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung4)
		{
			m_bDraw2Pyung4 = TRUE;
			for(int i = nTotalDataCnt-1; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung4 <= i; i--)
			{
				dTemp += pdClose[i];		// �ֱ� nMAPeriod�ϰ� ��
			}
			m_dMAPeriod4 = dTemp/g_clConfigSet.m_nDay2Pyung4;	
		}
		else 
			m_bDraw2Pyung4 = FALSE;

		pIPacketTime->Release();
		pIPacketOpen->Release();
		pIPacketHigh->Release();
		pIPacketLow->Release();
		pIPacketClose->Release();

		delete []pdMa;
		delete []plChangPoint;
		delete []pdOpen;
		delete []pdHigh;
		delete []pdLow;
		delete []pdClose;

		RequestSiseData();
	}
}


void CPatternMaker::DeleteHighLowPtData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryHighLowPtData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryHighLowPtData.GetAt(i);
		delete pObj;
	}

	m_aryHighLowPtData.RemoveAll();
}

void CPatternMaker::DeleteMAData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryMAData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryMAData.GetAt(i);
		delete pObj;
	}

	m_aryMAData.RemoveAll();
}

void CPatternMaker::DeleteRegistNSupportData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryRegistNSupportData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryRegistNSupportData.GetAt(i);
		delete pObj;
	}

	m_aryRegistNSupportData.RemoveAll();
}


int CPatternMaker::GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion)
{
	return m_p_pIChartManager->GetXPosition(m_strRQ, dDataOffset, 0);
	//int	nLeftBorder, nRightBorder, nDataOffset, nXPos, nAdd;
	//double dDecimalFrac;
	//
	//nDataOffset = int(dDataOffset);
	//dDecimalFrac = dDataOffset - nDataOffset;
	//
	//// ���� �°�踦 ���Ѵ�. (�� ���� �������� 1 Pixcel(����) �����̴�.)
	//nLeftBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset) + 0.5) + 1;
	//
	//// ���� ���踦 ���Ѵ�. (���� �� ���� �°�躸�� 1 Pixcel(����) �����̴�.)
	//nRightBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset + 1) + 0.5) - 1;
	//
	//// �Ҽ����� ���� �߰��� Pixel
	//nAdd = int(dDecimalFrac * dCandleWidth);
	//
	//// ���� �߽��� ��´�. (�������� ����Ѵ�.)
	//nXPos = ( nLeftBorder + nRightBorder) / 2 + nAdd;
	//
	//return nXPos;
}

double CPatternMaker::GetGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2)
{
	double dGradient=0;

	if( (dXPos2 - dXPos1) != 0)
	{
		dGradient = (dYPos2 - dYPos1) / (dXPos2 - dXPos1);
	}

	return dGradient;
}


void CPatternMaker::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	// ����ȯ�漳�� ������
	if ( pPatternEnvData )
		m_pPatternEnvData = pPatternEnvData;
}


void CPatternMaker::CreateLinePen()
{
	//===========================================================================================
	m_penYSprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYSprice	, g_clConfigSet.m_clrYSprice		);//���ϰ��� �ð�				��
	m_penYHprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYHprice	, g_clConfigSet.m_clrYHprice		);//���ϰ��� ��				��
	m_penYLprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYLprice	, g_clConfigSet.m_clrYLprice		);//���ϰ��� ����				��
	m_penYEprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYEprice	, g_clConfigSet.m_clrYEprice		);//���ϰ��� ����				��
	m_penYSHL			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYSHL		, g_clConfigSet.m_clrYSHL			);//���ϰ��� (��+��+��)/3		��
	m_penTSprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTSprice	, g_clConfigSet.m_clrTSprice		);//���ϰ��� �ð�				��
	m_penTHprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTHprice	, g_clConfigSet.m_clrTHprice		);//���ϰ��� ��				��
	m_penTLprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTLprice	, g_clConfigSet.m_clrTLprice		);//���ϰ��� ����				��
	m_penTEprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTEprice	, g_clConfigSet.m_clrTEprice		);//���ϰ��� ����				��
	m_penTHL			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTHL		, g_clConfigSet.m_clrTHL			);//���ϰ��� (��+��)/2			��
	m_penTSHL			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTSHL		, g_clConfigSet.m_clrTSHL			);//���ϰ��� (��+��+��)/3		��
	m_penTHLE			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTHLE		, g_clConfigSet.m_clrTHLE			);//���ϰ��� (��+��+(��*2))/4	��	
	m_penHighLimit		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineHighLimit	, g_clConfigSet.m_clrHighLimit		);//���Ѱ�						��
	m_penLowLimit		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineLowLimit	, g_clConfigSet.m_clrLowLimit		);//���Ѱ�						��	
	m_penPivot2Rst		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot2Rst	, g_clConfigSet.m_clrPivot2Rst		);// �Ǻ� 2�� ����				�� 
	m_penPivot1Rst		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot1Rst	, g_clConfigSet.m_clrPivot1Rst		);// �Ǻ� 1�� ����				��
	m_penPivotGLine		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivotGLine	, g_clConfigSet.m_clrPivotGLine		);// �Ǻ� ���ؼ�				��
	m_penPivot1GG		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot1GG	, g_clConfigSet.m_clrPivot1GG		);// �Ǻ� 1�� ����				��
	m_penPivot2GG		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot2GG	, g_clConfigSet.m_clrPivot2GG		);// �Ǻ� 2�� ����				��	
	m_penDMKPreHPrice	.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineDMKPreHPrice,g_clConfigSet.m_clrDMKPreHPrice	);// Demark �����			��
	m_penDMKGLine		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineDMKGLine	, g_clConfigSet.m_clrDMKGLine		);// Demark ���ؼ�				��
	m_penDMKPreLPrice	.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineDMKPreLPrice,g_clConfigSet.m_clrDMKPreLPrice	);// Demark ��������			��	
	m_pen2Pyung1		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung1	, g_clConfigSet.m_clr2Pyung1		);// �̵���� ù��° �Է°�		��
	m_pen2Pyung2		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung2	, g_clConfigSet.m_clr2Pyung2		);// �̵���� �ι�° �Է°�		��
	m_pen2Pyung3		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung3	, g_clConfigSet.m_clr2Pyung3		);// �̵���� ����° �Է°�		��
	m_pen2Pyung4		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung4	, g_clConfigSet.m_clr2Pyung4		);// �̵���� �׹�° �Է°�		��	
	m_penPmmDanga		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePmmDanga	, g_clConfigSet.m_clrPmmDanga		);// ��ո��� �ܰ�				��
	m_penPmmPDanga		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePmmPDanga	, g_clConfigSet.m_clrPmmPDanga		);// ��ո��� �ܰ� +n%			��
	m_penPmmMDanga		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePmmMDanga	, g_clConfigSet.m_clrPmmMDanga		);// ��ո��� �ܰ� -n%			��

	m_penChegyul		.CreatePen(PS_SOLID,	1/*�� ���� */	, RGB(205	,	173	,	254	)									);		// ü�� �ð�ǥ��	�� 
	m_penSell			.CreatePen(PS_SOLID,	1/*�� ���� */	, RGB(38	,	189	,	202	)									);		// �ŵ� ü�� ǥ��	��
	m_penBuy			.CreatePen(PS_SOLID,	1/*�� ���� */	, RGB(235	,	97	,	152	)									);		// �ż� ü�� ǥ��	��

	
}

void CPatternMaker::DeleteLinePen()
{
	//================================================================	
	m_penYSprice		.DeleteObject();
	m_penYHprice		.DeleteObject();
	m_penYLprice		.DeleteObject();
	m_penYEprice		.DeleteObject();
	m_penYSHL			.DeleteObject();
	m_penTSprice		.DeleteObject();
	m_penTHprice		.DeleteObject();
	m_penTLprice		.DeleteObject();
	m_penTEprice		.DeleteObject();
	m_penTHL			.DeleteObject();
	m_penTSHL			.DeleteObject();
	m_penTHLE			.DeleteObject();
	m_penHighLimit		.DeleteObject();
	m_penLowLimit		.DeleteObject();
	m_penPivot2Rst		.DeleteObject();
	m_penPivot1Rst		.DeleteObject();
	m_penPivotGLine		.DeleteObject();
	m_penPivot1GG		.DeleteObject();
	m_penPivot2GG		.DeleteObject();
	m_penDMKPreHPrice	.DeleteObject();
	m_penDMKGLine		.DeleteObject();
	m_penDMKPreLPrice	.DeleteObject();
	m_pen2Pyung1		.DeleteObject();
	m_pen2Pyung2		.DeleteObject();
	m_pen2Pyung3		.DeleteObject();
	m_pen2Pyung4		.DeleteObject();
	m_penPmmDanga		.DeleteObject();
	m_penPmmPDanga		.DeleteObject();
	m_penPmmMDanga		.DeleteObject();
	m_penChegyul		.DeleteObject();		// ü�� �ð�ǥ��	�� 
	m_penSell			.DeleteObject();		// �ŵ� ü�� ǥ��	��
	m_penBuy			.DeleteObject();		// �ż� ü�� ǥ��	��
	//================================================================	
}

void CPatternMaker::Init_GuideLineValue()
{		
	m_bDraw2Pyung1	= FALSE;			// �̵���� 1��° �׸��� ����..
	m_bDraw2Pyung2	= FALSE;			// �̵���� 2��° �׸��� ����..
	m_bDraw2Pyung3	= FALSE;			// �̵���� 3��° �׸��� ����..
	m_bDraw2Pyung4	= FALSE;			// �̵���� 4��° �׸��� ����..

	m_dYSprice		= 0 ;	//���ϰ��� �ð�
	m_dYHprice		= 0 ;	//���ϰ��� ��
	m_dYLprice		= 0 ;	//���ϰ��� ����
	m_dYEprice		= 0 ;	//���ϰ��� ����
	m_dYSHL			= 0 ;	//���ϰ��� (��+��+��)/3
	m_dTSprice		= 0 ;	//���ϰ��� �ð�
	m_dTHprice		= 0 ;	//���ϰ��� ��
	m_dTLprice		= 0 ;	//���ϰ��� ����
	m_dTEprice		= 0 ;	//���ϰ��� ����
	m_dTHL			= 0 ;	//���ϰ��� (��+��)/2
	m_dTSHL			= 0 ;	//���ϰ��� (��+��+��)/3
	m_dTHLE			= 0 ;	//���ϰ��� (��+��+(��*2))/4
	m_dHighLimit	= 0 ;	//���Ѱ�
	m_dLowLimit		= 0 ;	//���Ѱ�
	m_dPivot2Rst	= 0 ;	// �Ǻ� 2�� ����
	m_dPivot1Rst	= 0 ;	// �Ǻ� 1�� ����
	m_dPivotGLine	= 0 ;	// �Ǻ� ���ؼ�
	m_dPivot1GG		= 0 ;	// �Ǻ� 1�� ����
	m_dPivot2GG		= 0 ;	// �Ǻ� 2�� ����
	m_dDMKPreHPrice = 0 ;	// Demark �����
	m_dDMKGLine		= 0 ;	// Demark ���ؼ�
	m_dDMKPreLPrice = 0 ;	// Demark ��������

	m_dMAPeriod1	= 0 ;	//�̵���� 1��° ��
	m_dMAPeriod2	= 0 ;	//�̵���� 2��° ��
	m_dMAPeriod3	= 0 ;	//�̵���� 3��° ��
	m_dMAPeriod4	= 0 ;	//�̵���� 4��° ��

	m_dMASum1		= 0 ;
	m_dMASum2		= 0 ;
	m_dMASum3		= 0 ;
	m_dMASum4		= 0 ;

	m_dPmmDanga		= 0 ;	//��ո��Դܰ�
	m_dPmmPDanga	= 0 ;	//�ܰ� + n%
	m_dPmmMDanga	= 0 ;	//�ܰ� - n%	
}


void CPatternMaker::RecvRealCurPrice(CString sEnd)
{	

	m_dTEprice = atof(sEnd );
	
	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//���ϰ��� (��+��+(��*2))/4	

	m_dMAPeriod1 = (m_dTEprice + m_dMASum1) / ((double)g_clConfigSet.m_nDay2Pyung1);
	m_dMAPeriod2 = (m_dTEprice + m_dMASum2) / ((double)g_clConfigSet.m_nDay2Pyung2);
	m_dMAPeriod3 = (m_dTEprice + m_dMASum3) / ((double)g_clConfigSet.m_nDay2Pyung3);
	m_dMAPeriod4 = (m_dTEprice + m_dMASum4) / ((double)g_clConfigSet.m_nDay2Pyung4);
	

	
//	m_dMAPeriod1 = (m_dMAPeriod1 + m_dTEprice) / g_clConfigSet.m_nDay2Pyung1;
//	m_dMAPeriod2 = ((m_dMAPeriod2 * (g_clConfigSet.m_nDay2Pyung2 - 1)) + m_dTEprice) / g_clConfigSet.m_nDay2Pyung2;
//	m_dMAPeriod3 = ((m_dMAPeriod3 * (g_clConfigSet.m_nDay2Pyung3 - 1)) + m_dTEprice) / g_clConfigSet.m_nDay2Pyung3;
//	m_dMAPeriod4 = ((m_dMAPeriod4 * (g_clConfigSet.m_nDay2Pyung4 - 1)) + m_dTEprice) / g_clConfigSet.m_nDay2Pyung4;
	
	if(!g_clConfigSet.m_bChk2Pyung1) m_dMAPeriod1 = 0;
	if(!g_clConfigSet.m_bChk2Pyung2) m_dMAPeriod2 = 0;
	if(!g_clConfigSet.m_bChk2Pyung3) m_dMAPeriod3 = 0;
	if(!g_clConfigSet.m_bChk2Pyung4) m_dMAPeriod4 = 0;
}

void CPatternMaker::RecvRealHigh(CString sHigh)
{	
	m_dTHprice = atof(sHigh);
	
	m_dTHL			= (m_dTHprice + m_dTLprice) / 2;					//���ϰ��� (��+��)/2
	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//���ϰ��� (��+��+��)/3
	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//���ϰ��� (��+��+(��*2))/4	
}

void CPatternMaker::RecvRealOpen(CString sOpen)
{	
	m_dTSprice = atof(sOpen);
	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//���ϰ��� (��+��+��)/3
}

void CPatternMaker::RecvRealLow(CString sLow)
{	
	m_dTLprice = atof(sLow );
	
	m_dTHL			= (m_dTHprice + m_dTLprice) / 2;					//���ϰ��� (��+��)/2
	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//���ϰ��� (��+��+��)/3
	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//���ϰ��� (��+��+(��*2))/4	
}

void CPatternMaker::RecvRealJango(CString sMMDanga)
{
	if(sMMDanga.GetLength())
		m_dPmmDanga = atof(sMMDanga);
	else
	{		
		m_dPmmDanga		= 0;
		m_dPmmPDanga	= 0;
		m_dPmmMDanga	= 0;

		return;
	}

	if(m_dPmmDanga)
	{		
		m_dPmmPDanga = m_dPmmDanga + (abs(g_clConfigSet.m_nPmmPDanga) * m_dPmmDanga / 100);
		m_dPmmMDanga = m_dPmmDanga - (abs(g_clConfigSet.m_nPmmMDanga) * m_dPmmDanga / 100);
	}
}

//�̵� ��� ��
void CPatternMaker::SetMAInfo(D10418_O *pD10418)
{
	memcpy(&m_MAvgInfo, pD10418, sizeof(D10418_O));
	m_strCode.Format("%*.*s", sizeof(pD10418->sShcode), sizeof(pD10418->sShcode), pD10418->sShcode);
	m_strCode.Replace(" ", "");

	CString sTemp, sCurPrice;
	sCurPrice.Format("%*.*s", sizeof(pD10418->sCurPrice), sizeof(pD10418->sCurPrice), pD10418->sCurPrice);
	double dCurPrice = atof(sCurPrice); 
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung1)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum1), sizeof(pD10418->sSum1), pD10418->sSum1);
		m_dMASum1 = atof(sTemp);	
		m_dMAPeriod1 = (dCurPrice + m_dMASum1) / ((double)g_clConfigSet.m_nDay2Pyung1);
//@		sTemp.Format("SetMAInfo - 1 - Sum : %f, Avg : %f ", m_dMASum1, m_dMAPeriod1);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod1 = 0;
		m_dMASum1 = 0;
//@		sTemp.Format("SetMAInfo - 1 - Sum : %f, Avg : %f ", m_dMASum1, m_dMAPeriod1);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
//@	OutputDebugString("----------------------------------------------------------");
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung2)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum2), sizeof(pD10418->sSum2), pD10418->sSum2);
		m_dMASum2 = atof(sTemp);	
		m_dMAPeriod2 = (dCurPrice + m_dMASum2) / ((double)g_clConfigSet.m_nDay2Pyung2);
//@		sTemp.Format("SetMAInfo - 2 - Sum : %f, Avg : %f ", m_dMASum2, m_dMAPeriod2);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod2 = 0;
		m_dMASum2 = 0;
//@		sTemp.Format("SetMAInfo - 2 - Sum : %f, Avg : %f ", m_dMASum2, m_dMAPeriod2);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
//@	OutputDebugString("----------------------------------------------------------");
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung3)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum3), sizeof(pD10418->sSum3), pD10418->sSum3);
		m_dMASum3 = atof(sTemp);	
		m_dMAPeriod3 = (dCurPrice + m_dMASum3) / ((double)g_clConfigSet.m_nDay2Pyung3);
//@		sTemp.Format("SetMAInfo - 3 - Sum : %f, Avg : %f ", m_dMASum3, m_dMAPeriod3);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod3 = 0;
		m_dMASum3 = 0;
//@		sTemp.Format("SetMAInfo - 3 - Sum : %f, Avg : %f ", m_dMASum3, m_dMAPeriod3);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
//@	OutputDebugString("----------------------------------------------------------");
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung4)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum4), sizeof(pD10418->sSum4), pD10418->sSum4);
		m_dMASum4 = atof(sTemp);	
		m_dMAPeriod4 = (dCurPrice + m_dMASum4) / ((double)g_clConfigSet.m_nDay2Pyung4);
//@		sTemp.Format("SetMAInfo - 4 - Sum : %f, Avg : %f ", m_dMASum4, m_dMAPeriod4);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod4 = 0;
		m_dMASum4 = 0;
//@		sTemp.Format("SetMAInfo - 4 - Sum : %f, Avg : %f ", m_dMASum4, m_dMAPeriod4);
//@		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
}

void CPatternMaker::GetPMMDanga()
{
	if(m_strCode.GetLength() < 6) 
	{
		m_dPmmDanga		= 0;
		m_dPmmPDanga	= 0;
		m_dPmmMDanga	= 0;

		return;
	}

	if( g_clConfigSet.m_bChkPmmDanga	||
		g_clConfigSet.m_bChkPmmPDanga	||
		g_clConfigSet.m_bChkPmmMDanga 
		)
	{
		HINSTANCE hCommDll = ::LoadLibrary(_T("Comm.DLL"));
		if(hCommDll)
		{					
			//Acct(11) + Name(20) + Pass(8)  : ��ü ���� ����Ʈ 
			pGetUnitPrice2 = (FpGetUnitPrice2)::GetProcAddress(hCommDll, "EX_GetUnitPrice");
			if(pGetUnitPrice2) 
			{
				LPCSTR szUintPrice = pGetUnitPrice2((LPTSTR)(LPCTSTR)g_clConfigSet.m_strAcct, (LPTSTR)(LPCTSTR)m_strCode);
				CString strPmmDanga;
				strPmmDanga.Format("%9.9s", szUintPrice);
				RecvRealJango(strPmmDanga);				
			}
			
			FreeLibrary(hCommDll);
		}
	}
}

void CPatternMaker::SetCode(CString strCode)
{
	m_strCode = strCode;

	m_nMarketType = 0;

	IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if(pDataManager == NULL)
		return;
	
	BYTE ucMarketType = pDataManager->GetMarketTypeFromCode(strCode);
	if(	//	ucMarketType == 1	||		// ���� ����
		//	ucMarketType == 2	||		// �ڽ��� ����
		//	ucMarketType == 3	||		// ��3����(OTCBB)
			ucMarketType == 4	||		// HIGHYIELD �ݵ�(��������)
			ucMarketType == 5	)		// �ֽĿ���Ʈ ELW
		//	ucMarketType == 6 )			// KDR
	{
		m_nMarketType = 0;
	}
	else if( ucMarketType == 26 ||		// �峻����
			 ucMarketType == 27 ||		// KOSPI 200 ����
			 ucMarketType == 28 ||		// KOSPI 100 ����
			 ucMarketType == 33 ||		// �ڽ��� ����
			 ucMarketType == 34 ||		// �ڽ���50 ����
			 ucMarketType == 36 ||		// �ڽ���STAR ����
			 ucMarketType == 59 ||		// ��Ÿ��������
			 ucMarketType == 60	||		// KRX
			 ucMarketType == 61	||		// KRX ��������
			 ucMarketType == 1	||
			 ucMarketType == 62	)		// �����������
	{
		m_nMarketType = 1;
	}
	else if( ucMarketType == 21	||		// KOFEX ��������
			 ucMarketType == 24 ||		// ����
			 ucMarketType == 25 ||		// �ɼ�
			 ucMarketType == 2	||
			 ucMarketType == 65 )		// �������弱��
	{
		m_nMarketType = 2;
	}
	else if( ucMarketType == 64 ||		// �ֽļ���
			 ucMarketType == 3	||
			 ucMarketType == 38 )		// �ֽĿɼ�
	{
		m_nMarketType = 3;
	}
	else if (ucMarketType == 6)			// ��ǰ����
	{
		m_nMarketType = 6;
	}

}

void CPatternMaker::SetChegyulList(D48112_OSUB *pD48112_OSub)
{
	DeleteCheChartData();

	CString sTemp;
	sTemp.Format("%*.*s", sizeof(pD48112_OSub->grdCnt), sizeof(pD48112_OSub->grdCnt), pD48112_OSub->grdCnt);
	int nCnt = atoi(sTemp); //��ü ü�� ����Ʈ ����.
	//if(nCnt > 0) m_bDrawChegyulLine = TRUE;
	m_bDrawChegyulLine = TRUE;//�Ｚ �����
	for(int i = 0 ; i < nCnt ; i++)
	{		
		D48112_OG* pChegyulData = (D48112_OG*)((pD48112_OSub->grid) + i);
		CCheChartData* pCheChartData = NULL;
		pCheChartData = new CCheChartData;
		if(pCheChartData)
		{
			sTemp.Format("%*.*s", sizeof(pChegyulData->jmcode), sizeof(pChegyulData->jmcode), pChegyulData->jmcode);
			sTemp.TrimLeft(); sTemp.TrimRight(); sTemp.Remove(' '); sTemp.Remove('A');
			pCheChartData->m_strJmCode = sTemp; //�����ڵ�

			sTemp.Format("%*.*s", sizeof(pChegyulData->chetime), sizeof(pChegyulData->chetime), pChegyulData->chetime);
			pCheChartData->m_strChetime = sTemp; //ü�� ��¥.

			sTemp.Format("%*.*s", sizeof(pChegyulData->chedanga), sizeof(pChegyulData->chedanga), pChegyulData->chedanga);
			pCheChartData->m_dChedanga = atof(sTemp); //ü�� �ܰ�.

			sTemp.Format("%*.*s", sizeof(pChegyulData->mesumedogb), sizeof(pChegyulData->mesumedogb), pChegyulData->mesumedogb);
			pCheChartData->m_nMesuMedoGb = atoi(sTemp);//�ŵ��ż�����


			m_aryCheChartData.Add(pCheChartData);
		}		
	}
}

void CPatternMaker::DeleteCheChartData()
{
	m_bDrawChegyulLine = FALSE;
	CObject *pObj=NULL;
	for(int i=0; i < m_aryCheChartData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryCheChartData.GetAt(i);
		delete pObj;
	}

	m_aryCheChartData.RemoveAll();
}



void CPatternMaker::ReadOneLineForSamgungDemo()
{
//@	OutputDebugString("Read...file");

	FILE* pfile=fopen("chart.txt","r");
	if(pfile)
	{
	   m_ArrayFileChart.RemoveAll();

	   char szBuf[256];
	   memset(szBuf, 0x00, sizeof(GData));
	   while(fgets(szBuf,256,pfile))
	   {
			
			CString sTemp;
			sTemp.Format("%s", szBuf);
			sTemp.TrimLeft(); sTemp.TrimRight(); sTemp.Remove(' ');
			if(sTemp.GetLength())
			{				
//@				OutputDebugString(sTemp);
				GData *pData = (GData*)(LPTSTR)(LPCTSTR)sTemp;			
				m_ArrayFileChart.Add(sTemp);
			}
	   }
	   fclose(pfile);
	}
}

//{{ ���(����) ó�� ���
#define DR_MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

HRESULT CPatternMaker::CTrCommSite::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	if(dwTRDateLen==0)
		return 0;
	DR_MYMETHOD_PROLOGUE(CPatternMaker, TrCommSite)
	CString strTR = szTR;

	if(strTR == "n55008")
	{
		pThis->ReceiveSiseData(aTRData, dwTRDateLen);
	}
	return S_OK;
}

HRESULT CPatternMaker::CTrCommSite::StatusChange(int nState)
{
	return  S_OK;
}

HRESULT CPatternMaker::CTrCommSite::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return 1;
}

HRESULT CPatternMaker::CTrCommSite::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	DR_MYMETHOD_PROLOGUE(CPatternMaker, TrCommSite)
	return S_OK;
}

void CPatternMaker::RequestSiseData()
{
	CString sCode = m_strCode;
	if(sCode.GetLength()<1)
		return;

	IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if(pDataManager == NULL)
		return;
	
	m_strEU0609InRec1.Empty();
	EU_p0609_InRec1 eu0609InRec1;

	// �ʱ�ȭ
	memset(&eu0609InRec1, 0x20, sizeof(eu0609InRec1));

	char chGubun = '0';
	BYTE ucMarketType = pDataManager->GetMarketTypeFromCode(sCode);
	if(		ucMarketType == 1	||		// ���� ����
			ucMarketType == 2	||		// �ڽ��� ����
			ucMarketType == 3	||		// ��3����(OTCBB)
			ucMarketType == 4	||		// HIGHYIELD �ݵ�(��������)
			ucMarketType == 5	||		// �ֽĿ���Ʈ ELW
			ucMarketType == 6 )			// KDR
	{
		chGubun = '0';
		m_nKeySize = 6;
	}
	else if( ucMarketType == 26 ||		// �峻����
			 ucMarketType == 27 ||		// KOSPI 200 ����
			 ucMarketType == 28 ||		// KOSPI 100 ����
			 ucMarketType == 33 ||		// �ڽ��� ����
			 ucMarketType == 34 ||		// �ڽ���50 ����
			 ucMarketType == 36 ||		// �ڽ���STAR ����
			 ucMarketType == 59 ||		// ��Ÿ��������
			 ucMarketType == 60	||		// KRX
			 ucMarketType == 61	||		// KRX ��������
			 ucMarketType == 62	)		// �����������
	{
		chGubun = '1';
		m_nKeySize = 3;
	}
	else if( ucMarketType == 21	||		// KOFEX ��������
			 ucMarketType == 24 ||		// ����
			 ucMarketType == 25 ||		// �ɼ�
			 ucMarketType == 65 )		// �������弱��
	{
		chGubun = '2';
		m_nKeySize = 8;
	}
	else if( ucMarketType == 64 ||		// �ֽļ���
			 ucMarketType == 38 )		// �ֽĿɼ�
	{
		chGubun = '3';
		m_nKeySize = 8;
	}

	memcpy(eu0609InRec1.gubun, &chGubun, sizeof(eu0609InRec1.gubun));
	memcpy(eu0609InRec1.shcode, m_strCode, m_strCode.GetLength());

	CString strRecordInfo("0                                                                               ");
	CString strEU0609Info;
	char* szEU0609Info = strEU0609Info.GetBuffer(sizeof(eu0609InRec1));
	memcpy(szEU0609Info, (char*)&eu0609InRec1, sizeof(eu0609InRec1));
	strEU0609Info.ReleaseBuffer();
	CString strFidInfo("1               000001002003004005006007008009;");
	m_strEU0609InRec1 = strRecordInfo + strEU0609Info + strFidInfo;

	m_TRComm->SetTR("n55008");
	m_TRComm->Send2Server((LPVOID)(LPCTSTR)m_strEU0609InRec1, m_strEU0609InRec1.GetLength(), FALSE);
//@��������	
//	if(strType == "1" || strType == "3")
//	{
//		m_TRComm->SetTR("70000");
//		m_nKeySize = 6;
//		m_TRComm->AddMainKey(DI_CODE, sCode, C_DT_STRING, m_nKeySize);
//		m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// ������Ʈ Ÿ��
//		m_TRComm->AddDataKey(3004);//���Ͻð�
//		m_TRComm->AddDataKey(3005);//���ϰ�
//		m_TRComm->AddDataKey(3006);//��������
//		m_TRComm->AddDataKey(3003);//��������
//		m_TRComm->AddDataKey(15009);//�ð�
//		m_TRComm->AddDataKey(15010);//��
//		m_TRComm->AddDataKey(15011);//����
//		m_TRComm->AddDataKey(15001);//���簡
//		m_TRComm->AddDataKey(16117);//���Ѱ�
//		m_TRComm->AddDataKey(16118);//���Ѱ�
//	}
//	else if(strType == "5" || strType == "6")
//	{
//		m_TRComm->SetTR("80000");
//		m_nKeySize = 8;
//		m_TRComm->AddMainKey(DI_CODE, sCode, C_DT_STRING, m_nKeySize);
//		m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// ������Ʈ Ÿ��
//		m_TRComm->AddDataKey(3004);//���Ͻð�
//		m_TRComm->AddDataKey(3005);//���ϰ�
//		m_TRComm->AddDataKey(3006);//��������
//		m_TRComm->AddDataKey(3003);//��������
//		m_TRComm->AddDataKey(15009);//�ð�
//		m_TRComm->AddDataKey(15010);//��
//		m_TRComm->AddDataKey(15011);//����
//		m_TRComm->AddDataKey(15001);//���簡
//		m_TRComm->AddDataKey(16117);//���Ѱ�
//		m_TRComm->AddDataKey(16118);//���Ѱ�
//	}
//	else
//		return;
//
//
//	m_TRComm->Send2Server("", 0, FALSE);				
//@��������	
}	

void CPatternMaker::ReceiveSiseData(LPVOID	aTRData, long dwTRDateLen)
{
	KB_d1017_OutRec1* pChartData = (KB_d1017_OutRec1*)aTRData;
	char* szTemp = new char[10 + 1];
	
	if(m_nKeySize == 6 || m_nKeySize == 9)		// �ֽ� or �ֽļ���
	{
		// ���ϰ���
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->preopen, sizeof(pChartData->preopen));
		m_dYSprice		= atof(szTemp); 		// ���ϰ��� �ð�
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->prehigh, sizeof(pChartData->prehigh));
		m_dYHprice		= atof(szTemp); 		// ���ϰ��� �� 
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->prelow, sizeof(pChartData->prelow));
		m_dYLprice		= atof(szTemp); 		// ���ϰ��� ���� 
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->preclose, sizeof(pChartData->preclose));
		m_dYEprice		= atof(szTemp); 		// ���ϰ��� ���� 
		
		// ���ϰ��� 
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->open, sizeof(pChartData->open));
		m_dTSprice		= atof(szTemp);  		// ���ϰ��� �ð�
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->high, sizeof(pChartData->high));
		m_dTHprice		= atof(szTemp);  		// ���ϰ��� ��
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->low, sizeof(pChartData->low));
		m_dTLprice		= atof(szTemp);  		// ���ϰ��� ����
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->close, sizeof(pChartData->close));
		m_dTEprice		= atof(szTemp);  		// ���ϰ��� ����
		
		// ���Ѱ�
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->hprice, sizeof(pChartData->hprice));
		m_dHighLimit		= atof(szTemp); 
		
		// ���Ѱ�
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->lprice, sizeof(pChartData->lprice));
		m_dLowLimit		= atof(szTemp); 
	}
	else if(m_nKeySize == 3 || m_nKeySize == 8)		// ���� or ����
	{
		// ���ϰ���
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->preopen, sizeof(pChartData->preopen));
		m_dYSprice		= atof(szTemp)/100.; 		// ���ϰ��� �ð�
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->prehigh, sizeof(pChartData->prehigh));
		m_dYHprice		= atof(szTemp)/100.; 		// ���ϰ��� �� 
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->prelow, sizeof(pChartData->prelow));
		m_dYLprice		= atof(szTemp)/100.; 		// ���ϰ��� ���� 
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->preclose, sizeof(pChartData->preclose));
		m_dYEprice		= atof(szTemp)/100.; 		// ���ϰ��� ���� 
		
		// ���ϰ��� 
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->open, sizeof(pChartData->open));
		m_dTSprice		= atof(szTemp)/100.;  		// ���ϰ��� �ð�
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->high, sizeof(pChartData->high));
		m_dTHprice		= atof(szTemp)/100.;  		// ���ϰ��� ��
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->low, sizeof(pChartData->low));
		m_dTLprice		= atof(szTemp)/100.;  		// ���ϰ��� ����
		
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->close, sizeof(pChartData->close));
		m_dTEprice		= atof(szTemp)/100.;  		// ���ϰ��� ����
		
		// ���Ѱ�
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->hprice, sizeof(pChartData->hprice));
		m_dHighLimit	= atof(szTemp)/100.; 
		
		// ���Ѱ�
		::memset(szTemp, 0x00, 10 + 1);
		::memcpy(szTemp, pChartData->lprice, sizeof(pChartData->lprice));
		m_dLowLimit		= atof(szTemp)/100.; 
	}

	m_dYSHL			= (m_dYSprice + m_dYHprice + m_dYLprice) / 3;	//���ϰ��� (��+��+��)/3

	//Pivot===============================================================================
	m_dPivotGLine = (m_dYEprice + m_dYHprice + m_dYLprice ) / 3;
	//--> 0016752: [7111] �������ٿ��� ���ؼ� ����â�� Pivot - 1������ / 2������ ���� �ݴ�� �Ǿ� �ִ� ����
	m_dPivot1Rst = m_dPivotGLine * 2 - m_dYLprice;
	m_dPivot2Rst = m_dPivotGLine + m_dYHprice - m_dYLprice;
	//<-- 
	m_dPivot1GG = m_dPivotGLine * 2 - m_dYHprice;	
	m_dPivot2GG = m_dPivotGLine - m_dYHprice + m_dYLprice;	

	//Demark==============================================================================
	if (m_dYSprice > m_dYEprice)
		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYLprice) / 2;
	else if (m_dYSprice < m_dYEprice)
		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYHprice) / 2;
	else
		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYEprice) / 2;

	m_dDMKPreHPrice = m_dDMKGLine - m_dYLprice;
	m_dDMKPreLPrice = m_dDMKGLine - m_dYHprice;
	m_dDMKGLine = m_dDMKGLine / 2;

//>> [QC No]090805HMC085
	m_dTHL			= (m_dTHprice + m_dTLprice) / 2;					//���ϰ��� (��+��)/2
	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//���ϰ��� (��+��+��)/3
	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//���ϰ��� (��+��+(��*2))/4
//<< [QC No]090805HMC085

	RefreshChart();
	//CString strCount, strOut, strData;
	//int nCount;
	////�ɼ�Ű
	//strCount.Format("%d", dwTRDateLen);
	//nCount = atoi(strCount);
	//strCount.Format("    %.4d", nCount);

	////������ 
	//CHARVECTOR2* pData = (CHARVECTOR2*)aTRData;
	////������
	//int nLen[] = {10, 10, 10, 10};		// {�ð�,��,����,����}
	//int nRow=0, nCol=0;
	//if(dwTRDateLen == 2)
	//{
	//		
	//	//���ϰ��� 
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol++;	
	//	m_dTSprice		= atof(strData); 
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol++;	
	//	m_dTHprice		= atof(strData); 
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol++;	
	//	m_dTLprice		= atof(strData); 
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol = 0;
	//	m_dTEprice		= atof(strData); 

	//	m_dTHL			= (m_dTHprice + m_dTLprice) / 2;					//���ϰ��� (��+��)/2
	//	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//���ϰ��� (��+��+��)/3
	//	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//���ϰ��� (��+��+(��*2))/4
	//	
	//	//���ϰ���
	//	nRow = 1;
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol++;	
	//	m_dYSprice		= atof(strData); 		//���ϰ��� �ð�		
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol++;	
	//	m_dYHprice		= atof(strData); 		//���ϰ��� ��
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol++;	
	//	m_dYLprice		= atof(strData);		//���ϰ��� ����
	//	strData.Format("%-10.10s", (*pData)[nRow][nCol]); nCol++;	
	//	m_dYEprice		= atof(strData); 		//���ϰ��� ����

	//	m_dYSHL			= (m_dYSprice + m_dYHprice + m_dYLprice) / 3;	//���ϰ��� (��+��+��)/3

	//	//Pivot===============================================================================
	//	m_dPivotGLine = (m_dYEprice + m_dYHprice + m_dYLprice ) / 3;
	//	m_dPivot2Rst = m_dPivotGLine * 2 - m_dYLprice;
	//	m_dPivot1Rst = m_dPivotGLine + m_dYHprice - m_dYLprice;
	//	m_dPivot1GG = m_dPivotGLine * 2 - m_dYHprice;	
	//	m_dPivot2GG = m_dPivotGLine - m_dYHprice + m_dYLprice;	

	//	//Demark==============================================================================
	//	if (m_dYSprice > m_dYEprice)
	//		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYLprice) / 2;
	//	else if (m_dYSprice < m_dYEprice)
	//		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYHprice) / 2;
	//	else
	//		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYEprice) / 2;
	//	
	//	m_dDMKPreHPrice = m_dDMKGLine - m_dYLprice;
	//	m_dDMKPreLPrice = m_dDMKGLine - m_dYHprice;
	//	m_dDMKGLine = m_dDMKGLine / 2;

	//}

	////���Ѱ�, ���Ѱ�
	//CHARVECTOR2* pData2 = NULL;
	//pData2 = (CHARVECTOR2*)m_TRComm->GetFidIndexData(1);
	//int nCnt = pData2->size();
	//if(pData2 && nCnt>0)
	//{

	//	CHARVECTOR vec = (*pData2)[0];
	//	int nCnt2 = vec.size();
	//	if(nCnt2==2)
	//	{
	//		strData.Format("%-10.10s", (*pData2)[0][0]);
	//		m_dHighLimit		= atof(strData); 
	//		strData.Format("%-10.10s", (*pData2)[0][1]);
	//		m_dLowLimit		= atof(strData); 
	//	}
	//	else
	//	{
	//		m_dHighLimit		= 0; 
	//		m_dLowLimit		= 0; 
	//	}
	//}
	//RefreshChart();
}
//}}

// Set Pointer of IChartOCX 
void CPatternMaker::SetIChartOCX( IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager)
{
	m_p_pIChartManager	= p_pIChartManager;
	m_p_pIChartOCX		= p_pIChartOCX;
	m_p_pIPacketManager = p_pIPacketManager;

	DeleteLinePen();
	CreateLinePen();
}

// Refresh Chart for Pattern Data
void CPatternMaker::RefreshChart()
{
	m_p_pIChartOCX->InvalidateControl();
}

void CPatternMaker::MakeMAData( IChartManager *p_pIChartManager, IPacketManager *p_pIPacketManager, IChartOCX *p_pIChartOCX)
{
	// 0. ���� ȯ���� �˻��Ѵ�.
	if( !p_pIChartManager || !p_pIPacketManager || !p_pIChartOCX) return;

	// �ð����� �ڷ����ڸ� ���Ѵ� ---------------------------------------------->>
	IPacket *pIPacketTime, *pIPacketOpen, *pIPacketHigh, *pIPacketLow, *pIPacketClose;
	pIPacketTime = pIPacketOpen = pIPacketHigh = pIPacketLow = pIPacketClose = NULL;

	pIPacketTime	= p_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_DATE_TIME));
	pIPacketOpen	= p_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_OPEN));
	pIPacketHigh	= p_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_HIGH));
	pIPacketLow		= p_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_LOW));
	pIPacketClose	= p_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_CLOSE));

	if ( !pIPacketTime || !pIPacketOpen || !pIPacketHigh || !pIPacketLow || !pIPacketClose ) return;


	// �̵���� �ʱ�ȭ �� �ð����� ���� ----------------------------->>
	int nMAPeriod		= 5;//m_pPatternEnvData->m_nMAPeriod;		// �̵���� �Ⱓ
	int nMAFirstData	= nMAPeriod - 1;						// ù��° �̵����(���ű���)
	int nTotalDataCnt	= pIPacketTime->GetDataCount();
	int nMinDataCnt		= nTotalDataCnt - nMAPeriod;			// �̵���� �ּҰ���

	if (nMinDataCnt < 0)		return;					// ���� ��꿡 �ʿ��� �ּҵ����� ���� üũ


//	double *pdMa		= new double[nTotalDataCnt];	// �̵����
//	long   *plChangPoint= new long  [nTotalDataCnt];	// �ֿ��������,��������(��ȯ��)
	
//	double *pdOpen		= new double[nTotalDataCnt];	// �ð�
//	double *pdHigh		= new double[nTotalDataCnt];	// ��
//	double *pdLow		= new double[nTotalDataCnt];	// ����
//	double *pdClose		= new double[nTotalDataCnt];	// ����


//	int			nNum = 0;
//	for(nNum=0; nNum< nTotalDataCnt; nNum++)
//	{
//		pIPacketOpen->GetData(nNum, pdOpen[nNum]);
//		pIPacketHigh->GetData(nNum, pdHigh[nNum]);
//		pIPacketLow->GetData(nNum, pdLow[nNum]);
//		pIPacketClose->GetData(nNum, pdClose[nNum]);
//
//		pdMa[nNum]			= VALUE_ZERO;
//		plChangPoint[nNum]	= VALUE_ZERO;
//	}

//�̵� ��� ==========================================================================	
	double dTemp = 0, dValue = 0;

	// ù��° ������ �̵����------------------		
	CString strTest;

	if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung1)
	{
		m_bDraw2Pyung1 = TRUE;
		m_dMASum1 = 0;
		for(int i = nTotalDataCnt-2; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung1 <= i ; i--)
		{
			pIPacketClose->GetData(i, dValue);
			strTest.Format("KHD : %f", dValue);
//			OutputDebugString(strTest);
			m_dMASum1 += dValue;		// �ֱ� nMAPeriod�ϰ� ��
		}
		pIPacketClose->GetData(nTotalDataCnt-1, dValue); // ������ ���Ѵ�.  
		m_dMAPeriod1 = (dValue+m_dMASum1)/g_clConfigSet.m_nDay2Pyung1;
	}
	else 
		m_bDraw2Pyung1 = FALSE;

	
	// �ι�° ������ �̵����------------------	
	dTemp = 0;m_dMASum2 = 0;
	if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung2)
	{
		m_bDraw2Pyung2 = TRUE;
		for(int i = nTotalDataCnt-2; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung2 <= i; i--)
		{
			pIPacketClose->GetData(i, dValue);
			m_dMASum2 += dValue;		// �ֱ� nMAPeriod�ϰ� ��
		}
		pIPacketClose->GetData(nTotalDataCnt-1, dValue); // ������ ���Ѵ�.  
		m_dMAPeriod2 = (dValue+m_dMASum2)/g_clConfigSet.m_nDay2Pyung2;
	}
	else 
		m_bDraw2Pyung2 = FALSE;
	
	// ����° ������ �̵����------------------
	dTemp = 0;m_dMASum3 = 0;
	if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung3)
	{
		m_bDraw2Pyung3 = TRUE;
		for(int i = nTotalDataCnt-2; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung3 <= i; i--)
		{
			pIPacketClose->GetData(i, dValue);
			m_dMASum3 += dValue;		// �ֱ� nMAPeriod�ϰ� ��
		}
		pIPacketClose->GetData(nTotalDataCnt-1, dValue); // ������ ���Ѵ�.  
		m_dMAPeriod3 = (dValue+m_dMASum3)/g_clConfigSet.m_nDay2Pyung3;
	}
	else 
		m_bDraw2Pyung3 = FALSE;
	
	// �׹�° ������ �̵����------------------
	dTemp = 0;m_dMASum4 = 0;
	if(nTotalDataCnt >= g_clConfigSet.m_nDay2Pyung4)
	{
		m_bDraw2Pyung4 = TRUE;
		for(int i = nTotalDataCnt-2; nTotalDataCnt-g_clConfigSet.m_nDay2Pyung4 <= i; i--)
		{
			pIPacketClose->GetData(i, dValue);
			m_dMASum4 += dValue;		// �ֱ� nMAPeriod�ϰ� ��
		}
		pIPacketClose->GetData(nTotalDataCnt-1, dValue); // ������ ���Ѵ�.  
		m_dMAPeriod4 = (dValue+m_dMASum4)/g_clConfigSet.m_nDay2Pyung4;
	}
	else 
		m_bDraw2Pyung4 = FALSE;

	pIPacketTime->Release();
	pIPacketOpen->Release();
	pIPacketHigh->Release();
	pIPacketLow->Release();
	pIPacketClose->Release();

//	delete []pdMa;
//	delete []plChangPoint;
//	delete []pdOpen;
//	delete []pdHigh;
//	delete []pdLow;
//	delete []pdClose;
}

void CPatternMaker::UpdateRecvRealOpen(CString snewPrice)
{
	double dNewPrice = atof(snewPrice );
	snewPrice.Format("%f", dNewPrice);
	//m_dTSprice
	if(dNewPrice != m_dTSprice)
		RecvRealOpen(snewPrice);
}

void CPatternMaker::UpdateRecvRealHigh(CString snewPrice)
{
	double dNewPrice = atof(snewPrice );
	snewPrice.Format("%f", dNewPrice);
	if(dNewPrice>m_dTHprice)
		RecvRealHigh(snewPrice);
}

void CPatternMaker::UpdateRecvRealLow(CString snewPrice)
{
	double dNewPrice = atof(snewPrice );
	snewPrice.Format("%f", dNewPrice);
	if(dNewPrice<m_dTLprice)
		RecvRealLow(snewPrice);
}

void CPatternMaker::UpdateRecvRealCurPrice(CString snewPrice)
{
	double dNewPrice = atof(snewPrice );
	snewPrice.Format("%f", dNewPrice);
	if(dNewPrice!=m_dTEprice)
		RecvRealCurPrice(snewPrice);
}


// ���ؼ� Line�� ǥ�� ��� �߰� �� �������� ��� �߰� - onlyojt
void CPatternMaker::DrawPriceText(CDC *pDC, const CRect& rcGraphRegion)
{
	int nTextWidth = 0, nYHigh;
	COLORREF clrTitle;
	CString strName;
	double dPrice;
	stGuideLineFormat stTGLF;
	POSITION posTGLF = m_listYGLF.GetHeadPosition();
	while( posTGLF)
	{
		stTGLF = m_listYGLF.GetNext( posTGLF);
		
		nYHigh = stTGLF.m_nYHigh;
		clrTitle = stTGLF.m_clrPrice;
		strName = stTGLF.m_strName;
		dPrice = stTGLF.m_dPrice;


		CRect rcDraw;
		int nWidth = 0;
		int nShow = g_clConfigSet.m_nAlign;

		CString sTemp, sTitle;
		if (g_clConfigSet.m_bChkName)
		{
			if (g_clConfigSet.m_bChkValue)
			{
				if( 0 < m_nMarketType && 3 != m_nMarketType)
						sTemp.Format("%.02f", dPrice);
				else	sTemp.Format("%.0f", dPrice);
				sTitle = strName + ":" + SetComma(sTemp);
			}
			else
				sTitle = strName;
		}
		else
		{
			if (g_clConfigSet.m_bChkValue)
			{
				if( 0 < m_nMarketType && 3 != m_nMarketType)
						sTemp.Format("%.02f", dPrice);
				else	sTemp.Format("%.0f", dPrice);
				sTitle = SetComma(sTemp);
			}
			else
				return;
		}

		pDC->DrawText(sTitle, rcDraw, DT_CALCRECT);
		nWidth = rcDraw.Width();

		pDC->SetBkMode(TRANSPARENT);
		rcDraw = rcGraphRegion;
		if ((nShow == 0) || (nShow == 2))
		{
			rcDraw.bottom = nYHigh - 1;
			rcDraw.top = rcDraw.bottom - 14;

			if( rcDraw.left + nTextWidth + nWidth + 4 < rcDraw.right)
			{
				rcDraw.left = rcDraw.left + nTextWidth;
				rcDraw.right = rcDraw.left + nWidth + 4;
			}
			else
				rcDraw.right = rcDraw.left + nWidth + 4;

			rcDraw.DeflateRect(0, 1);
			pDC->SetTextColor(clrTitle);
			pDC->DrawText(sTitle, rcDraw, DT_CENTER | DT_VCENTER);
		}

		rcDraw = rcGraphRegion;
		if ((nShow == 1) || (nShow == 2))
		{
			rcDraw.bottom = nYHigh - 1;
			rcDraw.top = rcDraw.bottom - 14;

			if( rcDraw.right - nTextWidth - nWidth - 4 > rcDraw.left)
			{
				rcDraw.right = rcDraw.right - nTextWidth;
				rcDraw.left = rcDraw.right - nWidth - 4;
			}
			else
				rcDraw.left = rcDraw.right - nWidth - 4;

			rcDraw.DeflateRect(0, 1);
			pDC->SetTextColor(clrTitle);
			pDC->DrawText(sTitle, rcDraw, DT_CENTER | DT_VCENTER);
		}
		
		nTextWidth += (nWidth + 4);
	}

	m_listYGLF.RemoveAll();
}

void CPatternMaker::AddGLF(int nYHigh, COLORREF clrPrice, CString strName, double dPrice)
{
	stGuideLineFormat stGLF;

	stGLF.m_dPrice = dPrice;
	stGLF.m_strName = strName;
	stGLF.m_nYHigh = nYHigh;
	stGLF.m_clrPrice = clrPrice;
	
	POSITION pos = m_listYGLF.GetHeadPosition(), posOld = NULL;
	while( pos)
	{
		posOld = pos;
		if( dPrice >= m_listYGLF.GetNext( pos).m_dPrice)
		{
			pos = posOld;
			break;
		}
	}
	if( pos == NULL) m_listYGLF.AddTail( stGLF);
	else			 m_listYGLF.InsertBefore( pos, stGLF);
}


CString CPatternMaker::SetComma(CString strNumber, BOOL bShowSign)
{
	CString szRet=_T("");
	CString szTmp = strNumber;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	char	ch = NULL;
	int nLen = strlen(szTmp);
	for(int i=0;  i<nLen ; i++) {
		ch = szTmp[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	szTmp = szRet;

	if(szTmp.IsEmpty())	return "" ;
	if(szTmp[0]=='+' ||szTmp[0]=='-') {
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}

	szTmp.MakeReverse();
	nLen = szTmp.GetLength();	
	szRet.Empty();
	BOOL bIsFloat = FALSE;
	if(szTmp.Find('.')!=-1)
	{
		bIsFloat = TRUE;
	}
	int j = 0;
	for(int i=0;   i<nLen;   i++) 
	{
		if(szTmp[i]=='.') 	bIsFloat = FALSE;
		else
		{
			if(!bIsFloat)
			{
				if(j!=0 && !(j%3))	szRet += ',';
				j++;
			}
		}
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	if(bShowSign&&ch=='+'&&szRet!="0")
	{
		szRet += ch;
	}
	szRet.MakeReverse();
	return szRet;
}
