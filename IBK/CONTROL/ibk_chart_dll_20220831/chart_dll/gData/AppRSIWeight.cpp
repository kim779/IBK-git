// AppRSIWeight.cpp: implementation of the CAppRSIWeight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppRSIWeight.h"
#include "OrgData.h"
#include "DataMgr.h"
#include "../../h/axisgobj.h"
#include "../../h/axisgenv.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppRSIWeight::CAppRSIWeight(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption)
	: CLineData(pOrgData, dKey, gKind, pCInfo, cOption)
{
	m_iNLine = 1;

	for ( int ii = 0 ; ii < m_iNLine ; ii++ )
	{
		m_apdVal[ii] = new double[m_iDataSize];
	}
	
	Calculate(0,0);
}

CAppRSIWeight::~CAppRSIWeight()
{

}

//
// �����̵������ ����� RSI(Relative Strength Index)�� ����Ѵ�.
//
bool CAppRSIWeight::CalcuData(bool bShift, bool bLast)
{
	if (m_iDataSize <= 0)
		return false;
	
	int	ii = 0, jj = 0, kk = 0, nStart = -1, nCount = 0;
	double	val = 0.0, pVal = GD_UNUSED;
	CGrpBasic* gBasic = NULL;

	// nday�� �̵������ ����ϴ� ����
	int	nday = (int)m_pCInfo->awValue[0];
	int	nMim = nday + 1;

	m_iNSignal = (int)m_pCInfo->awValue[1];

	if (nday <= 0)
		return false;

	int	nPos = 0, nDataSize = m_iDataSize;

	// 2009.10.16 : DELETE
	//if (bLast)
	//{
	//	nDataSize = nMim;
	//	nPos = m_iDataSize - nDataSize;
	//}

	if (nPos < 0)	return false;

	for ( ii = 0 ; ii < nDataSize ; ii++ )
	{		
		if (nStart < 0)
		{
			gBasic = m_pOrgData->GetGraphData(ii + nPos);

			if (gBasic->m_iClose != GD_UNUSED)
				nStart = ii;
		}
	}

	// ���� RTS �����ͷ� ����ϴ� ��� ����� ���� AG, AL�� �̿��Ͽ� ����Ѵ�.
	if (true == bLast)
	{
		double dMovingAvergeOfIncreasing;
		double dMovingAvergeOfDecreasing;
		double dPresentData, dPreviousData;
		double dIncreasing, dDecreasing;
		double RS;
		int    nLastPosition = nDataSize - 1;	// �� ������ �������� ���ؽ� ��

		gBasic = m_pOrgData->GetGraphData(nLastPosition);
		dPresentData = (double)gBasic->m_iClose;
		gBasic = m_pOrgData->GetGraphData(nLastPosition - 1);
		dPreviousData = (double)gBasic->m_iClose;

		if (dPresentData > dPreviousData)
		{
			dIncreasing = dPresentData - dPreviousData;
			dDecreasing = 0;
		}
		else
		{
			dIncreasing = 0;
			dDecreasing = dPreviousData - dPresentData;
		}

		// �к�, �Ϻ� ���� �����Ͱ� ���� �ϼ��� �ȵ� �� ���� AG, AL�� �����Ͽ� �����Ѵ�.
		if (true == bShift)
		{
			m_dGainAveragePreviousInterval = m_dPreviousGainAverage;
			m_dLossAveragePreviousInterval = m_dPreviousLossAverage;
		}
	
		// ���� ������ AL, AG�� ����Ͽ� ���� AL, AG�� ���Ѵ�.
		dMovingAvergeOfIncreasing = m_dGainAveragePreviousInterval;
		dMovingAvergeOfDecreasing = m_dLossAveragePreviousInterval;
	
		dMovingAvergeOfIncreasing = (dMovingAvergeOfIncreasing * (nday - 1) + dIncreasing) / nday;
		dMovingAvergeOfDecreasing = (dMovingAvergeOfDecreasing * (nday - 1) + dDecreasing) / nday;

		// ���� AG, AL�� �����Ͽ� �����Ѵ�.
		m_dPreviousGainAverage = dMovingAvergeOfIncreasing;
		m_dPreviousLossAverage = dMovingAvergeOfDecreasing;

		if (dMovingAvergeOfDecreasing == 0.0 )
		{
			// dMovingAvergeOfDecreasing�� ���� ��� RSI�� 100 �̴�
			m_apdVal[0][nLastPosition] = 100.0 * 100.0;
		}
		else
		{
			RS = dMovingAvergeOfIncreasing / dMovingAvergeOfDecreasing;
			m_apdVal[0][nLastPosition] = (100.0 - (100.0 / (1.0 + RS))) * 100.0;
		}

		return true;
	}

	// ��ȸ �����ͷ� ����Ѵ�.
	if (nStart >= 0)
	{
		nCount = nDataSize - nStart;

		if (nCount >= nMim)
		{
			//double	upSum = 0.0, dnSum = 0.0, 
			double RS = 0.0;
			nStart = nStart + nday;
			
			for ( ii = nStart ; ii < nDataSize ; ii++ )
			{
				double dMovingAvergeOfIncreasing;
				double dMovingAvergeOfDecreasing;
				double dPresentData, dPreviousData;
				double dIncreasing, dDecreasing;

				// ù �������� ��� ������, ���Һ��� �ܼ� �̵� ����� ���Ѵ�.
				if (ii == nStart)
				{
					int nDataCount;
					double dSumOfIncreasing = 0;
					double dSumOfDecreasing = 0;

					for (nDataCount = nday; nDataCount > 0; nDataCount--)
					{
						gBasic = m_pOrgData->GetGraphData(ii + nPos - nDataCount + 1);
						dPresentData = (double)gBasic->m_iClose;
						gBasic = m_pOrgData->GetGraphData(ii + nPos - nDataCount);
						dPreviousData = (double)gBasic->m_iClose;
						if (dPresentData > dPreviousData)
						{
							dSumOfIncreasing += dPresentData - dPreviousData;
						}
						else
						{
							dSumOfDecreasing += dPreviousData - dPresentData;
						}
					}

					dMovingAvergeOfIncreasing = dSumOfIncreasing / nday;
					dMovingAvergeOfDecreasing = dSumOfDecreasing / nday;
				}
				else
				{
					// �ι�° �����ͺ��� ���������� ���Ѵ�.
					gBasic = m_pOrgData->GetGraphData(ii + nPos);
					dPresentData = (double)gBasic->m_iClose;
					gBasic = m_pOrgData->GetGraphData(ii + nPos - 1);
					dPreviousData = (double)gBasic->m_iClose;

					if (dPresentData > dPreviousData)
					{
						dIncreasing = dPresentData - dPreviousData;
						dDecreasing = 0;
					}
					else
					{
						dIncreasing = 0;
						dDecreasing = dPreviousData - dPresentData;

					}

					dMovingAvergeOfIncreasing = (dMovingAvergeOfIncreasing * (nday - 1) + dIncreasing) / nday;
					dMovingAvergeOfDecreasing = (dMovingAvergeOfDecreasing * (nday - 1) + dDecreasing) / nday;
				}

				if (dMovingAvergeOfDecreasing == 0.0 )
				{
					// dMovingAvergeOfDecreasing�� ���� ��� RSI�� 100 �̴�
					m_apdVal[0][ii + nPos] = 100.0 * 100.0;
				}
				else
				{
					RS = dMovingAvergeOfIncreasing / dMovingAvergeOfDecreasing;
					m_apdVal[0][ii + nPos] = (100.0 - (100.0 / (1.0 + RS))) * 100.0;
				}

				// AG, AL�� �����Ͽ� �����Ѵ�.
				/*
				m_dPreviousGainAverage = dMovingAvergeOfIncreasing;
				m_dPreviousLossAverage = dMovingAvergeOfDecreasing;
				m_dGainAveragePreviousInterval = dMovingAvergeOfIncreasing;
				m_dLossAveragePreviousInterval = dMovingAvergeOfDecreasing;
				*/
				m_dGainAveragePreviousInterval = m_dPreviousGainAverage;
				m_dLossAveragePreviousInterval = m_dPreviousLossAverage;
				m_dPreviousGainAverage = dMovingAvergeOfIncreasing;
				m_dPreviousLossAverage = dMovingAvergeOfDecreasing;
			}				
			if (!bLast || m_aiPIndex[0] == -1)
			{
				m_aiPIndex[0] = nStart;
				m_aiNLimit[0] = nMim - 1;
			}
		}
	}

	return true;
}
