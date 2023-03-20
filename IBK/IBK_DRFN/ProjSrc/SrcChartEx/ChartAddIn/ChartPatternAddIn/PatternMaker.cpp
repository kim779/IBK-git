 // PatternMaker.cpp : implementation file
//

#include "stdafx.h"
#include "PatternMaker.h"

#include "PatternDefine.h"
#include "DlgAnalysis.h"								// for CDlgAnalysis
#include <math.h>										// for fabs
#include "Include/ColorTable.h"							// for Color
#include "./Include_AddIn/I423611/_IPacket.h"			// for IPacket
#include "./Include_AddIn/I423611/_IPacketManager.h"	// for IPacketManager
#include "./Include_AddIn/I423611/_IChartManager.h"		// for IChartManager
#include "./Include_AddIn/I423611/_IBlock.h"
#include "DlgHtml.h"

// 2008.02.20 by LYH >>
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.02.20 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPatternMaker
extern HINSTANCE g_hInstance;
CPatternMaker::CPatternMaker()
{
	m_pointPrev		= CPoint(0, 0);
	m_nSamePointCnt	= 0;
	m_bSelWavePtn	= FALSE;

	m_bLog			= FALSE;		// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	m_bReverse		= FALSE;		// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.

	m_dViewMin		= 0;			// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	m_dViewMax		= 0;			// View���� ��� Graph�� View�� Maximum Scale ���̴�.

	m_pDlgAnalysis	= NULL;			// Analysis Dlg

		
	m_pnHighLowLinePen.CreatePen(PS_SOLID,	2, COLOR_HIGHLOW_PTLINE);	// �ֿ���� ���� ������� ����
	m_pnMaPen.CreatePen(		 PS_SOLID,	2, COLOR_MALINE);			// �̵���� ����
	m_pnHighLinePen.CreatePen(	 PS_SOLID,	1, COLOR_HIGH_HORZLINE);	// �ֿ���� ������� ����
	m_pnLowLinePen.CreatePen(	 PS_SOLID,	1, COLOR_LOW_HORZLINE);		// �ֿ����� ������� ����

	m_pnPatternWave.CreatePen(	 PS_SOLID,	2, COLOR_PATTERNWAVE);			// Pattern Wave
	m_pnUpwardReverse.CreatePen( PS_SOLID,	2, COLOR_UPWARD_REVERSE);		// Pattern Upward Reverse
	m_pnDnwardReverse.CreatePen( PS_SOLID,	2, COLOR_DNWARD_REVERSE);		// Pattern Downward Reverse
	m_pnUpwardContinue.CreatePen(PS_SOLID,	2, COLOR_UPWARD_CONTINUE);		// Pattern Upward Continue
	m_pnDnwardContinue.CreatePen(PS_SOLID,	2, COLOR_DNWARD_CONTINUE);		// Pattern Downward Continue

	m_hParentWnd = NULL;

	m_bDrawAutoTrendLine = FALSE;
	m_bDrawAutoPattern = FALSE;
	m_bDrawCandlePattern = FALSE;

	m_strRQ = "DEFAULT";

// 2008.02.20 by LYH >>
	m_hOcxWnd = NULL;
// 2008.02.20 by LYH <<
}

CPatternMaker::~CPatternMaker()
{
	// Delete all pen that was created;
	DeleteAllPen();

	// ���ϰ��� ������ ��� ����
	DeleteAllData();
//	// Destory Analysis Dialog
//	if (m_pDlgAnalysis->GetSafeHwnd())
//	{
//		m_pDlgAnalysis->DestroyWindow();
//		delete m_pDlgAnalysis;
//		m_pDlgAnalysis = NULL;
//	} 
}

// Set Pointer of IChartOCX 
void CPatternMaker::SetIChartOCX( IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager)
{
	m_pIChartManager	= p_pIChartManager;
	m_pIChartOCX		= p_pIChartOCX;
	m_pIPacketManager	= p_pIPacketManager;
}

// Refresh Chart for Pattern Data
void CPatternMaker::RefreshChart()
{
	m_pIChartOCX->InvalidateControl();
}

// Draw Pattern
void CPatternMaker::OnDraw( CDC *p_pDC)
{
	// ���Ϻм�â�� ���� ���
	if( !m_pDlgAnalysis)	return;

	// Ư����Ʈ�� ��쿡�� ������ �ʵ��� �Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// Drawing�� ������ ��Ȳ���� Ȯ���Ѵ�.
	if( !m_pIChartManager || !m_pIChartOCX || !m_pIPacketManager) return;

	// (2004.07.05, ��¿�) Time Mark�� ���� �ٸ� ���񿡼��� ��ȿ�ϵ���, 
	//		Packet�� Type�� �����Ͽ�, ����� ��찡 �ƴϸ� ��� Draw�ϵ��� �Ѵ�.
// 2008.01.20 by LYH >>	
	//IPacket *pIPacket = m_pIPacketManager->GetPacket( m_strRQ, "�ڷ�����");
	IPacket *pIPacket = m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_DATE_TIME));
// 2008.01.20 by LYH <<	
	if( !pIPacket) return;

	try
	{
		// ��Ʈ�� �⺻���� --------------------------------------------------------------------->>
		int		nStartIndex, nEndIndex;					// �� Data View�� Start/End Index
		int		nR, nC;									// ������Ʈ�� ���Ե� Block�� Index
		CRect	rctBlockRegion, rctGraphRegion;			// Frame ����, Graph ����

		if ( !GetChartBasicData( nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion))
			throw this;
		// <<--------------------------------------------------------------------------------------
		
		int		nDataOnePage = nEndIndex - nStartIndex + 1;								// �� Data View�� Data ����
		double	dCandleWidth = double( rctGraphRegion.Width()) / double( nDataOnePage);	// ���� ���� ���Ѵ�


		// �ֿ���� ���� ������� ����
		CPen *pOldPen = NULL;
		pOldPen = p_pDC->SelectObject( &m_pnHighLowLinePen);

		//2008.10.27 by LYH >> ���� ���� ����� �׸��� ���� ����
		CRgn rgn;
		rgn.CreateRectRgn((int)rctBlockRegion.left, (int)rctBlockRegion.top, (int)rctBlockRegion.right, (int)rctBlockRegion.bottom);
		p_pDC->SelectClipRgn(&rgn);
		//2008.10.27 by LYH <<

		if(m_bDrawAutoPattern)
		{
			// �ֿ���������� ������ �ĵ���, �������� ����, ������������ �׸��� *******************************(1_ST)
			DrawWaveLine(p_pDC, nStartIndex, nEndIndex, dCandleWidth, rctGraphRegion);

			// �߼������� �׸��� ************************************************************************************(4_ST)
			DrawAutoPattern(p_pDC, nStartIndex, nEndIndex, dCandleWidth, rctGraphRegion);

			// �����̵������ �׸��� ****************************************************************************(2_ST)
			p_pDC->SelectObject( &m_pnMaPen);
			DrawMALine(p_pDC, nStartIndex, nEndIndex, dCandleWidth, rctGraphRegion);
		}

//		int nX1, nY1, nX2, nY2;
		// ����, �������� �׸��� ****************************************************************************(3_ST)
		if(m_bDrawAutoTrendLine)
		{
			//p_pDC->SelectObject( &m_pnHighLowLinePen);
			DrawAutoTrendLine(p_pDC, nStartIndex, nEndIndex, dCandleWidth, rctGraphRegion);
		}

		if(m_bDrawCandlePattern)
		{
			// ĵ�������� �׸��� ************************************************************************************(5_ST)
			DrawCandlePattern(p_pDC, nStartIndex, nEndIndex, dCandleWidth, rctGraphRegion);
		}
		//2008.10.27 by LYH >> ���� ���� ����� �׸��� ���� ����
		p_pDC->SelectClipRgn(NULL);
		//2008.10.27 by LYH <<

		// �ֿ���� ���� ������� ����
		p_pDC->SelectObject( pOldPen);
	}
	catch( CPatternMaker *)
	{
		pIPacket->Release();
		return;
	}

	pIPacket->Release();
}


void CPatternMaker::GetAllData_AboutPattern()
{
	// ���Ϻм�â�� ���� ���
	if( !m_pDlgAnalysis)	return;

	// Drawing�� ������ ��Ȳ���� Ȯ���Ѵ�.
	if( !m_pIChartManager || !m_pIChartOCX || !m_pIPacketManager) return;


	// �ð����� �ڷ����ڸ� ���Ѵ� ---------------------------------------------->>
	IPacket *pIPacketTime, *pIPacketOpen, *pIPacketHigh, *pIPacketLow, *pIPacketClose;
	pIPacketTime = pIPacketOpen = pIPacketHigh = pIPacketLow = pIPacketClose = NULL;

// 2008.01.20 by LYH >>	
//	pIPacketTime	= m_pIPacketManager->GetPacket( m_strRQ, "�ڷ�����");
//	pIPacketOpen	= m_pIPacketManager->GetPacket( m_strRQ, "�ð�");
//	pIPacketHigh	= m_pIPacketManager->GetPacket( m_strRQ, "��");
//	pIPacketLow		= m_pIPacketManager->GetPacket( m_strRQ, "����");
//	pIPacketClose	= m_pIPacketManager->GetPacket( m_strRQ, "����");
	pIPacketTime	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_DATE_TIME));
	pIPacketOpen	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_OPEN));
	pIPacketHigh	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_HIGH));
	pIPacketLow		= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_LOW));
	pIPacketClose	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_CLOSE));
// 2008.01.20 by LYH <<	

	if ( !pIPacketTime || !pIPacketOpen || !pIPacketHigh || !pIPacketLow || !pIPacketClose ) return;


	CList<double,double> *clistTime, *clistOpen, *clistHigh, *clistLow, *clistClose;
	clistTime = clistOpen = clistHigh = clistLow = clistClose = NULL;
		
	clistTime	= pIPacketTime->GetDataList();
	clistOpen	= pIPacketOpen->GetDataList();
	clistHigh	= pIPacketHigh->GetDataList();
	clistLow	= pIPacketLow->GetDataList();
	clistClose	= pIPacketClose->GetDataList();
	// <<--------------------------------------------------------------------------


	// �̵���� �ʱ�ȭ �� �ð����� ���� ----------------------------->>
	int nMAPeriod		= m_PatternEnvData.m_nMAPeriod;			// �̵���� �Ⱓ
	int nMAFirstData	= nMAPeriod - 1;						// ù��° �̵����(���ű���)
	int nTotalDataCnt	= clistTime->GetCount();
	int nMinDataCnt		= nTotalDataCnt - nMAPeriod;			// �̵���� �ּҰ���

	if (nMinDataCnt < 0)		return;					// ���� ��꿡 �ʿ��� �ּҵ����� ���� üũ


	double *pdMa		= new double[nTotalDataCnt];	// �̵����
	long   *plTurningPt	= new long  [nTotalDataCnt];	// �ֿ��������,��������(��ȯ��)
	
	double *pdTime		= new double[nTotalDataCnt];	// �ð�
	double *pdHigh		= new double[nTotalDataCnt];	// ��
	double *pdLow		= new double[nTotalDataCnt];	// ����
	double *pdClose		= new double[nTotalDataCnt];	// ����


	int			nNum = 0;
	POSITION	posTime, posHigh, posLow, posClose;
	
	for(posTime = clistTime->GetHeadPosition(),		posHigh  = clistHigh->GetHeadPosition(),
		posLow  = clistLow->GetHeadPosition(),		posClose = clistClose->GetHeadPosition(); 
		posTime && posHigh && posLow && posClose;)
	{
		if (posTime == NULL)	continue;

		pdTime[nNum]	= (double)clistTime->GetNext(posTime);
		pdHigh[nNum]	= (double)clistHigh->GetNext(posHigh);
		pdLow[nNum]		= (double)clistLow->GetNext(posLow);
		pdClose[nNum]	= (double)clistClose->GetNext(posClose);


		pdMa[nNum]			= ZERO;
		plTurningPt[nNum]	= ZERO;

		nNum++;
	}
	// <<-------------------------------------------------------------

	

	// �̵���� �� �����̵���� ���ϱ� ------------------------------------------>>
	CMAData* pMaData;						// �����̵����
	
	double dSCo  = 0;						// �����̵���� ���
	int	   nGap  = 1;
	double dTemp = 0;
	int i = 0;
	int k = 0;
	// ù��° ������ �̵����------------------
	for( i = 0; i < nMAPeriod; i++)
	{
		dTemp += pdClose[i];		// �ֱ� nMAPeriod �Ⱓ���� ��
	}
	pdMa[nMAFirstData] = dTemp/nMAPeriod;

	// DEBUG#######################	
	//	CString strOut;
	//	strOut.Format("(%d) %.2f  �ð�:%f",  nMAFirstData, pdMa[nMAFirstData], pdTime[nMAFirstData]);
	//	strOut = "\n�̵����  : " + strOut;
	//	OutputDebugString(strOut);
	//################################

	dSCo = 0;
	nGap = 1;
	for ( k = nMAPeriod; k > 0; k--)
	{
		dSCo += ( nMAPeriod - nGap) * pdClose[nMAFirstData-k+1];
		nGap += 2;
	}
	dSCo /= 2.;
		
	/*
	dSCo =(	(nMAPeriod-1) * pdClose[nMAFirstData-4] +
			(nMAPeriod-3) * pdClose[nMAFirstData-3] +
			(nMAPeriod-5) * pdClose[nMAFirstData-2] +
			(nMAPeriod-7) * pdClose[nMAFirstData-1] +
			(nMAPeriod-9) * pdClose[nMAFirstData  ]	) / 2.;
	*/

	// �����̵����
	pdMa[nMAFirstData] = pdMa[nMAFirstData] + (6*dSCo) / ((nMAPeriod+1)*nMAPeriod);
	pMaData = new CMAData;
	pMaData->m_nDataSeq	= nMAFirstData;
	pMaData->m_dMMa		= pdMa[nMAFirstData];
	m_aryMAData.Add(pMaData);

	// DEBUG######################	
	//	strOut.Format("(%d) %.2f  �ð�:%f",  nMAFirstData, pdMa[nMAFirstData], pdTime[nMAFirstData]);
	//	strOut = "\n�����̵����  : " + strOut;
	//	OutputDebugString(strOut);
	//###########################

	// ù��° ������ �̵����--------------------
	for (i = nMAPeriod; i < nTotalDataCnt; i++)
	{
		dTemp = dTemp + pdClose[i] - pdClose[i-nMAPeriod];
		pdMa[i] = dTemp/nMAPeriod;

		// DEBUG#########################
		//	strOut.Format("(%d) %.2f  �ð�:%f",  i, pdMa[i], pdTime[i]);
		//	strOut = "\n�̵����  : " + strOut;
		//	OutputDebugString(strOut);
		//###################################

		dSCo = 0;
		nGap = 1;
		for (k = nMAPeriod; k > 0; k--)
		{
			dSCo += ( nMAPeriod - nGap) * pdClose[i-k+1];
			nGap += 2;
		}
		dSCo /= 2.;		

		/*
		dSCo =(	(nMAPeriod-1) * pdClose[i-4] +
						(nMAPeriod-3) * pdClose[i-3] +
						(nMAPeriod-5) * pdClose[i-2] +
						(nMAPeriod-7) * pdClose[i-1] +
		 				(nMAPeriod-9) * pdClose[i  ] ) / 2.;	
		*/
		
		// �����̵����
		pdMa[i] = pdMa[i] + (6*dSCo) / ((nMAPeriod+1)*nMAPeriod);
		pMaData = new CMAData;
		pMaData->m_nDataSeq	= i;
		pMaData->m_dMMa		= pdMa[i];
		m_aryMAData.Add(pMaData);

		// DEBUG######################	
		//	strOut.Format("(%d) %.2f  �ð�:%f",  i, pdMa[i], pdTime[i]);
		//	strOut = "\n�����̵����  : " + strOut;
		// 	OutputDebugString(strOut);
		//###################################
	}
	// <<---------------------------------------------------------------------------



	CHighLowPtData *pHighLowPtData, *pHighLowPtTemp;
	int		nSeq, nDataMin, nPointPos;
	BOOL	bUsePoint;
	double  dMaxValue, dMinValue, dValueTemp;


	// ###########################################################
	//		�ڵ��߼��� �۵���(0: �����̵���չ�, 1:����ĵ��)
	// ###########################################################
	int nDrawMethod = m_PatternEnvData.m_nDrawMethod;

	// ***********************************************************************
	//	(0)	�����̵������ �̿��Ͽ� �ֿ���� �ֿ������� ���Ѵ�.
	// ***********************************************************************
	if ( nDrawMethod == 0)
	{
		// �̵���տ��� �϶���ȯ��, �����ȯ���� ���Ѵ� --------------------------->>
		// Ⱦ������ ���͸� ��뿩��
		BOOL bFilterSideWalkPeriod = m_PatternEnvData.m_bFilterSideWalkPeriod;

		for (i = nMAFirstData; i < nTotalDataCnt-2; i++)
		{
			// �ֿ��������
			if (pdMa[i] < pdMa[i+1])
			{
				if (pdMa[i+1] > pdMa[i+2])
				{
					plTurningPt[i+1] = CHANGINGPT_HIGHTOLOW;	// �ֿ��������(�϶���ȯ��)
				}
				// === Ⱦ������ ���͸� ===
				else if (pdMa[i+1] == pdMa[i+2] && bFilterSideWalkPeriod)
				{
					for (int k=(i+2); k < nTotalDataCnt-1; k++)
					{
						if (pdMa[k] == ZERO)
							continue;

						if (pdMa[k] > pdMa[k+1])
						{
							plTurningPt[k] = CHANGINGPT_HIGHTOLOW;	// �ֿ��������(�϶���ȯ��)
							break;
						}
						else if (pdMa[k] < pdMa[k+1])
						{
							break;
						}
						else // Ⱦ������
							continue;
					}
				}
			}
			// �ֿ���������
			else if (pdMa[i] > pdMa[i+1])
			{
				if (pdMa[i+1] < pdMa[i+2])
				{
					plTurningPt[i+1] = CHANGINGPT_LOWTOHIGH;	// �ֿ���������(�����ȯ��)
				}
				// === Ⱦ������ ���͸� ===
				else if (pdMa[i+1] == pdMa[i+2]  && bFilterSideWalkPeriod)
				{
					for (int k=(i+2); k < nTotalDataCnt-1; k++)
					{
						if (pdMa[k] == ZERO)
							continue;

						if (pdMa[k] < pdMa[k+1])
						{
							plTurningPt[k] = CHANGINGPT_LOWTOHIGH;	// �ֿ���������(�����ȯ��)
							break;
						}
						else if (pdMa[k] > pdMa[k+1])
						{
							break;
						}
						else // Ⱦ������
							continue;
					}
				}
			}
		}
		// <<------------------------------------------------------------------------


		// �ֿ����, �ֿ����� ������ �迭�� �߰�
		double	dHLPtAllowalbeError = m_PatternEnvData.m_dHLPtAllowalbeError;	// ����, ���� ����%(Default:0)

		for (i = nMAPeriod; i < nTotalDataCnt; i++)
		{
			if (plTurningPt[i] == CHANGINGPT_HIGHTOLOW)
			{
				// �ֿ�������� ���� ������ nMAPeriod �̳����� �ֿ������ ã�´�
				nSeq	  = i;
				nDataMin  = i-nMAPeriod;
				dMaxValue = pdHigh[i];
				nPointPos = m_aryHighLowPtData.GetSize()-1;
				bUsePoint = TRUE;

				pHighLowPtTemp = NULL;
				if ( nPointPos >= 0)
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(nPointPos);
				
				for (int k = i; k > nDataMin; k--)
				{
					// ���� �ֿ���� �Ǵ� �ֿ������� ������ Break
					if ( pHighLowPtTemp ) 
					{
						if (pHighLowPtTemp->m_nPointType == CHANGINGPT_HIGHTOLOW)
						{
							bUsePoint = FALSE;
							break;
						}
						else if (pHighLowPtTemp->m_nDataSeq == k)
							break;
					}
					
					if ( dMaxValue < pdHigh[k])
					{
						dMaxValue  = pdHigh[k];
						nSeq = k;
					}
				}

				// === ���͸� ===
				// ���� �������� ������ ������ ������ ������� �ʰ�, ���� ���� �����͵� ����
				if (pHighLowPtTemp)
				{
					if ( pHighLowPtTemp->m_nPointType == CHANGINGPT_LOWTOHIGH)
					{
						// ���� �������� ������ ���������% ���� Ŀ����
						dValueTemp = pHighLowPtTemp->m_dLow + (pHighLowPtTemp->m_dLow * dHLPtAllowalbeError / 100);

						if (dValueTemp >= dMaxValue)
						{
							bUsePoint = FALSE;
							
							delete pHighLowPtTemp;
							m_aryHighLowPtData.RemoveAt(nPointPos);
						}
					}
				}

				// ��������Ʈ�� �ֿ������ �ƴѰ�� �迭�� �߰�(�����ߺ�����)
				if ( bUsePoint )
				{
					pHighLowPtData = new CHighLowPtData;	// �ֿ���� ������ �߰�
					pHighLowPtData->m_nPointType	= CHANGINGPT_HIGHTOLOW;
					pHighLowPtData->m_nDataSeq		= nSeq;
					pHighLowPtData->m_dTime			= pdTime[nSeq];
					pHighLowPtData->m_dHigh			= pdHigh[nSeq];
					pHighLowPtData->m_dLow			= pdLow[nSeq];
					
					m_aryHighLowPtData.Add(pHighLowPtData);
				}

				// DEBUG#########################
				//	strOut.Format("(%d) %.2f  �ð�:%f",  nSeq, pdMa[nSeq], pdTime[nSeq]);
				//	strOut = "\n������ : " + strOut;
				//	OutputDebugString(strOut);
				//###########################
			}
			else if (plTurningPt[i] == CHANGINGPT_LOWTOHIGH)
			{
				// �ֿ��������� ���� �������� nMAPeriod �̳����� �ֿ������� ã�´�
				nSeq	  = i;
				nDataMin  = i-nMAPeriod;
				dMinValue = pdLow[i];
				nPointPos = m_aryHighLowPtData.GetSize()-1;
				bUsePoint = TRUE;

				pHighLowPtTemp = NULL;
				if ( nPointPos >= 0)
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(nPointPos);

				for (int k = i; k > nDataMin; k--)
				{
					// ���� �ֿ���� �Ǵ� �ֿ������� ������ Break
					if ( pHighLowPtTemp ) 
					{
						if (pHighLowPtTemp->m_nPointType == CHANGINGPT_LOWTOHIGH)
						{
							bUsePoint = FALSE;
							break;
						}
						else if (pHighLowPtTemp->m_nDataSeq == k)
							break;
					}

					if ( dMinValue > pdLow[k])
					{
						dMinValue  = pdLow[k];
						nSeq = k;
					}
				}

				// === ���͸� ===
				// ���� �������� ������ ������ ������ ������� �ʰ�, ���� ���� �����͵� ����
				if (pHighLowPtTemp)
				{
					if ( pHighLowPtTemp->m_nPointType == CHANGINGPT_HIGHTOLOW)
					{
						// ���� �������� ������ ���������% ���� �۾ƾ���
						dValueTemp = pHighLowPtTemp->m_dHigh - (pHighLowPtTemp->m_dHigh * dHLPtAllowalbeError / 100);

						if (dValueTemp <= dMinValue)
						{
							bUsePoint = FALSE;
							
							delete pHighLowPtTemp;
							m_aryHighLowPtData.RemoveAt(nPointPos);
						}
					}
				}
				
				// ��������Ʈ�� �ֿ������� �ƴѰ�� �迭�� �߰�(�����ߺ�����)
				if ( bUsePoint )
				{
					pHighLowPtData = new CHighLowPtData;	// �ֿ����� ������ �߰�
					pHighLowPtData->m_nPointType	= CHANGINGPT_LOWTOHIGH;
					pHighLowPtData->m_nDataSeq		= nSeq;
					pHighLowPtData->m_dTime			= pdTime[nSeq];
					pHighLowPtData->m_dHigh			= pdHigh[nSeq];
					pHighLowPtData->m_dLow			= pdLow[nSeq];
					
					m_aryHighLowPtData.Add(pHighLowPtData);
				}
				
				// DEBUG#########################
				//	strOut.Format("(%d) %.2f  �ð�:%f",  nSeq, pdMa[nSeq], pdTime[nSeq]);
				//	strOut = "\n�������� : " + strOut;
				//	OutputDebugString(strOut);
				//###########################
			}
		}

		// ������ ������ �ֿ���� �Ǵ� �ֿ��������� ��� ------------------------>>
		nPointPos = m_aryHighLowPtData.GetSize()-1;
		pHighLowPtTemp = NULL;
		if ( nPointPos >= 0)
		{
			int nPtType;
			pHighLowPtTemp = m_aryHighLowPtData.GetAt(nPointPos);

			if ( pHighLowPtTemp->m_nPointType == CHANGINGPT_LOWTOHIGH)
			{
				// ���� �������� ������ ���������% ���� Ŀ����
				dValueTemp = pHighLowPtTemp->m_dLow + (pHighLowPtTemp->m_dLow * dHLPtAllowalbeError / 100);

				if (dValueTemp >= pdClose[nTotalDataCnt-1])
				{
					delete pHighLowPtTemp;
					m_aryHighLowPtData.RemoveAt(nPointPos);

					nPtType = CHANGINGPT_LOWTOHIGH;
				}
				else
				{
					nPtType = CHANGINGPT_HIGHTOLOW;
				}
			}
			else
			{
				// ���� �������� ������ ���������% ���� �۾ƾ���
				dValueTemp = pHighLowPtTemp->m_dHigh - (pHighLowPtTemp->m_dHigh * dHLPtAllowalbeError / 100);

				if (dValueTemp <= pdClose[nTotalDataCnt-1])
				{
					delete pHighLowPtTemp;
					m_aryHighLowPtData.RemoveAt(nPointPos);

					nPtType = CHANGINGPT_HIGHTOLOW;
				}
				else
				{
					nPtType = CHANGINGPT_LOWTOHIGH;
				}
			}		

			pHighLowPtData = new CHighLowPtData;
			pHighLowPtData->m_nPointType	= nPtType;
			pHighLowPtData->m_nDataSeq		= nTotalDataCnt-1;
			pHighLowPtData->m_dTime			= pdTime[nTotalDataCnt-1];
			pHighLowPtData->m_dHigh			= pdClose[nTotalDataCnt-1];	// �������
			pHighLowPtData->m_dLow			= pdClose[nTotalDataCnt-1];	// �������
			
			m_aryHighLowPtData.Add(pHighLowPtData);
		}
		// <<----------------------------------------------------------------------
	}
	// ***********************************************************************
	//	(1) ���� �������� �ֿ���� �ֿ������� ���Ѵ�.
	// ***********************************************************************
	else
	{
		int		nBaseCandleCnt, nBaseCandleAfterCnt, nMinimunCnt, nContiCnt;
		BOOL	bCondOK, bFirstConti;

		nBaseCandleCnt = m_PatternEnvData.m_nCandleCnt;		// ���� ĵ�� ����
		nBaseCandleAfterCnt = m_PatternEnvData.m_nCandleAfterCnt;		// �� ĵ�� ����
		nMinimunCnt	= nBaseCandleCnt+nBaseCandleAfterCnt+1;					// �ʿ��� �ּҺ��� ����
		
		for (i = 0; i < nTotalDataCnt-nMinimunCnt; i++)
		{
			// ***** �ֿ������ ã�´� *****
			bCondOK = TRUE;
			for ( k = i; k < i+nBaseCandleCnt; k++)
			{
				if ( pdHigh[k] < pdHigh[i+nBaseCandleCnt] )
				{
					continue;
				}
				else
				{
					bCondOK = FALSE;
					break;
				}
			}

			if ( bCondOK)
			{
				nContiCnt	= 0;		// Ⱦ������ ����
				bFirstConti = TRUE;
				for ( k = i+nBaseCandleCnt+1; k < i+nMinimunCnt+nContiCnt; k++)
				{
					// ���Ӱ����� ���(Ⱦ������) (** �ҿ��Ӱ��� ���͸� **)
					if ( pdHigh[k] == pdHigh[i+nBaseCandleCnt+nContiCnt] )
					{
						nContiCnt++;
						
						if ( bFirstConti)
							continue;
						else
							break;
					}

					if ( pdHigh[k] < pdHigh[i+nBaseCandleCnt+nContiCnt])
					{
						bFirstConti = FALSE;

						if ( k == (i+nMinimunCnt+nContiCnt-1) )
						{
							plTurningPt[i+nBaseCandleCnt+nContiCnt] |= CANDLE_HIGHPT;
						}
					}
					else
						break;
				}
			}

			// ***** �ֿ������� ã�´� *****
			bCondOK = TRUE;
			for ( k = i; k < i+nBaseCandleCnt; k++)
			{
				if ( pdLow[k] > pdLow[i+nBaseCandleCnt] )
				{
					continue;
				}
				else
				{
					bCondOK = FALSE;
					break;
				}
			}

			if ( bCondOK)
			{
				nContiCnt	= 0;		// Ⱦ������ ����
				bFirstConti = TRUE;
				for ( k = i+nBaseCandleCnt+1; k < i+nMinimunCnt+nContiCnt; k++)
				{
					// ���������� ���(Ⱦ������) (** �ҿ������� ���͸� **)
					if ( pdLow[k] == pdLow[i+nBaseCandleCnt+nContiCnt] )
					{
						if ( bFirstConti)
							continue;
						else
							break;
					}

					if ( pdLow[k] > pdLow[i+nBaseCandleCnt+nContiCnt])
					{
						bFirstConti = FALSE;

						if ( k == (i+nMinimunCnt+nContiCnt-1) )
						{
							plTurningPt[i+nBaseCandleCnt+nContiCnt] |= CANDLE_LOWPT;
						}
					}
					else
						break;
				}
			}
		}


		// �ֿ����, �ֿ����� ������ �迭�� �߰�
		for (i = 0; i < nTotalDataCnt-nBaseCandleCnt-1; i++)
		{
			if ( (plTurningPt[i] & CANDLE_HIGHPT) && (plTurningPt[i] & CANDLE_LOWPT))
			{
				plTurningPt[i] = ZERO;
				continue;
			}
			else if ( (plTurningPt[i] & CANDLE_HIGHPT))
			{
				nPointPos = m_aryHighLowPtData.GetSize()-1;

				pHighLowPtTemp = NULL;
				if ( nPointPos >= 0)
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(nPointPos);
				
				if (pHighLowPtTemp)
				{
					// ���ӵ� �ֿ������ ��� �ֿ���� ���̿� ���������� �߰�
					if ( pHighLowPtTemp->m_nPointType == CHANGINGPT_HIGHTOLOW)
					{
						nSeq = i-1;
						dMinValue = pdLow[i-1];
						bUsePoint = TRUE;

						for ( k = i-1; k > pHighLowPtTemp->m_nDataSeq; k--)
						{
							if (plTurningPt[k] & CANDLE_HIGHPT)
							{
								bUsePoint = FALSE;
								break;
							}
							
							if ( dMinValue > pdLow[k])
							{
								dMinValue  = pdLow[k];
								nSeq = k;
							}
						}

						if ( bUsePoint)
						{
							pHighLowPtData = new CHighLowPtData;	// �ֿ����� ������ �߰�
							pHighLowPtData->m_nPointType	= CHANGINGPT_LOWTOHIGH;
							pHighLowPtData->m_nDataSeq		= nSeq;
							pHighLowPtData->m_dTime			= pdTime[nSeq];
							pHighLowPtData->m_dHigh			= pdHigh[nSeq];
							pHighLowPtData->m_dLow			= pdLow[nSeq];

							m_aryHighLowPtData.Add(pHighLowPtData);
						}
					}
				}

				pHighLowPtData = new CHighLowPtData;	// �ֿ���� ������ �߰�
				pHighLowPtData->m_nPointType	= CHANGINGPT_HIGHTOLOW;
				pHighLowPtData->m_nDataSeq		= i;
				pHighLowPtData->m_dTime			= pdTime[i];
				pHighLowPtData->m_dHigh			= pdHigh[i];
				pHighLowPtData->m_dLow			= pdLow[i];

				m_aryHighLowPtData.Add(pHighLowPtData);
			}
			else if( (plTurningPt[i] & CANDLE_LOWPT))
			{
				nPointPos = m_aryHighLowPtData.GetSize()-1;

				pHighLowPtTemp = NULL;
				if ( nPointPos >= 0)
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(nPointPos);
				
				if (pHighLowPtTemp)		
				{
					// ���ӵ� �ֿ������� ��� �ֿ����� ���̿� ���������� �߰�
					if ( pHighLowPtTemp->m_nPointType == CHANGINGPT_LOWTOHIGH)
					{
						nSeq = i-1;
						dMaxValue = pdHigh[i-1];
						bUsePoint = TRUE;

						for ( k = i-1; k > pHighLowPtTemp->m_nDataSeq; k--)
						{
							if (plTurningPt[k] & CANDLE_LOWPT)
							{
								bUsePoint = FALSE;
								break;
							}
							
							if ( dMaxValue < pdHigh[k])
							{
								dMaxValue  = pdHigh[k];
								nSeq = k;
							}
						}

						if ( bUsePoint)
						{
							pHighLowPtData = new CHighLowPtData;	// �ֿ���� ������ �߰�
							pHighLowPtData->m_nPointType	= CHANGINGPT_HIGHTOLOW;
							pHighLowPtData->m_nDataSeq		= nSeq;
							pHighLowPtData->m_dTime			= pdTime[nSeq];
							pHighLowPtData->m_dHigh			= pdHigh[nSeq];
							pHighLowPtData->m_dLow			= pdLow[nSeq];

							m_aryHighLowPtData.Add(pHighLowPtData);
						}
					}
				}

				pHighLowPtData = new CHighLowPtData;	// �ֿ����� ������ �߰�
				pHighLowPtData->m_nPointType	= CHANGINGPT_LOWTOHIGH;
				pHighLowPtData->m_nDataSeq		= i;
				pHighLowPtData->m_dTime			= pdTime[i];
				pHighLowPtData->m_dHigh			= pdHigh[i];
				pHighLowPtData->m_dLow			= pdLow[i];

				m_aryHighLowPtData.Add(pHighLowPtData);
			}
		}
	}

	try
	{
		// ��Ʈ�� �⺻���� --------------------------------------------------------------------->>
		int		nStartIndex, nEndIndex;					// �� Data View�� Start/End Index
		int		nR, nC;									// ������Ʈ�� ���Ե� Block�� Index
		CRect	rctBlockRegion, rctGraphRegion;			// Frame ����, Graph ����

		if ( !GetChartBasicData( nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion))
			throw this;
		// <<--------------------------------------------------------------------------------------

		// �ֿ���� ���� �����Ͱ� ������ ����
		if (m_aryHighLowPtData.GetSize() == 0) throw this;

		// *************************************************************
		// ## Get Regist & Support Line
		// *************************************************************
		m_wpWavePattern.Get_RegistNSupportLine(		&m_aryHighLowPtData, &m_aryRegistNSupportData, m_PatternEnvData.m_dNextPtAllowableError);

		// *************************************************************
		// ## Find All Wave Pattern
		// *************************************************************
		m_wpWavePattern.Find_All_WavePattern(		&m_aryHighLowPtData, &m_aryWavePatternData);


		// Get Count of Pattern which was found
		int nFindPatternCnt[TOT_PATTERN_CNT] = {0, };
		GetFoundPatternCnt ( nFindPatternCnt);


		// *************************************************************
		// Statistics of the Wave Pattern Analysis
		// *************************************************************
		// Sorting by Time
		CWavePatternData *pWavePatternData1 = NULL;
		CWavePatternData *pWavePatternData2 = NULL;
		for ( int i = 0; i < m_aryWavePatternData.GetSize(); i++)
		{
			for ( int j = 0; j < m_aryWavePatternData.GetSize()-1; j++)
			{
				pWavePatternData1 = m_aryWavePatternData.GetAt(j);
				pWavePatternData2 = m_aryWavePatternData.GetAt(j+1);

				if ( pWavePatternData1->m_nEndSeq > pWavePatternData2->m_nEndSeq)
				{
					m_aryWavePatternData.SetAt(j+1,	pWavePatternData1);
					m_aryWavePatternData.SetAt(j,	pWavePatternData2);
				}
			}
		}


		// Set Find Pattern Count to Grid
		if ( m_pDlgAnalysis)
			m_pDlgAnalysis->SetFoundWavePtnToGrid_RptType1( &m_aryWavePatternData, pdTime, pdHigh, pdLow, nTotalDataCnt);

	}
	catch( CPatternMaker *)
	{
		pIPacketTime->Release();
		pIPacketOpen->Release();
		pIPacketHigh->Release();
		pIPacketLow->Release();
		pIPacketClose->Release();

		delete []pdMa;
		delete []plTurningPt;
		delete []pdTime;
		delete []pdHigh;
		delete []pdLow;
		delete []pdClose;

		return;
	}


	pIPacketTime->Release();
	pIPacketOpen->Release();
	pIPacketHigh->Release();
	pIPacketLow->Release();
	pIPacketClose->Release();

	delete []pdMa;
	delete []plTurningPt;
	delete []pdTime;
	delete []pdHigh;
	delete []pdLow;
	delete []pdClose;
}



// Destory Analysis Dialog
void CPatternMaker::DestroyAnalysisDialog()
{
	if (m_pDlgAnalysis->GetSafeHwnd())
	{
		m_pDlgAnalysis->DestroyWindow();
		delete m_pDlgAnalysis;
		m_pDlgAnalysis = NULL;
	}
}

void CPatternMaker::DeleteAllPen()
{
	// �ֿ���� ���� ������� ����
	if ( m_pnHighLowLinePen.GetSafeHandle())	m_pnHighLowLinePen.DeleteObject();

	// �̵���� ���� 
	if ( m_pnMaPen.GetSafeHandle())				m_pnMaPen.DeleteObject();

	// �ֿ���� ���� ����
	if ( m_pnHighLinePen.GetSafeHandle())		m_pnHighLinePen.DeleteObject();

	// �ֿ����� ���� ����
	if ( m_pnLowLinePen.GetSafeHandle())		m_pnLowLinePen.DeleteObject();

	// Delete Pen Pattern Wave 
	if ( m_pnPatternWave.GetSafeHandle())		m_pnPatternWave.DeleteObject();

	// Delete Pen Pattern Upward Reverse
	if ( m_pnUpwardReverse.GetSafeHandle())		m_pnUpwardReverse.DeleteObject();

	// Delete Pen Pattern Downward Reverse
	if ( m_pnDnwardReverse.GetSafeHandle())		m_pnDnwardReverse.DeleteObject();

	// Delete Pen Pattern Upward Continue
	if ( m_pnUpwardContinue.GetSafeHandle())	m_pnUpwardContinue.DeleteObject();

	// Delete Pen Pattern Downward Continue
	if ( m_pnDnwardContinue.GetSafeHandle())	m_pnDnwardContinue.DeleteObject();
}


void CPatternMaker::DeleteAllData()
{
	// �ֿ����, �ֿ����� ������ ����
	DeleteHighLowPtData();

	// �̵���� ������ ����
	DeleteMAData();

	// ����, ������ ������ ����
	DeleteRegistNSupportData();

	// Delete Wave Pattern Data
	DeleteWavePatternData();

	// Delete Candle Pattern Data
	DeleteCandlePatternData();

	// Delete Candle Pattern Position Info
	DeleteCandlePatternPosInfo();
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

void CPatternMaker::DeleteWavePatternData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryWavePatternData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryWavePatternData.GetAt(i);
		delete pObj;
	}

	m_aryWavePatternData.RemoveAll();
}



int CPatternMaker::GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion)
{
	//PAREA pArea;
	return m_pIChartManager->GetXPosition( m_strRQ, ( int)dDataOffset, 0);
/*
	int	nLeftBorder, nRightBorder, nDataOffset, nXPos, nAdd;
	double dDecimalFrac;

	nDataOffset = int(dDataOffset);
	dDecimalFrac = dDataOffset - nDataOffset;

	// ���� �°�踦 ���Ѵ�. (�� ���� �������� 1 Pixcel(����) �����̴�.)
	nLeftBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset) + 0.5) + 1;
	
	// ���� ���踦 ���Ѵ�. (���� �� ���� �°�躸�� 1 Pixcel(����) �����̴�.)
	nRightBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset + 1) + 0.5) - 1;

	// �Ҽ����� ���� �߰��� Pixel
	nAdd = int(dDecimalFrac * dCandleWidth);

	// ���� �߽��� ��´�. (�������� ����Ѵ�.)
	nXPos = ( nLeftBorder + nRightBorder) / 2 + nAdd;

	return nXPos;
*/
}


// **********************************************
// Get Count of Pattern which was found
// **********************************************
void CPatternMaker::GetFoundPatternCnt(int *p_nFindPatternCnt)
{
	CWavePatternData *pWavePatternData = NULL;
	for (int i = 0; i < m_aryWavePatternData.GetSize(); i++)
	{
		pWavePatternData = m_aryWavePatternData.GetAt(i);
		if ( pWavePatternData)
		{
			// Find Pattern Cnt;
			switch (pWavePatternData->m_nPatternType)
			{
			case PTN_HEADNSHOULDER:			// Head & Shoulder				(�Ӹ������)
				p_nFindPatternCnt[0]++;
				break;
			case PTN_R_HEADNSHOULDER:		// Reversal Head & Shoulder		(���Ӹ������)
				p_nFindPatternCnt[1]++;
				break;

			case PTN_DOUBLETOP:				// Double Top					(�ֺ���)
				p_nFindPatternCnt[2]++;
				break;
			case PTN_R_DOUBLETOP:			// Reversal Double Top			(���ֺ���)
				p_nFindPatternCnt[3]++;
				break;
				
			case PTN_BULL_ISOTRIANGLE:		// Bull Isosceles Triangles		(�����̵�ﰢ��)
				p_nFindPatternCnt[4]++;
				break;
			case PTN_BEAR_ISOTRIANGLE:		// Bear Isosceles Triangles		(�༼�̵�ﰢ��)
				p_nFindPatternCnt[5]++;
				break;
				
			case PTN_BULL_TRIANGLE:			// Bull Triangles				(��»ﰢ��)
				p_nFindPatternCnt[6]++;
				break;
			case PTN_BEAR_TRIANGLE:			// Bear Triangles				(�϶��ﰢ��)
				p_nFindPatternCnt[7]++;
				break;
				
			case PTN_BULLISH_WIDETRIANGLE:	// Bullish Wide Triangles		(���Ȯ��ﰢ��)
				p_nFindPatternCnt[8]++;
				break;
			case PTN_BEARISH_WIDETRIANGLE:	// Bearish Wide Triangles		(�϶�Ȯ��ﰢ��)
				p_nFindPatternCnt[9]++;
				break;
				
			case PTN_BULL_WEDGE:			// Bull Wedge					(���������)
				p_nFindPatternCnt[10]++;
				break;
			case PTN_BEAR_WEDGE:			// Bear Wedge					(���������)
				p_nFindPatternCnt[11]++;
				break;
				
			case PTN_BULL_SQUAREFLAG:		// Bull Square Flag				(��»簢�����)
				p_nFindPatternCnt[12]++;
				break;
			case PTN_BEAR_SQUAREFLAG:		// Bear Square Flag				(�϶��簢�����)
				p_nFindPatternCnt[13]++;
				break;
				
			case PTN_BULL_TRIANGLEFLAG:		// Bull Triangle Flag			(��»ﰢ�����)
				p_nFindPatternCnt[14]++;
				break;
			case PTN_BEAR_TRIANGLEFLAG:		// Bear Triangle Flag			(�϶��ﰢ�����)
				p_nFindPatternCnt[15]++;
				break;
				
			case PTN_BULL_RECTANGLE:		// Bull Rectangle				(������簢��)
				p_nFindPatternCnt[16]++;
				break;
			case PTN_BEAR_RECTANGLE:		// Bear Rectangle				(�϶����簢��)
				p_nFindPatternCnt[17]++;
				break;
				
			case PTN_V_TOP:					// V Top						(�ܺ�õ����)
				p_nFindPatternCnt[18]++;
				break;
			case PTN_V_BOTTOM:				// V Bottom						(�ܺ��ٴ���)
				p_nFindPatternCnt[19]++;
				break;
			}
		}
	}
}

// Get Chart Region Data
BOOL CPatternMaker::GetChartBasicData(	int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion)
{
	// �� Data View�� Start/End Index�� ���Ѵ�.
	//if( !m_pIChartOCX->GetViewStartEndIndex( p_nStartIndex, p_nEndIndex)) return FALSE;
	if( !m_pIChartManager->GetStartEndDrawIdx_Cur(m_strRQ, p_nStartIndex, p_nEndIndex)) return FALSE;

	//// ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	//if( !m_pIChartManager->GetBlockIndexOfGraph( "������Ʈ", p_nR, p_nC)) return FALSE;
	//
	//// Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	//m_pIChartManager->GetVerticalScaleConversionType( p_nR, p_nC, m_bLog, m_bReverse);

	//// View�� Min/Max ���� ���Ѵ�.
	//m_pIChartManager->GetMinMaxDataInViewWithGraph( "������Ʈ", m_dViewMin, m_dViewMax);

	//// Frame ����, Graph ������ ���Ѵ�.
	//p_rctBlockRegion = m_pIChartManager->GetNoFrameGraphRegion( p_nR, p_nC);		
	//p_rctGraphRegion = m_pIChartManager->GetDrawingGraphRegion( p_nR, p_nC);

	int nV, nG;
	//<<20100327_JS.Kim ������ �˼� ������... FindFirstIndicator ��� �� CSiteCustomAddin::m_lpszRQ�� �ٲ�� ���� �߻��Ͽ� �Լ� ��ü
//	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), p_nR, p_nC, nV, nG, m_strRQ);
//	if( szGraphName.IsEmpty()) return FALSE;
	if( !m_pIChartManager->GetIndexOfGraph( m_strRQ, _MTEXT( C2_PRICE_CHART), p_nR, p_nC, nV, nG)) 
		return FALSE;
	//>>

	IBlock *pIBlock = m_pIChartManager->GetBlock( p_nR, p_nC);
	if( pIBlock)
	{
		// 2.2.1 ������Ʈ�� Block�� Log/������ �������� Ȯ���Ѵ�.
		m_bLog = pIBlock->IsLogScale();
		m_bReverse = pIBlock->IsVScaleInvert( nV);
		// 2.2.0 View�� Min/Max ���� ���Ѵ�.
		pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
		pIBlock->GetGraphRegions(p_rctGraphRegion, p_rctBlockRegion );
		p_rctGraphRegion = pIBlock->GetGraphDrwingRegion();		
		pIBlock->Release();
		pIBlock = NULL;
	}
	return TRUE;
}


void CPatternMaker::CreateDlgAnalysis( IChartOCX *m_pIChartOCX, int nType, BOOL bPatternShow /*= TRUE*/)
{
	// (2004.10.19, ��¿�) Resource�� ������ �ߵǵ��� Module State�� �����Ѵ�.
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(nType == 1)
		m_bDrawAutoTrendLine = TRUE;
	else if(nType == 2)
		m_bDrawAutoPattern = TRUE;
	else if(nType == 3)
		m_bDrawCandlePattern = TRUE;
	else if(nType == 4)
		m_bDrawAutoTrendLine = FALSE;
	else if(nType == 5)
		m_bDrawAutoPattern = FALSE;
	else if(nType == 6)
		m_bDrawCandlePattern = FALSE;
	else if(nType == 7)
	{
		// if Analysis Dialog is in existence, then only show the dialog.
		if (m_pDlgAnalysis)
		{
//			if(bPatternShow)
			m_pDlgAnalysis->ShowWindow(SW_SHOW);
			return;
		}
	}

	if(!m_pDlgAnalysis)
	{
		CWnd *pWnd = NULL;
		pWnd = CWnd::FromHandle( m_pIChartOCX->GetOcxHwnd());
		m_hParentWnd = pWnd;

		CRect rcDlg;
		pWnd->GetWindowRect( &rcDlg);
		rcDlg.left += 20;
		rcDlg.top += 20;

		// Create Analysis Dialog
		ML_SET_LANGUAGE_RES();
		m_pDlgAnalysis = new CDlgAnalysis(pWnd, this);
		m_pDlgAnalysis->Create(IDD_DLG_ANALYSIS, pWnd);
		if(bPatternShow)
		{
			if(m_pDlgAnalysis)//KHD : �����Ÿ��� �ε� �ȵǴ� ���� ���� 0003234
				m_pDlgAnalysis->ShowWindow(SW_SHOW);
		}
	}
}


BOOL CPatternMaker::OnLButtonDown( UINT nFlags, CPoint &point)
{
	// Ŭ���� ��ġ�� ������ ���ٸ� ���������� ������ ã�´�.
	if ( m_pointPrev == point)
	{
		m_nSamePointCnt++;
	}
	else
	{
		m_pointPrev = point;
		m_nSamePointCnt = 0;
	}

	BOOL bRet = FindPtn_OnLBtnDownPos( nFlags, point, m_nSamePointCnt);
	if ( bRet)
	{
		m_bSelWavePtn	= TRUE;
	}
	else
	{
		m_pointPrev		= CPoint(0, 0);
		m_nSamePointCnt	= 0;
		m_bSelWavePtn	= FALSE;
	}

//	return !bRet;
	return FALSE;
}

BOOL CPatternMaker::OnLButtonUp( UINT nFlags, CPoint &point)
{
	ML_SET_LANGUAGE_RES();

	CCandlePatternPosInfo* pInfo = NULL;
	int nFindIdx = -1;
	for(int i=0; i<m_aryCandlePatternPosInfo.GetSize(); i++)
	{
		pInfo = m_aryCandlePatternPosInfo.GetAt(i);
		//		if ( (point.x > pInfo->m_rect.left && point.x < pInfo->m_rect.right) &&
		if ( (point.x > pInfo->m_rect.left && point.x < (pInfo->m_rect.left + 12)) &&
			(point.y > pInfo->m_rect.top  && point.y < pInfo->m_rect.bottom)	)
		{
			nFindIdx = i;
			break;
		}
	}

	if(nFindIdx >= 0)
	{
		CString strURL;
		// (2008/3/21 - Seung-Won, Bae) Support in only Korean Language.
		switch( g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd))
		{
			case IMetaTable::ENGLISH:
				//strURL.Format("%s\\data\\Candle\\e_%s.html", m_pIChartOCX->GetHtsPath(), m_cpCandlePattern.GetCandlePatternHelpName(pInfo->m_nCandlePtnType));
				strURL.Format("%s\\Icss\\Candle\\e_%s.html", m_pIChartOCX->GetHtsPath(), m_cpCandlePattern.GetCandlePatternHelpName(pInfo->m_nCandlePtnType));	//@Solomon:Directory
				break;
			case IMetaTable::KOREAN:
				//strURL.Format("%s\\data\\Candle\\%s.html", m_pIChartOCX->GetHtsPath(), m_cpCandlePattern.GetCandlePatternHelpName(pInfo->m_nCandlePtnType));
				strURL.Format("%s\\Icss\\Candle\\%s.html", m_pIChartOCX->GetHtsPath(), m_cpCandlePattern.GetCandlePatternHelpName(pInfo->m_nCandlePtnType));		//@Solomon:Directory
				break;
		}
		CFileFind ffind;
		if( !strURL.IsEmpty() && ffind.FindFile( strURL))
		{
			CDlgHtml dlghtml;
			dlghtml.SetURL(strURL);
			dlghtml.DoModal();
		}
		ffind.Close();
	}
// 2008.02.20 by LYH >>
	//AfxSetResourceHandle(hInstResourceClient);
// 2008.02.20 by LYH <<

	//	return !bRet;
	return FALSE;
}

BOOL CPatternMaker::FindPtn_OnLBtnDownPos( UINT nFlags, CPoint &point, int nSamePointCnt)
{
	// ���Ϻм�â�� ���� ���
	if( !m_pDlgAnalysis)	return FALSE;


	// Ư����Ʈ�� ��쿡�� ������ �ʵ��� �Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	if( m_pIChartOCX->IsOnSpecialChart()) return FALSE;

	// Drawing�� ������ ��Ȳ���� Ȯ���Ѵ�.
	if( !m_pIChartManager || !m_pIChartOCX) return FALSE;


	// ��Ʈ�� �⺻���� --------------------------------------------------------------------->>
	int		nStartIndex, nEndIndex;					// �� Data View�� Start/End Index
	int		nR, nC;									// ������Ʈ�� ���Ե� Block�� Index
	CRect	rctBlockRegion, rctGraphRegion;			// Frame ����, Graph ����

	if ( !GetChartBasicData( nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion))
		return FALSE;
	// <<--------------------------------------------------------------------------------------

	// LButton Down�� �׷����������� üũ ------------------------------------------->>
	if ( (point.x < rctBlockRegion.left || point.x > rctBlockRegion.right) ||
		 (point.y < rctBlockRegion.top  || point.y > rctBlockRegion.bottom)	)
		 return FALSE;
	//if ( (point.x < rctGraphRegion.left || point.x > rctGraphRegion.right) ||
	//	 (point.y < rctGraphRegion.top  || point.y > rctGraphRegion.bottom)	)
	//	 return FALSE;
	// <<------------------------------------------------------------------------------

	// Init Selected Pattern Data
	InitSelectedPatternData();


	int		nDataOnePage = nEndIndex - nStartIndex + 1;								// �� Data View�� Data ����
	double	dCandleWidth = double( rctGraphRegion.Width()) / double( nDataOnePage);	// ���� ���� ���Ѵ�

	// **********************************************
	// Ŭ���� �����Ϳ� �ִ� ������ ã�´� 
	// **********************************************
	int		nX1, nY1, nX2, nY2, nX3, nY3, nX4, nY4;
	int		nNeckXPos1, nNeckYPos1, nNeckXPos2, nNeckYPos2;
	int		nNeckYBase1, nNeckYBase2, nNeckYLength1, nNeckYLength2, nNeckXLength;
	int		nYHigh, nYLow, nYOut;
	BOOL	bIsDrawReady;
	CString	strPatternName;
	double	dDataOffset, dGradient;
	CWavePatternData *pWavePatternData = NULL;

	for ( int i = 0; i < m_aryWavePatternData.GetSize(); i++)
	{
		nX1 = nY1 = nX2 = nY2 = 0;
		nX3 = nY3 = nX4 = nY4 = 0;
		nNeckXPos1 = nNeckYPos1 = nNeckXPos2 = nNeckYPos2 = 0;
		nNeckYLength1 = nNeckYLength2 = nNeckXLength = 0;
		bIsDrawReady	= FALSE;
		strPatternName	= _T("");

		pWavePatternData = m_aryWavePatternData.GetAt(i);
		if ( !pWavePatternData)	continue;

		
		// Get X, Y Point for Drawing and Find Pattern
		switch (pWavePatternData->m_nPatternType)
		{
		case PTN_HEADNSHOULDER:			// Head & Shoulder				(�Ӹ������)
		case PTN_R_HEADNSHOULDER:		// Reversal Head & Shoulder		(���Ӹ������)
		case PTN_V_TOP:					// V Top						(�ܺ�õ����)
		case PTN_V_BOTTOM:				// V Bottom						(�ܺ��ٴ���)
			if ( (pWavePatternData->m_dDrawSeq1_1 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_1 <= nEndIndex) &&
				 (pWavePatternData->m_dDrawSeq1_2 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_2 <= nEndIndex)	)
			{
					// Point1
					dDataOffset = pWavePatternData->m_dDrawSeq1_1;
					nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY1 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_1, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					
					// Point2
					dDataOffset = pWavePatternData->m_dDrawSeq1_2;
					nX2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY2 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_2, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

					bIsDrawReady = TRUE;
			}
			break;

		case PTN_DOUBLETOP:				// Double Top					(�ֺ���)
		case PTN_R_DOUBLETOP:			// Reversal Double Top			(���ֺ���)
		case PTN_BULL_ISOTRIANGLE:		// Bull Isosceles Triangles		(�����̵�ﰢ��)
		case PTN_BEAR_ISOTRIANGLE:		// Bear Isosceles Triangles		(�༼�̵�ﰢ��)
		case PTN_BULL_TRIANGLE:			// Bull Triangles				(��»ﰢ��)
		case PTN_BEAR_TRIANGLE:			// Bear Triangles				(�϶��ﰢ��)
		case PTN_BULLISH_WIDETRIANGLE:	// Bullish Wide Triangles		(���Ȯ��ﰢ��)
		case PTN_BEARISH_WIDETRIANGLE:	// Bearish Wide Triangles		(�϶�Ȯ��ﰢ��)
		case PTN_BULL_WEDGE:			// Bull Wedge					(���������)
		case PTN_BEAR_WEDGE:			// Bear Wedge					(���������)
		case PTN_BULL_SQUAREFLAG:		// Bull Square Flag				(��»簢�����)
		case PTN_BEAR_SQUAREFLAG:		// Bear Square Flag				(�϶��簢�����)
		case PTN_BULL_TRIANGLEFLAG:		// Bull Triangle Flag			(��»ﰢ�����)
		case PTN_BEAR_TRIANGLEFLAG:		// Bear Triangle Flag			(�϶��ﰢ�����)
		case PTN_BULL_RECTANGLE:		// Bull Rectangle				(������簢��)
		case PTN_BEAR_RECTANGLE:		// Bear Rectangle				(�϶����簢��)
			if ( (pWavePatternData->m_dDrawSeq1_1 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_1 <= nEndIndex) &&
				 (pWavePatternData->m_dDrawSeq1_2 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_2 <= nEndIndex)	&& 
				 (pWavePatternData->m_dDrawSeq2_1 >= nStartIndex) && (pWavePatternData->m_dDrawSeq2_1 <= nEndIndex) &&
				 (pWavePatternData->m_dDrawSeq2_2 >= nStartIndex) && (pWavePatternData->m_dDrawSeq2_2 <= nEndIndex)	 )
			{
					// Point1
					dDataOffset = pWavePatternData->m_dDrawSeq1_1;
					nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY1 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_1, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					
					// Point2
					dDataOffset = pWavePatternData->m_dDrawSeq1_2;
					nX2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY2 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_2, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

					// Point3
					dDataOffset = pWavePatternData->m_dDrawSeq2_1;
					nX3 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY3 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt2_1, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					
					// Point4
					dDataOffset = pWavePatternData->m_dDrawSeq2_2;
					nX4 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY4 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt2_2, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

					bIsDrawReady = TRUE;
			}
			break;
		}

		if ( !bIsDrawReady)		continue;


		// Get Neck Point
		switch (pWavePatternData->m_nPatternType)
		{
		case PTN_HEADNSHOULDER:			// Head & Shoulder
		case PTN_R_HEADNSHOULDER:		// Reversal Head & Shoulder
		case PTN_DOUBLETOP:				// Double Top					(�ֺ���)
		case PTN_R_DOUBLETOP:			// Reversal Double Top			(���ֺ���)
		case PTN_BULL_ISOTRIANGLE:		// Bull Isosceles Triangles		(�����̵�ﰢ��)
		case PTN_BEAR_ISOTRIANGLE:		// Bear Isosceles Triangles		(�༼�̵�ﰢ��)
		case PTN_BULL_TRIANGLE:			// Bull Triangles				(��»ﰢ��)
		case PTN_BEAR_TRIANGLE:			// Bear Triangles				(�϶��ﰢ��)
		case PTN_BULLISH_WIDETRIANGLE:	// Bullish Wide Triangles		(���Ȯ��ﰢ��)
		case PTN_BEARISH_WIDETRIANGLE:	// Bearish Wide Triangles		(�϶�Ȯ��ﰢ��)
		case PTN_BULL_WEDGE:			// Bull Wedge					(���������)
		case PTN_BEAR_WEDGE:			// Bear Wedge					(���������)
		case PTN_BULL_SQUAREFLAG:		// Bull Square Flag				(��»簢�����)
		case PTN_BEAR_SQUAREFLAG:		// Bear Square Flag				(�϶��簢�����)
		case PTN_BULL_TRIANGLEFLAG:		// Bull Triangle Flag			(��»ﰢ�����)
		case PTN_BEAR_TRIANGLEFLAG:		// Bear Triangle Flag			(�϶��ﰢ�����)
		case PTN_BULL_RECTANGLE:		// Bull Rectangle				(������簢��)
		case PTN_BEAR_RECTANGLE:		// Bear Rectangle				(�϶����簢��)
			{
				// Get First Neck Pt
				dDataOffset = pWavePatternData->m_dNeckSeq1;
				nNeckXPos1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
				nNeckYPos1 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckYPt1, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				// Get Second Neck Pt
				dDataOffset = pWavePatternData->m_dNeckSeq2;
				nNeckXPos2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
				nNeckYPos2 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckYPt2, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

				// Neck Y Length
				nNeckYBase1 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckBaseYPt1, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				nNeckYBase2 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckBaseYPt2, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

				nNeckYLength1 = nNeckYBase2 - nNeckYBase1;
				if (	pWavePatternData->m_nPatternType == PTN_BULLISH_WIDETRIANGLE	|| 
						pWavePatternData->m_nPatternType == PTN_BEARISH_WIDETRIANGLE	|| 
						pWavePatternData->m_nPatternType == PTN_BULL_SQUAREFLAG			||
						pWavePatternData->m_nPatternType == PTN_BEAR_SQUAREFLAG		)
				{
					nNeckYLength2 = nNeckYBase1 - nNeckYBase2;
				}
				else
				{
					nNeckYLength2 = nNeckYLength1;
				}

				// Neck X Length
				nNeckXLength = nNeckXPos2 - nNeckXPos1;
			}
			break;

		case PTN_V_TOP:					// V Top
		case PTN_V_BOTTOM:				// V Bottom
			break;
		}


		// Get Pattern Name
		strPatternName = m_wpWavePattern.GetPatternName(pWavePatternData->m_nPatternType);
		
		// Find at Draw Line1
		if ( nX1 > 0 && nY1 > 0 && nX2 > 0 && nY2 > 0 )
		{
			if ( nY1 >= nY2)
			{
				nYHigh = nY1;
				nYLow  = nY2;
			}
			else
			{
				nYHigh = nY2;
				nYLow  = nY1;
			}

			if ( nX1 <= point.x && nX2 >= point.x )
			{
				if ( nYHigh >= point.y && nYLow <= point.y)
				{
					// ����(Gradient)�� ���Ѵ�.
					dGradient = m_wpWavePattern.GetGradient(nX1, nY1, nX2, nY2);
					
					// Ŭ���� ����Ʈ�� X��ǥ�� Y���� ������ üũ
					nYOut = int(dGradient * (point.x - nX1) + nY1);

					if ( (nYOut+4 >= point.y) && (nYOut-4 <= point.y))
					{
						if ( nSamePointCnt < 1)
						{
							pWavePatternData->m_bIsSelect = TRUE;

							m_selWavePtternData.m_nPatternType	= pWavePatternData->m_nPatternType;
							m_selWavePtternData.m_nStartSeq		= pWavePatternData->m_nStartSeq;
							return TRUE;
						}
						else
						{
							nSamePointCnt--;
							continue;
						}
					}
				}
				else if ( abs(nYHigh-nYLow <= 1 ) )	// ������ ���
				{
					nYOut = nYHigh;

					if ( (nYOut+4 >= point.y) && (nYOut-4 <= point.y))
					{
						if ( nSamePointCnt < 1)
						{
							pWavePatternData->m_bIsSelect = TRUE;

							m_selWavePtternData.m_nPatternType	= pWavePatternData->m_nPatternType;
							m_selWavePtternData.m_nStartSeq		= pWavePatternData->m_nStartSeq;
							return TRUE;
						}
						else
						{
							nSamePointCnt--;
							continue;
						}
					}
				}
			}
		}
		// Find at Draw Line2
		if ( nX3 > 0 && nY3 > 0 && nX4 > 0 && nY4 > 0 )
		{
			if ( nY3 >= nY4)
			{
				nYHigh = nY3;
				nYLow  = nY4;
			}
			else
			{
				nYHigh = nY4;
				nYLow  = nY3;
			}

			if ( nX3 <= point.x && nX4 >= point.x )
			{
				if ( nYHigh >= point.y && nYLow <= point.y )
				{
					// ����(Gradient)�� ���Ѵ�.
					dGradient = m_wpWavePattern.GetGradient(nX3, nY3, nX4, nY4);
					
					// Ŭ���� ����Ʈ�� X��ǥ�� Y���� ������ üũ
					nYOut = int(dGradient * (point.x - nX3) + nY3);

					if ( (nYOut+4 >= point.y) && (nYOut-4 <= point.y))
					{
						if ( nSamePointCnt < 1)
						{
							pWavePatternData->m_bIsSelect = TRUE;

							m_selWavePtternData.m_nPatternType	= pWavePatternData->m_nPatternType;
							m_selWavePtternData.m_nStartSeq		= pWavePatternData->m_nStartSeq;
							return TRUE;
						}
						else
						{
							nSamePointCnt--;
							continue;
						}
					}
				}
				else if ( abs(nYHigh-nYLow <= 1 ) )	// ������ ���
				{
					nYOut = nYHigh;

					if ( (nYOut+4 >= point.y) && (nYOut-4 <= point.y))
					{
						if ( nSamePointCnt < 1)
						{
							pWavePatternData->m_bIsSelect = TRUE;

							m_selWavePtternData.m_nPatternType	= pWavePatternData->m_nPatternType;
							m_selWavePtternData.m_nStartSeq		= pWavePatternData->m_nStartSeq;
							return TRUE;
						}
						else
						{
							nSamePointCnt--;
							continue;
						}
					}
				}
			}
		}

		// Find at Draw Neck Line1
		if ( nNeckXPos1 > 0 && nNeckYPos1 > 0)
		{

		}
		// Find at Draw Neck Line2
		if ( nNeckXPos2 > 0 && nNeckYPos2 > 0)
		{

		}
		// <---------------------------------------------------

	}

	return FALSE;
}


void CPatternMaker::InitSelectedPatternData(BOOL bReal/*=FALSE*/)
{
	CWavePatternData *pWavePatternData = NULL;

	// Initialize with FALSE value the m_bIsSelect variable which is show selected state or not.
	for ( int i = 0; i < m_aryWavePatternData.GetSize(); i++)
	{
		pWavePatternData = m_aryWavePatternData.GetAt(i);
		if ( !pWavePatternData)	continue;

		pWavePatternData->m_bIsSelect = FALSE;
	}


	// Initialize with FALSE value of the m_bDraw which is a variable of the m_spdDrawingData.
	m_spdDrawingData.m_bDraw = FALSE;


	// Initialize with ZERO value of the m_nPatternType & m_nStartSeq which is a variable the m_spdDrawingData.
	if ( !bReal)
	{
		m_selWavePtternData.m_nPatternType	= ZERO;
		m_selWavePtternData.m_nStartSeq		= ZERO;

		m_bSelWavePtn = FALSE;	// Selected Wave Pattern OnLButtonDown
	}
}

void CPatternMaker::CheckSelectedPatternData_ForReal()
{
	// ���õ� ������ ������ ����
	if( !m_bSelWavePtn)	return;

	CWavePatternData *pWavePatternData = NULL;

	for ( int i = 0; i < m_aryWavePatternData.GetSize(); i++)
	{
		pWavePatternData = m_aryWavePatternData.GetAt(i);
		if ( !pWavePatternData)	continue;

		if ( (m_selWavePtternData.m_nPatternType == pWavePatternData->m_nPatternType) && 
			 (m_selWavePtternData.m_nStartSeq	 == pWavePatternData->m_nStartSeq)		)
		{
			pWavePatternData->m_bIsSelect = TRUE;
			break;
		}
	}
}



void CPatternMaker::DeleteCandlePatternData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryCandlePatternData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryCandlePatternData.GetAt(i);
		delete pObj;
	}

	m_aryCandlePatternData.RemoveAll();
}

void CPatternMaker::DeleteCandlePatternPosInfo()
{
	CObject *pObj=NULL;

	for(int i=0; i < m_aryCandlePatternPosInfo.GetSize(); i++)
	{
		pObj = (CObject *) m_aryCandlePatternPosInfo.GetAt(i);
		delete pObj;
	}

	m_aryCandlePatternPosInfo.RemoveAll();
}

void CPatternMaker::GetAllData_AboutCandlePattern()
{
	// ���Ϻм�â�� ���� ���
	if( !m_pDlgAnalysis)	return;

	// Drawing�� ������ ��Ȳ���� Ȯ���Ѵ�.
	if( !m_pIChartManager || !m_pIChartOCX || !m_pIPacketManager) return;


	// �ð����� �ڷ����ڸ� ���Ѵ� ---------------------------------------------->>
	IPacket *pIPacketTime, *pIPacketOpen, *pIPacketHigh, *pIPacketLow, *pIPacketClose;
	pIPacketTime = pIPacketOpen = pIPacketHigh = pIPacketLow = pIPacketClose = NULL;

// 2008.01.20 by LYH >>	
//	pIPacketTime	= m_pIPacketManager->GetPacket( m_strRQ, "�ڷ�����");
//	pIPacketOpen	= m_pIPacketManager->GetPacket( m_strRQ, "�ð�");
//	pIPacketHigh	= m_pIPacketManager->GetPacket( m_strRQ, "��");
//	pIPacketLow		= m_pIPacketManager->GetPacket( m_strRQ, "����");
//	pIPacketClose	= m_pIPacketManager->GetPacket( m_strRQ, "����");
	pIPacketTime	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_DATE_TIME));
	pIPacketOpen	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_OPEN));
	pIPacketHigh	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_HIGH));
	pIPacketLow		= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_LOW));
	pIPacketClose	= m_pIPacketManager->GetPacket( m_strRQ, _MTEXT(C0_CLOSE));
// 2008.01.20 by LYH <<

	if ( !pIPacketTime || !pIPacketOpen || !pIPacketHigh || !pIPacketLow || !pIPacketClose ) return;


	CList<double,double> *clistTime, *clistOpen, *clistHigh, *clistLow, *clistClose;
	clistTime = clistOpen = clistHigh = clistLow = clistClose = NULL;
		
	clistTime	= pIPacketTime->GetDataList();
	clistOpen	= pIPacketOpen->GetDataList();
	clistHigh	= pIPacketHigh->GetDataList();
	clistLow	= pIPacketLow->GetDataList();
	clistClose	= pIPacketClose->GetDataList();
	// <<--------------------------------------------------------------------------


	// �ð����� ���� ----------------------------------------------->>
	int nTotalDataCnt	= clistTime->GetCount();

	double *pdTime		= new double[nTotalDataCnt];	// �ð�
	double *pdOpen		= new double[nTotalDataCnt];	// �ð�
	double *pdHigh		= new double[nTotalDataCnt];	// ��
	double *pdLow		= new double[nTotalDataCnt];	// ����
	double *pdClose		= new double[nTotalDataCnt];	// ����


	int			nNum = 0;
	POSITION	posTime, posOpen, posHigh, posLow, posClose;
	
	for(posTime = clistTime->GetHeadPosition(),		posOpen = clistOpen->GetHeadPosition(), 
		posHigh  = clistHigh->GetHeadPosition(),	posLow  = clistLow->GetHeadPosition(),		
		posClose = clistClose->GetHeadPosition(); 	posTime && posOpen && posHigh && posLow && posClose;)
	{
		if (posTime == NULL)	continue;

		pdTime[nNum]	= (double)clistTime->GetNext(posTime);
		pdOpen[nNum]	= (double)clistOpen->GetNext(posOpen);
		pdHigh[nNum]	= (double)clistHigh->GetNext(posHigh);
		pdLow[nNum]		= (double)clistLow->GetNext(posLow);
		pdClose[nNum]	= (double)clistClose->GetNext(posClose);

		nNum++;
	}
	// <<-------------------------------------------------------------

	STCANDLE_DATA stCandleData;
	stCandleData.nDataCnt	= nTotalDataCnt;
	stCandleData.dTime		= pdTime;
	stCandleData.dOpen		= pdOpen;
	stCandleData.dHigh		= pdHigh;
	stCandleData.dLow		= pdLow;
	stCandleData.dClose		= pdClose;


	// *************************************************************
	// ## Find All Candle Pattern
	// *************************************************************
	m_cpCandlePattern.CDFind_All_CandlePattern( &stCandleData, &m_aryCandlePatternData);


	// *************************************************************
	// Statistics of the Candle Pattern Analysis
	// *************************************************************
	// Sorting by Time
	CCandlePatternData* pCandlePatternData1 = NULL;
	CCandlePatternData* pCandlePatternData2 = NULL;
	for ( int i = 0; i < m_aryCandlePatternData.GetSize(); i++)
	{
		for ( int j = 0; j < m_aryCandlePatternData.GetSize()-1; j++)
		{
			pCandlePatternData1 = m_aryCandlePatternData.GetAt(j);
			pCandlePatternData2 = m_aryCandlePatternData.GetAt(j+1);

			if ( pCandlePatternData1->m_nEndPos > pCandlePatternData2->m_nEndPos)
			{
				m_aryCandlePatternData.SetAt(j+1,	pCandlePatternData1);
				m_aryCandlePatternData.SetAt(j,		pCandlePatternData2);
			}
		}
	}

	
	// Set Candle PatternData to Grid
	if ( m_pDlgAnalysis)
	{
		m_pDlgAnalysis->SetReportTitleBasicInfo( stCandleData.dTime[0], stCandleData.dTime[nTotalDataCnt-1], nTotalDataCnt);
		m_pDlgAnalysis->SetFoundCandlePtnToGrid_RptType1( &m_aryCandlePatternData);
	}



	pIPacketTime->Release();
	pIPacketOpen->Release();
	pIPacketHigh->Release();
	pIPacketLow->Release();
	pIPacketClose->Release();

	delete []pdTime;
	delete []pdOpen;
	delete []pdHigh;
	delete []pdLow;
	delete []pdClose;
}


void CPatternMaker::InitSelectedCandlePatternData()
{
	CCandlePatternData* pCandlePatternData = NULL;
	pCandlePatternData = new CCandlePatternData;

	memcpy( &m_selCandlePatternData, pCandlePatternData, sizeof(m_selCandlePatternData));
	
	delete pCandlePatternData;
}


void CPatternMaker::SetSelectedCandlePatternData( double dTime, CString strPatternName)
{
	int nPatternType;
	CCandlePatternData* pCandlePatternData = NULL;

	for ( int i = 0; i < m_aryCandlePatternData.GetSize(); i++)
	{

		pCandlePatternData = m_aryCandlePatternData.GetAt(i);

		if ( pCandlePatternData)
		{
			nPatternType = pCandlePatternData->m_nCandlePtnType;
			if ( pCandlePatternData->m_dTime == dTime && m_cpCandlePattern.GetCandlePatternName( nPatternType) == strPatternName)
			{
				memcpy( &m_selCandlePatternData, pCandlePatternData, sizeof(m_selCandlePatternData));
			}
		}
	}
}

void CPatternMaker::ResetScrollPosition( int nDataPos)
{
	int nStartPos	= m_selCandlePatternData.m_nStartPos;
	int nEndPos		= m_selCandlePatternData.m_nEndPos;

	ML_SET_LANGUAGE_RES();
	AfxMessageBox( IDS_MSG_SCROLL);
}

void CPatternMaker::SetGSharedData(CString strKey, CString strData)
{
	m_pIChartOCX->FireGSharedData(strKey,strData);
}

BOOL CPatternMaker::OnMouseMove( UINT nFlags, CPoint &point, CDC* pDC)
{
	// ���Ϻм�â�� ���� ���
	if( !m_pDlgAnalysis)	return FALSE;


	// Ư����Ʈ�� ��쿡�� ������ �ʵ��� �Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	if( m_pIChartOCX->IsOnSpecialChart()) return FALSE;

	// Drawing�� ������ ��Ȳ���� Ȯ���Ѵ�.
	if( !m_pIChartManager || !m_pIChartOCX) return FALSE;


	CCandlePatternPosInfo* pInfo = NULL;
	int nFindIdx = -1;
	for(int i=0; i<m_aryCandlePatternPosInfo.GetSize(); i++)
	{
		 pInfo = m_aryCandlePatternPosInfo.GetAt(i);
		 if ( (point.x > pInfo->m_rect.left && point.x < pInfo->m_rect.right) &&
			 (point.y > pInfo->m_rect.top  && point.y < pInfo->m_rect.bottom)	)
		 {
			 nFindIdx = i;
			 break;
		 }
	}

	if(nFindIdx >= 0)
	{
		CRect rect;
		rect.left = pInfo->m_rect.left;
		rect.right = pInfo->m_rect.right;
		rect.top = pInfo->m_rect.top+12;
		rect.bottom = pInfo->m_rect.bottom+12;
		CPoint pntCursor = SetToolTipText(m_cpCandlePattern.GetCandlePatternName(pInfo->m_nCandlePtnType), rect);
		RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(pntCursor.x),LOWORD(pntCursor.y)));
	}
	else
	{
		CRect rect(0,0,0,0);
		SetToolTipText("", rect);
	}
	return FALSE;
}

CPoint CPatternMaker::SetToolTipText(CString s, CRect rect)
{
	CPoint point(0,0);
	if(m_toolTip.m_hWnd==NULL)
	{	
		if(s.GetLength() <= 0)
			return point;

		if( m_toolTip.Create(m_hParentWnd, TTS_ALWAYSTIP))
		{

			if(m_toolTip.AddTool(m_hParentWnd, (LPCTSTR)s))
			{
				m_toolTip.Activate(1);
			}
		}
	} 
	else
	{
		m_toolTip.UpdateTipText((LPCTSTR)s,m_hParentWnd);
	}
	return point;
}

void CPatternMaker::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(NULL != m_toolTip.m_hWnd)
	{
		MSG msg;
		msg.hwnd = m_hParentWnd->GetSafeHwnd();
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);

		m_toolTip.RelayEvent(&msg);
	}
}

void CPatternMaker::DrawWaveLine( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion)
{
	// �ֿ���������� ������ �ĵ���, �������� ����, ������������ �׸��� *******************************(1_ST)
	double	dDataOffset;
	BOOL	bDrawHighLowPtLine = (m_PatternEnvData.m_bDrawHighLowPtLine && !m_PatternEnvData.m_bNotDrawAll);	// �ĵ��� �׸��� ����
	int		nXHigh, nYHigh, nXLow, nYLow;
	nXHigh = nYHigh = nXLow = nYLow = 0;

	CHighLowPtData	*pHighLowPtData, *pHighLowPtTemp;
	pHighLowPtData = pHighLowPtTemp = NULL;

	int nPointCnt = m_aryHighLowPtData.GetSize();


	for(int i=0; i < nPointCnt; i++ )
	{
		pHighLowPtData = m_aryHighLowPtData.GetAt(i);
		
		if ( !pHighLowPtData ) continue;

		// ************************************************
		// �ĵ����� �׸���
		// ************************************************
		// *** �׷��� �������� �ִ� �����͸� �׸���. ***
		if ((pHighLowPtData->m_nDataSeq >= nStartIndex) && (pHighLowPtData->m_nDataSeq <= nEndIndex) && (bDrawHighLowPtLine))
		{
			// �������� �������
			if ( pHighLowPtData->m_nPointType == CHANGINGPT_HIGHTOLOW && pHighLowPtData->m_dHigh != ZERO)
			{
				// X��ǥ�� ���Ѵ�
				dDataOffset = pHighLowPtData->m_nDataSeq;
				if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
					continue;
				nXHigh = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
				
				// Y��ǥ�� ���Ѵ�
				nYHigh = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtData->m_dHigh, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				
				// ����) ******
				// ���� �Ǵ� ���� ������ �׷��� ������ ��� ������ �ʴ� ���(���� 1��)
				int nXOut, nYOut;
				double dGradient;
				
				// ��������						
				if (i < nPointCnt-1)
				{
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(i+1);
					if ( (pHighLowPtTemp->m_nDataSeq > nEndIndex) && (pHighLowPtTemp->m_nPointType == CHANGINGPT_LOWTOHIGH) )
					{
						// X��ǥ�� ���Ѵ�
						dDataOffset = pHighLowPtTemp->m_nDataSeq;
						if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
							continue;
						nXOut = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
						
						// Y��ǥ�� ���Ѵ�
						nYOut = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtTemp->m_dLow, m_dViewMax, m_dViewMin, 
																		rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
						
						// ����(Gradient)�� ���Ѵ�.
						dGradient = m_wpWavePattern.GetGradient(nXHigh, nYHigh, nXOut, nYOut);
						
						// �����׷��� �������� ������ ���Ѵ�.
						nXOut = rctGraphRegion.right;
						nYOut = int(dGradient * (nXOut - nXHigh) + nYHigh);

						// Y��ǥ�� YPos �׷��� ������ ����� ��� ó��
						if (nYOut < rctGraphRegion.top)
						{
							nYOut = rctGraphRegion.top;
							nXOut = int((nYOut-nYHigh)/dGradient + nXHigh);
						}
						else if (nYOut > rctGraphRegion.bottom)
						{
							nYOut = rctGraphRegion.bottom;
							nXOut = int((nYOut-nYHigh)/dGradient + nXHigh);
						}
						
						// ## ���� ������ �׸��� ##
						//p_pDC->MoveTo( nXOut, nYOut);
						//p_pDC->LineTo( nXHigh, nYHigh);
					}
				}
				
				// ��������
				if ( i > 0 )
				{
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(i-1);
					if ( (pHighLowPtTemp->m_nDataSeq < nStartIndex) && (pHighLowPtTemp->m_nPointType == CHANGINGPT_LOWTOHIGH) )
					{
						// X��ǥ�� ���Ѵ�
						dDataOffset = pHighLowPtTemp->m_nDataSeq;
						if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
							continue;
						nXOut = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
						
						// Y��ǥ�� ���Ѵ�
						nYOut = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtTemp->m_dLow, m_dViewMax, m_dViewMin, 
																		rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
						
						// ����(Gradient)�� ���Ѵ�.
						dGradient = m_wpWavePattern.GetGradient(nXHigh, nYHigh, nXOut, nYOut);
						
						// �����׷��� �������� ������ ���Ѵ�.
						nXOut = rctGraphRegion.left;
						nYOut = int(dGradient * (nXOut - nXHigh) + nYHigh);

						// Y��ǥ�� YPos �׷��� ������ ����� ��� ó��
						if (nYOut < rctGraphRegion.top)
						{
							nYOut = rctGraphRegion.top;
							nXOut = int((nYOut-nYHigh)/dGradient + nXHigh);
						}
						else if (nYOut > rctGraphRegion.bottom)
						{
							nYOut = rctGraphRegion.bottom;
							nXOut = int((nYOut-nYHigh)/dGradient + nXHigh);
						}
						
						// ## ���� ������ �׸��� ##
						//p_pDC->MoveTo( nXOut, nYOut);
						//p_pDC->LineTo( nXHigh, nYHigh);
					}
				}
			}	
			// �������� �������
			else if ( pHighLowPtData->m_nPointType == CHANGINGPT_LOWTOHIGH && pHighLowPtData->m_dLow != ZERO)
			{
				// X��ǥ�� ���Ѵ�
				dDataOffset = pHighLowPtData->m_nDataSeq;
				if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
					continue;
				nXLow = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
				
				// Y��ǥ�� ���Ѵ�
				nYLow = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtData->m_dLow, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				
				// ����) ******
				// ���� �Ǵ� ���� ������ �׷��� ������ ��� ������ �ʴ� ���(���� 1��)
				int nXOut, nYOut;
				double dGradient;
				
				// ��������						
				if (i < nPointCnt-1)
				{
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(i+1);
					if ( (pHighLowPtTemp->m_nDataSeq > nEndIndex) && (pHighLowPtTemp->m_nPointType == CHANGINGPT_HIGHTOLOW) )
					{
						// X��ǥ�� ���Ѵ�
						dDataOffset = pHighLowPtTemp->m_nDataSeq;
						if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
							continue;
						nXOut = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
						
						// Y��ǥ�� ���Ѵ�
						nYOut = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtTemp->m_dHigh, m_dViewMax, m_dViewMin, 
																		rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
						
						// ����(Gradient)�� ���Ѵ�.
						dGradient = m_wpWavePattern.GetGradient(nXLow, nYLow, nXOut, nYOut);
						
						// �����׷��� �������� ������ ���Ѵ�.
						nXOut = rctGraphRegion.right;
						nYOut = int(dGradient * (nXOut - nXLow) + nYLow);

						// Y��ǥ�� YPos �׷��� ������ ����� ��� ó��
						if (nYOut < rctGraphRegion.top)
						{
							nYOut = rctGraphRegion.top;
							nXOut = int((nYOut-nYLow)/dGradient + nXLow);
						}
						else if (nYOut > rctGraphRegion.bottom)
						{
							nYOut = rctGraphRegion.bottom;
							nXOut = int((nYOut-nYLow)/dGradient + nXLow);
						}
						
						// ## ���� ������ �׸��� ##
						//p_pDC->MoveTo( nXOut, nYOut);
						//p_pDC->LineTo( nXLow, nYLow);
					}
				}

				// ��������
				if ( i > 0 )
				{
					pHighLowPtTemp = m_aryHighLowPtData.GetAt(i-1);
					if ( (pHighLowPtTemp->m_nDataSeq < nStartIndex) && (pHighLowPtTemp->m_nPointType == CHANGINGPT_HIGHTOLOW) )
					{
						// X��ǥ�� ���Ѵ�
						dDataOffset = pHighLowPtTemp->m_nDataSeq;
						if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
							continue;
						nXOut = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
						
						// Y��ǥ�� ���Ѵ�
						nYOut = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtTemp->m_dHigh, m_dViewMax, m_dViewMin, 
																		rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
						
						// ����(Gradient)�� ���Ѵ�.
						dGradient = m_wpWavePattern.GetGradient(nXLow, nYLow, nXOut, nYOut);
						
						// �����׷��� �������� ������ ���Ѵ�.
						nXOut = rctGraphRegion.left;
						nYOut = int(dGradient * (nXOut - nXLow) + nYLow);

						// Y��ǥ�� YPos �׷��� ������ ����� ��� ó��
						if (nYOut < rctGraphRegion.top)
						{
							nYOut = rctGraphRegion.top;
							nXOut = int((nYOut-nYLow)/dGradient + nXLow);
						}
						else if (nYOut > rctGraphRegion.bottom)
						{
							nYOut = rctGraphRegion.bottom;
							nXOut = int((nYOut-nYLow)/dGradient + nXLow);
						}
						
						// ## ���� ������ �׸��� ##
						//p_pDC->MoveTo( nXOut, nYOut);
						//p_pDC->LineTo( nXLow, nYLow);
					}
				}
			}

			// ### ����, ���� ������ �׸��� ###
			if (nXHigh > 0 && nYHigh > 0 && nXLow > 0 && nYLow > 0)
			{
				CPen pnSteady(PS_SOLID, m_PatternEnvData.m_lineDataHighLowPt.m_nWeight, m_PatternEnvData.m_lineDataHighLowPt.m_color);
				CPen *ppnOld = p_pDC->SelectObject(&pnSteady);
				p_pDC->MoveTo( nXHigh, nYHigh);
				p_pDC->LineTo( nXLow, nYLow);
				p_pDC->SelectObject(ppnOld);
			}
		}


		// ************************************************
		// ����, ���� ���� ������ �׸���
		// ************************************************
		BOOL bDrawHighPtHLine	= (m_PatternEnvData.m_bDrawHighPtHLine && !m_PatternEnvData.m_bNotDrawAll);
		BOOL bDrawLowPtHLine	= (m_PatternEnvData.m_bDrawLowPtHLine && !m_PatternEnvData.m_bNotDrawAll);

		if ( bDrawHighPtHLine || bDrawLowPtHLine)
		{
			int nXTemp, nYTemp;
			double dTemp = 0;
			CPen pnSteady;			
			if ( pHighLowPtData->m_nPointType == CHANGINGPT_HIGHTOLOW && pHighLowPtData->m_dHigh != ZERO && bDrawHighPtHLine)
			{
				p_pDC->SelectObject( &m_pnHighLinePen);
				dTemp = pHighLowPtData->m_dHigh;
				pnSteady.CreatePen(PS_SOLID, m_PatternEnvData.m_lineDataHighPtH.m_nWeight, m_PatternEnvData.m_lineDataHighPtH.m_color);
			}
			else if ( pHighLowPtData->m_nPointType == CHANGINGPT_LOWTOHIGH && pHighLowPtData->m_dLow != ZERO && bDrawLowPtHLine)
			{
				p_pDC->SelectObject( &m_pnLowLinePen);
				dTemp = pHighLowPtData->m_dLow;
				pnSteady.CreatePen(PS_SOLID, m_PatternEnvData.m_lineDataLowPtH.m_nWeight, m_PatternEnvData.m_lineDataLowPtH.m_color);
			}
			
			if ( (pHighLowPtData->m_nDataSeq < nStartIndex) && (dTemp > 0) )
			{
				// Y��ǥ�� ���Ѵ�
				nYTemp = m_pIChartOCX->ConvertDataToYPosition(	dTemp, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
				// ### ���� ������ �׸��� ###
				if (nYTemp >= rctGraphRegion.top && nYTemp <= rctGraphRegion.bottom)	// YPos�� �׷��� �����ΰ��
				{

					CPen *ppnOld = p_pDC->SelectObject(&pnSteady);

					p_pDC->MoveTo( rctGraphRegion.left, nYTemp);
					p_pDC->LineTo( rctGraphRegion.right, nYTemp);

					p_pDC->SelectObject(ppnOld);
				}
			}
			else if ( (pHighLowPtData->m_nDataSeq <= nEndIndex) && (dTemp > 0) )
			{
				// X��ǥ�� ���Ѵ�
				dDataOffset = pHighLowPtData->m_nDataSeq;
				if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
					continue;
				nXTemp = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);

				// Y��ǥ�� ���Ѵ�
				nYTemp = m_pIChartOCX->ConvertDataToYPosition(	dTemp, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				
				// ### ���� ������ �׸��� ###
				if (nYTemp >= rctGraphRegion.top && nYTemp <= rctGraphRegion.bottom)	// YPos�� �׷��� �����ΰ��
				{
					CPen *ppnOld = p_pDC->SelectObject(&pnSteady);
					p_pDC->MoveTo( nXTemp, nYTemp);
					p_pDC->LineTo( rctGraphRegion.right, nYTemp);
					p_pDC->SelectObject(ppnOld);
				}
			}

			p_pDC->SelectObject( &m_pnHighLowLinePen);
		}
	}
	// **************************************************************************************************(1_END)
}

void CPatternMaker::DrawMALine( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion)
{
	// �����̵������ �׸��� ****************************************************************************(2_ST)
	double	dDataOffset;
	int nX1, nY1, nX2, nY2;
	BOOL bDrawMMa = (m_PatternEnvData.m_bDrawMMa && !m_PatternEnvData.m_bNotDrawAll);
	if (bDrawMMa)
	{
		nX1 = nY1 = nX2 = nY2 = 0;

		CMAData	*pMaData = NULL;
		int nMACnt = m_aryMAData.GetSize();
		
		for(int i=0; i < nMACnt; i++ )
		{
			pMaData = m_aryMAData.GetAt(i);
			if ( !pMaData ) continue;

			// *** �׷��� �������� �ִ� �����͸� �׸��� ***
			if ((pMaData->m_nDataSeq >= nStartIndex) && (pMaData->m_nDataSeq <= nEndIndex))
			{
				nX2 = nX1;	
				nY2 = nY1;

				// X��ǥ�� ���Ѵ�
				dDataOffset = pMaData->m_nDataSeq;
				if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
					continue;
				nX1 = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
				
				// Y��ǥ�� ���Ѵ�
				nY1 = m_pIChartOCX->ConvertDataToYPosition(	pMaData->m_dMMa, m_dViewMax, m_dViewMin, 
															rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
				// ### �̵���� ������ �׸��� ###
				if (nX1 > 0 && nY1 > 0 && nX2 > 0 && nY2 > 0)
				{
					p_pDC->MoveTo( nX1, nY1);
					p_pDC->LineTo( nX2, nY2);
				}
			}
		}
	}
}

//�ڵ��߼���
void CPatternMaker::DrawAutoTrendLine( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion)
{
	// ����, �������� �׸��� ****************************************************************************(3_ST)
	double	dDataOffset;
	int nX1, nY1, nX2, nY2;

	CRegistNSupportData *pRegistNSupportData = NULL;
	double		dGradient = 0;
	BOOL		bDrawHighRegistLine = (m_PatternEnvData.m_bDrawHighRegistLine && !m_PatternEnvData.m_bNotDrawAll);
	BOOL		bDrawLowSupportLine = (m_PatternEnvData.m_bDrawLowSupportLine && !m_PatternEnvData.m_bNotDrawAll);

	int nPointCnt = m_aryRegistNSupportData.GetSize();

	for (int i = 0; i < nPointCnt; i++)
	{
		nX1 = nY1 = nX2 = nY2 = 0;

		pRegistNSupportData = m_aryRegistNSupportData.GetAt(i);
		if ( !pRegistNSupportData)	continue;

		// �������׼�, ���������� �׸��� ����
		if (	pRegistNSupportData->m_nRSLineType == LINE_HIGHREGIST && !bDrawHighRegistLine) 	continue;
		else if(pRegistNSupportData->m_nRSLineType == LINE_LOWSUPPORT && !bDrawLowSupportLine)	continue;


		// *** ù��° �����Ͱ� �������� �ְ�, �ι�° �����ʹ� �������� �ְų� ���� ��� ***
		if ((pRegistNSupportData->m_nDataSeq1 >= nStartIndex) && (pRegistNSupportData->m_nDataSeq1 <= nEndIndex))
		{
			// Point1
			dDataOffset = pRegistNSupportData->m_nDataSeq1;
			if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
				continue;
			nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
			nY1 = m_pIChartOCX->ConvertDataToYPosition( pRegistNSupportData->m_dYPoint1, m_dViewMax, m_dViewMin, 
														rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

			// Point2
			dDataOffset = pRegistNSupportData->m_nDataSeq2;
			if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
				continue;
			nX2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
			nY2 = m_pIChartOCX->ConvertDataToYPosition( pRegistNSupportData->m_dYPoint2, m_dViewMax, m_dViewMin, 
														rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

			// ����(Gradient)�� ���Ѵ�.
			dGradient = m_wpWavePattern.GetGradient(nX1, nY1, nX2, nY2);
			
			// �����׷��� �������� ������ ���Ѵ�.
			//{{
			//nX2 = rctGraphRegion.right;
			nX2 = m_pIChartManager->GetGraphXArea_End(TRUE);
			//}}
			nY2 = int(dGradient * (nX2 - nX1) + nY1);

			// Point2�� YPos �׷��� ������ ����� ��� ó��
			if (nY2 < rctGraphRegion.top)
			{
				nY2 = rctGraphRegion.top;
				nX2 = int((nY2-nY1)/dGradient + nX1);
			}
			else if (nY2 > rctGraphRegion.bottom)
			{
				nY2 = rctGraphRegion.bottom;
				nX2 = int((nY2-nY1)/dGradient + nX1);
			}
		}
		// *** ù��° �����Ͱ� �������� �ְų� ����, �ι�° �����Ͱ� �������� �ִ� ��� ***
		else if ((pRegistNSupportData->m_nDataSeq2 >= nStartIndex) && (pRegistNSupportData->m_nDataSeq2 <= nEndIndex))
		{
			// Point1
			dDataOffset = pRegistNSupportData->m_nDataSeq1;
			if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
				continue;
			nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
			nY1 = m_pIChartOCX->ConvertDataToYPosition( pRegistNSupportData->m_dYPoint1, m_dViewMax, m_dViewMin, 
														rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

			// Point2
			dDataOffset = pRegistNSupportData->m_nDataSeq2;
			if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
				continue;
			nX2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
			nY2 = m_pIChartOCX->ConvertDataToYPosition( pRegistNSupportData->m_dYPoint2, m_dViewMax, m_dViewMin, 
														rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			if( nX1<0 || nX2<0 )
				continue;

			// ����(Gradient)�� ���Ѵ�.
			dGradient = m_wpWavePattern.GetGradient(nX1, nY1, nX2, nY2);
			
			// �����׷��� �������� ������ ���Ѵ�.
			nX1 = rctGraphRegion.left;
			nY1 = int(dGradient * (nX1 - nX2) + nY2);

			// Point1�� YPos �׷��� ������ ����� ��� ó��
			if (nY1 < rctGraphRegion.top)
			{
				nY1 = rctGraphRegion.top;
				nX1 = int((nY1-nY2)/dGradient + nX2);
			}
			else if (nY1 > rctGraphRegion.bottom)
			{
				nY1 = rctGraphRegion.bottom;
				nX1 = int((nY1-nY2)/dGradient + nX2);
			}
			

			// �����׷��� �������� ������ ���Ѵ�.
			//nX2 = rctGraphRegion.right;
			nX2 = m_pIChartManager->GetGraphXArea_End(TRUE);
			nY2 = int(dGradient * (nX2 - nX1) + nY1);

			// Point2�� YPos �׷��� ������ ����� ��� ó��
			if (nY2 < rctGraphRegion.top)
			{
				nY2 = rctGraphRegion.top;
				nX2 = int((nY2-nY1)/dGradient + nX1);
			}
			else if (nY2 > rctGraphRegion.bottom)
			{
				nY2 = rctGraphRegion.bottom;
				nX2 = int((nY2-nY1)/dGradient + nX1);
			}
		}
		// *** ù��°, �ι�° ������ ��� �׷������� ���ʿ� �ִ� ��� ***
		else if ((pRegistNSupportData->m_nDataSeq1 < nStartIndex) && (pRegistNSupportData->m_nDataSeq2 < nStartIndex))
		{
			// Point1
			dDataOffset = pRegistNSupportData->m_nDataSeq1;
			if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
				continue;
			nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
			nY1 = m_pIChartOCX->ConvertDataToYPosition( pRegistNSupportData->m_dYPoint1, m_dViewMax, m_dViewMin, 
														rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

			// Point2
			dDataOffset = pRegistNSupportData->m_nDataSeq2;
			if(dDataOffset<nStartIndex || dDataOffset>nEndIndex)
				continue;
			nX2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
			nY2 = m_pIChartOCX->ConvertDataToYPosition( pRegistNSupportData->m_dYPoint2, m_dViewMax, m_dViewMin, 
														rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

			if( nX1<0 || nX2<0 )
				continue;
			// ����(Gradient)�� ���Ѵ�.
			dGradient = m_wpWavePattern.GetGradient(nX1, nY1, nX2, nY2);
			
			// �����׷��� �������� ������ ���Ѵ�.
			nX1 = rctGraphRegion.left;
			nY1 = int(dGradient * (nX1 - nX2) + nY2);

			// Point1�� YPos �׷��� ������ ����� ��� ó��
			if (nY1 < rctGraphRegion.top)
			{
				nY1 = rctGraphRegion.top;
				nX1 = int((nY1-nY2)/dGradient + nX2);
			}
			else if (nY1 > rctGraphRegion.bottom)
			{
				nY1 = rctGraphRegion.bottom;
				nX1 = int((nY1-nY2)/dGradient + nX2);
			}

			// *** ����� ������ ������ ������ ��� Point1�� �׷��� �������� �ִ��� üũ ***
			if (nX1 >= rctGraphRegion.left && nX1 <= rctGraphRegion.right)
			{
				// �����׷��� �������� ������ ���Ѵ�.
				//nX2 = rctGraphRegion.right;
				nX2 = m_pIChartManager->GetGraphXArea_End(TRUE);
				nY2 = int(dGradient * (nX2 - nX1) + nY1);

				// Point2�� YPos �׷��� ������ ����� ��� ó��
				if (nY2 < rctGraphRegion.top)
				{
					nY2 = rctGraphRegion.top;
					nX2 = int((nY2-nY1)/dGradient + nX1);
				}
				else if (nY2 > rctGraphRegion.bottom)
				{
					nY2 = rctGraphRegion.bottom;
					nX2 = int((nY2-nY1)/dGradient + nX1);
				}

				// *** ����� ������ ������ ������ ��� Point2�� �׷��� �������� �ִ��� üũ ***
				if (nX2 < rctGraphRegion.left && nX1 > rctGraphRegion.right)
				{
					nX1 = nY1 = nX2 = nY2 = 0;
				}
			}
		}
		

		// ### ����, ���� ������ �׸��� ###
		if (nX1 > 0 && nY1 > 0 && nX2 > 0 && nY2 > 0)
		{
			if(pRegistNSupportData->m_nRSLineType == LINE_HIGHREGIST)
			{
				CPen pnSteady(PS_SOLID, m_PatternEnvData.m_lineDataHighRegist.m_nWeight, m_PatternEnvData.m_lineDataHighRegist.m_color);
				CPen *ppnOld = p_pDC->SelectObject(&pnSteady);

				p_pDC->MoveTo( nX1, nY1);
				p_pDC->LineTo( nX2, nY2);

				p_pDC->SelectObject(ppnOld);
			}
			else if(pRegistNSupportData->m_nRSLineType == LINE_LOWSUPPORT)
			{
				CPen pnSteady(PS_SOLID, m_PatternEnvData.m_lineDataLowSupport.m_nWeight, m_PatternEnvData.m_lineDataLowSupport.m_color);
				CPen *ppnOld = p_pDC->SelectObject(&pnSteady);

				p_pDC->MoveTo( nX1, nY1);
				p_pDC->LineTo( nX2, nY2);

				p_pDC->SelectObject(ppnOld);
			}
		}
	}
	// **************************************************************************************************(3_END)
}

void CPatternMaker::DrawAutoPattern( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion)
{
	BOOL	bDrawAutoPatternLine = (m_PatternEnvData.m_bDrawAutoPatternLine && !m_PatternEnvData.m_bNotDrawAll);	// �߼����� �׸��� ����
	if(!bDrawAutoPatternLine)
		return;

	// ������ �׸��� ************************************************************************************(4_ST)
	CHighLowPtData	*pHighLowPtData, *pHighLowPtTemp;
	pHighLowPtData = pHighLowPtTemp = NULL;

	double	dDataOffset;
	int		nX1, nY1, nX2, nY2;
	int		nX3, nY3, nX4, nY4;
	int		nXHigh, nYHigh, nXLow, nYLow;
	int		nNeckXPos1, nNeckYPos1, nNeckXPos2, nNeckYPos2;
	int		nNeckYBase1, nNeckYBase2, nNeckYLength1, nNeckYLength2, nNeckXLength;
	int		nPatternTrend;
	BOOL	bIsDrawReady;
	CString	strPatternName;
	CRect	rectText;
	CWavePatternData *pWavePatternData = NULL;

	int nPointCnt = m_aryWavePatternData.GetSize();

// 2008.02.20 by LYH >>
	IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd);
	int nTextHeight = 11;
	if(eLanguageID == IMetaTable::ENGLISH)
		nTextHeight = 13;
// 2008.02.20 by LYH <<

	for (int i = 0; i < nPointCnt; i++)
	{
		// ��� ������ �׸��� �ʴ� ���
		if( m_PatternEnvData.m_bNotDrawAll) break;

		nX1 = nY1 = nX2 = nY2 = 0;
		nX3 = nY3 = nX4 = nY4 = 0;
		nNeckXPos1 = nNeckYPos1 = nNeckXPos2 = nNeckYPos2 = 0;
		nNeckYLength1 = nNeckYLength2 = nNeckXLength = 0;
		bIsDrawReady	= FALSE;
		strPatternName	= _T("");

		pWavePatternData = m_aryWavePatternData.GetAt(i);
		if ( !pWavePatternData)	continue;

		
		// Get X, Y Point for Drawing
		switch (pWavePatternData->m_nPatternType)
		{
		case PTN_HEADNSHOULDER:			// Head & Shoulder				(�Ӹ������)
		case PTN_R_HEADNSHOULDER:		// Reversal Head & Shoulder		(���Ӹ������)
		case PTN_V_TOP:					// V Top						(�ܺ�õ����)
		case PTN_V_BOTTOM:				// V Bottom						(�ܺ��ٴ���)
			if ( (pWavePatternData->m_dDrawSeq1_1 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_1 <= nEndIndex) &&
				 (pWavePatternData->m_dDrawSeq1_2 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_2 <= nEndIndex)	)
			{
					// Point1
					dDataOffset = pWavePatternData->m_dDrawSeq1_1;
					nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY1 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_1, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					
					// Point2
					dDataOffset = pWavePatternData->m_dDrawSeq1_2;
					nX2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY2 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_2, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

					bIsDrawReady = TRUE;
			}
			break;

		case PTN_DOUBLETOP:				// Double Top					(�ֺ���)
		case PTN_R_DOUBLETOP:			// Reversal Double Top			(���ֺ���)
		case PTN_BULL_ISOTRIANGLE:		// Bull Isosceles Triangles		(�����̵�ﰢ��)
		case PTN_BEAR_ISOTRIANGLE:		// Bear Isosceles Triangles		(�༼�̵�ﰢ��)
		case PTN_BULL_TRIANGLE:			// Bull Triangles				(��»ﰢ��)
		case PTN_BEAR_TRIANGLE:			// Bear Triangles				(�϶��ﰢ��)
		case PTN_BULLISH_WIDETRIANGLE:	// Bullish Wide Triangles		(���Ȯ��ﰢ��)
		case PTN_BEARISH_WIDETRIANGLE:	// Bearish Wide Triangles		(�϶�Ȯ��ﰢ��)
		case PTN_BULL_WEDGE:			// Bull Wedge					(���������)
		case PTN_BEAR_WEDGE:			// Bear Wedge					(���������)
		case PTN_BULL_SQUAREFLAG:		// Bull Square Flag				(��»簢�����)
		case PTN_BEAR_SQUAREFLAG:		// Bear Square Flag				(�϶��簢�����)
		case PTN_BULL_TRIANGLEFLAG:		// Bull Triangle Flag			(��»ﰢ�����)
		case PTN_BEAR_TRIANGLEFLAG:		// Bear Triangle Flag			(�϶��ﰢ�����)
		case PTN_BULL_RECTANGLE:		// Bull Rectangle				(������簢��)
		case PTN_BEAR_RECTANGLE:		// Bear Rectangle				(�϶����簢��)
			if ( (pWavePatternData->m_dDrawSeq1_1 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_1 <= nEndIndex) &&
				 (pWavePatternData->m_dDrawSeq1_2 >= nStartIndex) && (pWavePatternData->m_dDrawSeq1_2 <= nEndIndex)	&& 
				 (pWavePatternData->m_dDrawSeq2_1 >= nStartIndex) && (pWavePatternData->m_dDrawSeq2_1 <= nEndIndex) &&
				 (pWavePatternData->m_dDrawSeq2_2 >= nStartIndex) && (pWavePatternData->m_dDrawSeq2_2 <= nEndIndex)	 )
			{
					// Point1
					dDataOffset = pWavePatternData->m_dDrawSeq1_1;
					nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY1 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_1, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					
					// Point2
					dDataOffset = pWavePatternData->m_dDrawSeq1_2;
					nX2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY2 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt1_2, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

					// Point3
					dDataOffset = pWavePatternData->m_dDrawSeq2_1;
					nX3 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY3 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt2_1, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					
					// Point4
					dDataOffset = pWavePatternData->m_dDrawSeq2_2;
					nX4 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
					nY4 = m_pIChartOCX->ConvertDataToYPosition( pWavePatternData->m_dDrawYPt2_2, m_dViewMax, m_dViewMin, 
																rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

					bIsDrawReady = TRUE;
			}
			break;
		}

		if ( !bIsDrawReady)		
		{
			if ( pWavePatternData->m_bIsSelect)
				m_spdDrawingData.m_bDraw = FALSE;
			continue;
		}


		// Get Neck Point
		switch (pWavePatternData->m_nPatternType)
		{
		case PTN_HEADNSHOULDER:			// Head & Shoulder				(�Ӹ������)
		case PTN_R_HEADNSHOULDER:		// Reversal Head & Shoulder		(���Ӹ������)
		case PTN_DOUBLETOP:				// Double Top					(�ֺ���)
		case PTN_R_DOUBLETOP:			// Reversal Double Top			(���ֺ���)
		case PTN_BULL_ISOTRIANGLE:		// Bull Isosceles Triangles		(�����̵�ﰢ��)
		case PTN_BEAR_ISOTRIANGLE:		// Bear Isosceles Triangles		(�༼�̵�ﰢ��)
		case PTN_BULL_TRIANGLE:			// Bull Triangles				(��»ﰢ��)
		case PTN_BEAR_TRIANGLE:			// Bear Triangles				(�϶��ﰢ��)
		case PTN_BULLISH_WIDETRIANGLE:	// Bullish Wide Triangles		(���Ȯ��ﰢ��)
		case PTN_BEARISH_WIDETRIANGLE:	// Bearish Wide Triangles		(�϶�Ȯ��ﰢ��)
		case PTN_BULL_WEDGE:			// Bull Wedge					(���������)
		case PTN_BEAR_WEDGE:			// Bear Wedge					(���������)
		case PTN_BULL_SQUAREFLAG:		// Bull Square Flag				(��»簢�����)
		case PTN_BEAR_SQUAREFLAG:		// Bear Square Flag				(�϶��簢�����)
		case PTN_BULL_TRIANGLEFLAG:		// Bull Triangle Flag			(��»ﰢ�����)
		case PTN_BEAR_TRIANGLEFLAG:		// Bear Triangle Flag			(�϶��ﰢ�����)
		case PTN_BULL_RECTANGLE:		// Bull Rectangle				(������簢��)
		case PTN_BEAR_RECTANGLE:		// Bear Rectangle				(�϶����簢��)
			{
				// Get First Neck Pt
				dDataOffset = pWavePatternData->m_dNeckSeq1;
				nNeckXPos1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
				nNeckYPos1 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckYPt1, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				// Get Second Neck Pt
				dDataOffset = pWavePatternData->m_dNeckSeq2;
				nNeckXPos2 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
				nNeckYPos2 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckYPt2, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

				// Neck Y Length
				nNeckYBase1 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckBaseYPt1, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				nNeckYBase2 = m_pIChartOCX->ConvertDataToYPosition(	pWavePatternData->m_dNeckBaseYPt2, m_dViewMax, m_dViewMin, 
																	rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

				nNeckYLength1 = nNeckYBase2 - nNeckYBase1;
				if (	pWavePatternData->m_nPatternType == PTN_BULLISH_WIDETRIANGLE	|| 
						pWavePatternData->m_nPatternType == PTN_BEARISH_WIDETRIANGLE	|| 
						pWavePatternData->m_nPatternType == PTN_BULL_SQUAREFLAG			||
						pWavePatternData->m_nPatternType == PTN_BEAR_SQUAREFLAG		)
				{
					nNeckYLength2 = nNeckYBase1 - nNeckYBase2;
				}
				else
				{
					nNeckYLength2 = nNeckYLength1;
				}

				// Neck X Length
				nNeckXLength = nNeckXPos2 - nNeckXPos1;
			}
			break;

		case PTN_V_TOP:					// V Top				(�ܺ�õ����)
		case PTN_V_BOTTOM:				// V Bottom				(�ܺ��ٴ���)
			break;
		}


		// Get Pattern Name
		strPatternName = m_wpWavePattern.GetPatternName(pWavePatternData->m_nPatternType);

		// Draw Pattern Line ---------------------------------------------------->
		// Get Pattern Trend & Set Pattern Color
		nPatternTrend = m_wpWavePattern.GetPatternTrend(pWavePatternData->m_nPatternType);
		if ( nPatternTrend == UPWARD_REVERSE)
			p_pDC->SelectObject( &m_pnUpwardReverse);
		else if ( nPatternTrend == DNWARD_REVERSE)
			p_pDC->SelectObject( &m_pnDnwardReverse);
		else if ( nPatternTrend == UPWARD_CONTINUE)
			p_pDC->SelectObject( &m_pnUpwardContinue);
		else if ( nPatternTrend == DNWARD_CONTINUE)
			p_pDC->SelectObject( &m_pnDnwardContinue);
		else
			p_pDC->SelectObject( &m_pnMaPen);


		// Draw Non-Selected Pattern Line
		if ( !pWavePatternData->m_bIsSelect )
		{
			// Draw Pattern Wave(cf. �ĵ���ü�� ���̴°��) 
			if ( (pWavePatternData->m_nStartSeq >= nStartIndex) && (pWavePatternData->m_nStartSeq <= nEndIndex) &&
				 (pWavePatternData->m_nEndSeq >= nStartIndex)	&& (pWavePatternData->m_nEndSeq <= nEndIndex)	)
			{
				nXHigh = nYHigh = nXLow = nYLow = 0;
				pHighLowPtData = NULL;

				for(int k=0; k < m_aryHighLowPtData.GetSize(); k++ )
				{
					pHighLowPtData = m_aryHighLowPtData.GetAt(k);
					
					if ( !pHighLowPtData ) continue;
					
					// *** ���� �������� �ִ� �����͸� �׸���. ***
					if ((pHighLowPtData->m_nDataSeq >= pWavePatternData->m_nStartSeq) && 
						(pHighLowPtData->m_nDataSeq <= pWavePatternData->m_nEndSeq)		)
					{
						// �������� �������
						if ( pHighLowPtData->m_nPointType == CHANGINGPT_HIGHTOLOW && pHighLowPtData->m_dHigh != ZERO)
						{
							// X��ǥ�� ���Ѵ�
							dDataOffset = pHighLowPtData->m_nDataSeq;
							nXHigh = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
							
							// Y��ǥ�� ���Ѵ�
							nYHigh = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtData->m_dHigh, m_dViewMax, m_dViewMin, 
																			rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
						}
						// �������� �������
						else if ( pHighLowPtData->m_nPointType == CHANGINGPT_LOWTOHIGH && pHighLowPtData->m_dLow != ZERO)
						{
							// X��ǥ�� ���Ѵ�
							dDataOffset = pHighLowPtData->m_nDataSeq;
							nXLow = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
							
							// Y��ǥ�� ���Ѵ�
							nYLow = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtData->m_dLow, m_dViewMax, m_dViewMin, 
																			rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
						}
					}

					// ### ����, ���� ������ �׸��� ###
					if (nXHigh > 0 && nYHigh > 0 && nXLow > 0 && nYLow > 0)
					{
						p_pDC->MoveTo( nXHigh, nYHigh);
						p_pDC->LineTo( nXLow, nYLow);
					}
				}
			}


			// Draw Line 1
			if ( nX1 > 0 && nY1 > 0 && nX2 > 0 && nY2 > 0 )
			{
				p_pDC->MoveTo( nX1, nY1);
				p_pDC->LineTo( nX2, nY2);

				if ( pWavePatternData->m_nPatternType == PTN_V_TOP || pWavePatternData->m_nPatternType == PTN_V_BOTTOM)
				{
					p_pDC->SetBkMode(TRANSPARENT);
					rectText.left	= nX2-22;
					rectText.right	= rectText.left+strPatternName.GetLength()*12;
					rectText.top	= nY2+3;
// 2008.02.20 by LYH >>
					//rectText.bottom = rectText.top+11;
					rectText.bottom = rectText.top+nTextHeight;
// 2008.02.20 by LYH <<
					p_pDC->DrawText(strPatternName, rectText, DT_LEFT | DT_TOP);
				}
			}
			// Draw Line2
			if ( nX3 > 0 && nY3 > 0 && nX4 > 0 && nY4 > 0 )
			{
				p_pDC->MoveTo( nX3, nY3);
				p_pDC->LineTo( nX4, nY4);
			}

			// Draw Neck Line1
			if ( nNeckXPos1 > 0 && nNeckYPos1 > 0)
			{
				p_pDC->MoveTo( nNeckXPos1, nNeckYPos1);
				p_pDC->LineTo( nNeckXPos1, nNeckYPos1+nNeckYLength1);
			}
			// Draw Neck Line2
			if ( nNeckXPos2 > 0 && nNeckYPos2 > 0)
			{
				p_pDC->MoveTo( nNeckXPos2, nNeckYPos2);
				p_pDC->LineTo( nNeckXPos2, nNeckYPos2+nNeckYLength2);

				p_pDC->LineTo( nNeckXPos2+nNeckXLength, nNeckYPos2+nNeckYLength2);

				p_pDC->SetBkMode(TRANSPARENT);
				rectText.left	= nNeckXPos2+12;
				rectText.right	= rectText.left+strPatternName.GetLength()*12;
				rectText.top	= nNeckYPos2+nNeckYLength2-14;
// 2008.02.20 by LYH >>
				//rectText.bottom = rectText.top+11;	
				rectText.bottom = rectText.top+nTextHeight;	
// 2008.02.20 by LYH <<
				p_pDC->DrawText(strPatternName, rectText, DT_LEFT | DT_TOP);
			}
		}
		// Set Selected Pattern Data for Drawing
		else
		{
			m_spdDrawingData.m_bDraw		= TRUE;
			m_spdDrawingData.m_nPatternType	= pWavePatternData->m_nPatternType;

			m_spdDrawingData.m_nStartSeq	= pWavePatternData->m_nStartSeq;
			m_spdDrawingData.m_nEndSeq		= pWavePatternData->m_nEndSeq;

			m_spdDrawingData.m_nX1	= nX1;
			m_spdDrawingData.m_nY1	= nY1;
			m_spdDrawingData.m_nX2	= nX2;
			m_spdDrawingData.m_nY2	= nY2;
			m_spdDrawingData.m_nX3	= nX3;
			m_spdDrawingData.m_nY3	= nY3;
			m_spdDrawingData.m_nX4	= nX4;
			m_spdDrawingData.m_nY4	= nY4;

			m_spdDrawingData.m_nNeckXPos1 = nNeckXPos1;
			m_spdDrawingData.m_nNeckYPos1 = nNeckYPos1;
			m_spdDrawingData.m_nNeckXPos2 = nNeckXPos2;
			m_spdDrawingData.m_nNeckYPos2 = nNeckYPos2;

			m_spdDrawingData.m_nNeckXLength  = nNeckXLength;
			m_spdDrawingData.m_nNeckYLength1 = nNeckYLength1;
			m_spdDrawingData.m_nNeckYLength2 = nNeckYLength2;
		}
		// <----------------------------------------------------------------------
	}


	// ************************************************
	// Draw Selected Pattern
	// ************************************************
	if ( m_spdDrawingData.m_bDraw )
	{
		// Get Pattern Name
		strPatternName = m_wpWavePattern.GetPatternName(m_spdDrawingData.m_nPatternType);
		
		// Thick Pen for Selected Pattern
		LOGPEN		logpen;
		CPen		pnSelect;
		COLORREF	clrLine;

		nPatternTrend = m_wpWavePattern.GetPatternTrend(m_spdDrawingData.m_nPatternType);	// Pattern Color
		if ( nPatternTrend == UPWARD_REVERSE)
			clrLine = COLOR_UPWARD_REVERSE;
		else if ( nPatternTrend == DNWARD_REVERSE)
			clrLine = COLOR_DNWARD_REVERSE;
		else if ( nPatternTrend == UPWARD_CONTINUE)
			clrLine = COLOR_UPWARD_CONTINUE;
		else if ( nPatternTrend == DNWARD_CONTINUE)
			clrLine = COLOR_DNWARD_CONTINUE;
		else
			clrLine = COLOR_BLACK;

		p_pDC->GetCurrentPen()->GetLogPen(&logpen);
		logpen.lopnColor = clrLine;
		logpen.lopnWidth.x = 4;
		pnSelect.CreatePenIndirect( &logpen );
		p_pDC->SelectObject( &pnSelect);


		// Bold Font for Selected Pattern
		LOGFONT		logfont;
		CFont		fontBold, *pOldFont=NULL;
		
		p_pDC->GetCurrentFont()->GetLogFont(&logfont);
		logfont.lfHeight = 12;
		logfont.lfWeight = FW_BOLD;
		fontBold.CreateFontIndirect( &logfont);
		pOldFont = p_pDC->SelectObject( &fontBold);
		

		// Draw Pattern Wave
		if ( (m_spdDrawingData.m_nStartSeq >= nStartIndex)	&& (m_spdDrawingData.m_nStartSeq <= nEndIndex) &&
			 (m_spdDrawingData.m_nEndSeq >= nStartIndex)	&& (m_spdDrawingData.m_nEndSeq <= nEndIndex)	)
		{
			nXHigh = nYHigh = nXLow = nYLow = 0;
			pHighLowPtData = NULL;

			for(int k=0; k < m_aryHighLowPtData.GetSize(); k++ )
			{
				pHighLowPtData = m_aryHighLowPtData.GetAt(k);
				
				if ( !pHighLowPtData ) continue;
				
				// *** ���� �������� �ִ� �����͸� �׸���. ***
				if ((pHighLowPtData->m_nDataSeq >= m_spdDrawingData.m_nStartSeq) && 
					(pHighLowPtData->m_nDataSeq <= m_spdDrawingData.m_nEndSeq)		)
				{
					// �������� �������
					if ( pHighLowPtData->m_nPointType == CHANGINGPT_HIGHTOLOW && pHighLowPtData->m_dHigh != ZERO)
					{
						// X��ǥ�� ���Ѵ�
						dDataOffset = pHighLowPtData->m_nDataSeq;
						nXHigh = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
						
						// Y��ǥ�� ���Ѵ�
						nYHigh = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtData->m_dHigh, m_dViewMax, m_dViewMin, 
																		rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					}
					// �������� �������
					else if ( pHighLowPtData->m_nPointType == CHANGINGPT_LOWTOHIGH && pHighLowPtData->m_dLow != ZERO)
					{
						// X��ǥ�� ���Ѵ�
						dDataOffset = pHighLowPtData->m_nDataSeq;
						nXLow = GetXPositoin(dDataOffset, dCandleWidth, rctGraphRegion);
						
						// Y��ǥ�� ���Ѵ�
						nYLow = m_pIChartOCX->ConvertDataToYPosition(	pHighLowPtData->m_dLow, m_dViewMax, m_dViewMin, 
																		rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
					}
				}

				// ### ����, ���� ������ �׸��� ###
				if (nXHigh > 0 && nYHigh > 0 && nXLow > 0 && nYLow > 0)
				{
					p_pDC->MoveTo( nXHigh, nYHigh);
					p_pDC->LineTo( nXLow, nYLow);
				}
			}
		}			


		// Draw Line 1
		if ( m_spdDrawingData.m_nX1 > 0 && m_spdDrawingData.m_nY1 > 0 && m_spdDrawingData.m_nX2 > 0 && m_spdDrawingData.m_nY2 > 0 )
		{
			p_pDC->MoveTo( m_spdDrawingData.m_nX1, m_spdDrawingData.m_nY1);
			p_pDC->LineTo( m_spdDrawingData.m_nX2, m_spdDrawingData.m_nY2);

			if ( m_spdDrawingData.m_nPatternType == PTN_V_TOP || m_spdDrawingData.m_nPatternType == PTN_V_BOTTOM)
			{
				p_pDC->SetBkMode(OPAQUE);
				rectText.left	= m_spdDrawingData.m_nX2-22;
				rectText.right	= rectText.left+strPatternName.GetLength()*13;
				rectText.top	= m_spdDrawingData.m_nY2+3;
// 2008.02.20 by LYH >>
				//rectText.bottom = rectText.top+11;
				rectText.bottom = rectText.top+nTextHeight;
// 2008.02.20 by LYH <<
				p_pDC->DrawText(strPatternName, rectText, DT_LEFT | DT_TOP);

				// Draw Symbol
				rectText.left	= rectText.left-9;
				rectText.right	= rectText.left+8;
				p_pDC->FillSolidRect(rectText, clrLine);
			}
		}
		// Draw Line2
		if ( m_spdDrawingData.m_nX3 > 0 && m_spdDrawingData.m_nY3 > 0 && m_spdDrawingData.m_nX4 > 0 && m_spdDrawingData.m_nY4 > 0 )
		{
			p_pDC->MoveTo( m_spdDrawingData.m_nX3, m_spdDrawingData.m_nY3);
			p_pDC->LineTo( m_spdDrawingData.m_nX4, m_spdDrawingData.m_nY4);
		}

		// Draw Neck Line1
		if ( m_spdDrawingData.m_nNeckXPos1 > 0 && m_spdDrawingData.m_nNeckYPos1 > 0)
		{
			p_pDC->MoveTo(	m_spdDrawingData.m_nNeckXPos1, m_spdDrawingData.m_nNeckYPos1);
			p_pDC->LineTo(	m_spdDrawingData.m_nNeckXPos1, m_spdDrawingData.m_nNeckYPos1+m_spdDrawingData.m_nNeckYLength1);
		}
		// Draw Neck Line2
		if ( m_spdDrawingData.m_nNeckXPos2 > 0 && m_spdDrawingData.m_nNeckYPos2 > 0)
		{
			p_pDC->MoveTo(	m_spdDrawingData.m_nNeckXPos2, m_spdDrawingData.m_nNeckYPos2);
			p_pDC->LineTo(	m_spdDrawingData.m_nNeckXPos2, m_spdDrawingData.m_nNeckYPos2+m_spdDrawingData.m_nNeckYLength2);

			p_pDC->LineTo(	m_spdDrawingData.m_nNeckXPos2+m_spdDrawingData.m_nNeckXLength, m_spdDrawingData.m_nNeckYPos2+m_spdDrawingData.m_nNeckYLength2);

			p_pDC->SetBkMode(OPAQUE);
			rectText.left	= m_spdDrawingData.m_nNeckXPos2+12;
			rectText.right	= rectText.left+strPatternName.GetLength()*13;
			rectText.top	= m_spdDrawingData.m_nNeckYPos2+m_spdDrawingData.m_nNeckYLength2-14;
// 2008.02.20 by LYH >>
			//rectText.bottom = rectText.top+11;
			rectText.bottom = rectText.top+nTextHeight;
// 2008.02.20 by LYH <<
			p_pDC->DrawText(strPatternName, rectText, DT_LEFT | DT_TOP);

			// Draw Symbol
			rectText.left	= rectText.left-9;
			rectText.right	= rectText.left+8;
			p_pDC->FillSolidRect(rectText, clrLine);
		}


		// Delete Pen
		if ( pnSelect.GetSafeHandle())
			pnSelect.DeleteObject();

		// Delete Bold Font
		if ( pOldFont)
			p_pDC->SelectObject( pOldFont);
		if ( fontBold.GetSafeHandle())
			fontBold.DeleteObject();
	}
	// **************************************************************************************************(4_END)
}

void CPatternMaker::DrawCandlePattern( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion)
{
	// Draw Candle Pattern
	int		nX1, nY1, nX2, nY2;
	int		nX3, nY3, nX4, nY4;
	int		nNeckXPos1, nNeckYPos1, nNeckXPos2, nNeckYPos2;
	int		nNeckYLength1, nNeckYLength2, nNeckXLength;
	CCandlePatternData* pCandlePatternData = NULL;
	int nPointCnt = m_aryCandlePatternData.GetSize();
	CCandlePatternPosInfo* pInfo = NULL;
	BOOL	bIsDrawReady;
	CString	strPatternName;
	int dDataOffset = 0;
	CRect	rectText;

	//m_aryCandlePatternPosInfo.RemoveAll();
	DeleteCandlePatternPosInfo();
	BOOL bDisplay;
	for (int i = 0; i < nPointCnt; i++)
	{
		// ��� ������ �׸��� �ʴ� ���
		//if( m_PatternEnvData.m_bNotDrawAll) break;

		nX1 = nY1 = nX2 = nY2 = 0;
		nX3 = nY3 = nX4 = nY4 = 0;
		nNeckXPos1 = nNeckYPos1 = nNeckXPos2 = nNeckYPos2 = 0;
		nNeckYLength1 = nNeckYLength2 = nNeckXLength = 0;
		bIsDrawReady	= FALSE;
		strPatternName	= _T("");

		pCandlePatternData = m_aryCandlePatternData.GetAt(i);
		if ( !pCandlePatternData)	continue;

		bDisplay = m_PatternEnvData.IsDisplayCandle(pCandlePatternData->m_nCandlePtnType);
		if(!bDisplay)
			continue;

		if ( //(pCandlePatternData->m_nStartPos >= nStartIndex) && (pCandlePatternData->m_nStartPos <= nEndIndex) &&
			(pCandlePatternData->m_nEndPos >= nStartIndex) && (pCandlePatternData->m_nEndPos <= nEndIndex)	)
		{
			int iTrust = m_cpCandlePattern.GetCandlePatternTrust(pCandlePatternData->m_nCandlePtnType);
			//dDataOffset = pCandlePatternData->m_nEndPos;
			dDataOffset = pCandlePatternData->m_nEndPos;
			nX1 = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
			if(iTrust == UPWARD_LEVEL1 || iTrust == UPWARD_LEVEL2 || iTrust == UPWARD_LEVEL3)
			{
				nY1 = m_pIChartOCX->ConvertDataToYPosition( pCandlePatternData->m_dLow, m_dViewMax, m_dViewMin, 
					rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				rectText.top	= nY1+10;
			}else
			{
				nY1 = m_pIChartOCX->ConvertDataToYPosition( pCandlePatternData->m_dHigh, m_dViewMax, m_dViewMin, 
					rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
				rectText.top	= nY1-18;
			}
			CString strPatternName = "";
			if(m_PatternEnvData.m_bDrawPatternName)
				strPatternName = m_cpCandlePattern.GetCandlePatternName(pCandlePatternData->m_nCandlePtnType);

			rectText.left	= nX1-6;
			rectText.right	= rectText.left+12 + strPatternName.GetLength()*12;

// 2008.02.20 by LYH >>
			IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd);
			if(eLanguageID == IMetaTable::ENGLISH)
				rectText.bottom = rectText.top+13;	
			else
				rectText.bottom = rectText.top+11;	
// 2008.02.20 by LYH <<
			p_pDC->SetBkMode(TRANSPARENT);

			ML_SET_LANGUAGE_RES();
			CString strN;
			switch(iTrust)
			{
			case UPWARD_LEVEL3:
				p_pDC->SetTextColor(COLOR_UPWARD_CONTINUE);
				strN.LoadString( IDS_N3);
				p_pDC->DrawText( strN + strPatternName, rectText, DT_LEFT | DT_TOP);
				break;
			case UPWARD_LEVEL2:
				p_pDC->SetTextColor(COLOR_UPWARD_CONTINUE);
				strN.LoadString( IDS_N2);
				p_pDC->DrawText( strN + strPatternName, rectText, DT_LEFT | DT_TOP);
				break;
			case UPWARD_LEVEL1:
				p_pDC->SetTextColor(COLOR_UPWARD_CONTINUE);
				strN.LoadString( IDS_N1);
				p_pDC->DrawText( strN + strPatternName, rectText, DT_LEFT | DT_TOP);
				break;
			case DNWARD_LEVEL3:
				p_pDC->SetTextColor(COLOR_DNWARD_CONTINUE);
				strN.LoadString( IDS_N3);
				p_pDC->DrawText( strN + strPatternName, rectText, DT_LEFT | DT_TOP);
				break;
			case DNWARD_LEVEL2:
				p_pDC->SetTextColor(COLOR_DNWARD_CONTINUE);
				strN.LoadString( IDS_N2);
				p_pDC->DrawText( strN + strPatternName, rectText, DT_LEFT | DT_TOP);
				break;
			case DNWARD_LEVEL1:
				p_pDC->SetTextColor(COLOR_DNWARD_CONTINUE);
				strN.LoadString( IDS_N1);
				p_pDC->DrawText( strN + strPatternName, rectText, DT_LEFT | DT_TOP);
				break;

			}
			pInfo = new CCandlePatternPosInfo;
			pInfo->m_rect = rectText;
			pInfo->m_nCandlePtnType = pCandlePatternData->m_nCandlePtnType;
			m_aryCandlePatternPosInfo.Add(pInfo);
		}
	}

	// **************************************************************************************************(5_END)
}