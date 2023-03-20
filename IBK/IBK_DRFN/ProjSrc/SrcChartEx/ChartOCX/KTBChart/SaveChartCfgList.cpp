// SaveChartCfgList.cpp: implementation of the CSaveChartCfgList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveChartCfgList.h"

#include "ChartCfg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveChartCfgList::CSaveChartCfgList()
{

}

CSaveChartCfgList::~CSaveChartCfgList()
{
	RemoveAll();
}

bool CSaveChartCfgList::AddChartCfg(	CPropertyMap *p_pAddInEnvMap,
										const int nRowCount, const int nColumnCount, 
										const int nRowDoubleSizeBlock, const int nColumnDoubleSizeBlock,
										const int nOnePageDataCount, const int nBongCountMargin, 
										const CString& strChartList, const CString& strBlockRegion,
										const BOOL bWholeView, 
										const int nNumericalInquiryOption,
										const int nYScalePosition,
										const int nUseInquiryWithCrossline,
										const BOOL bToolTipwithZoomIn,
										const BOOL bUseConditiontoTitle,
										const BOOL bUseDatatoTitle,
										const int nVertGap, const int nHorzGap,
										const BOOL bShowMinMaxbyPrice, /*const bool bUseFullMinMax,*/	//������ �ٸ�
										const BOOL bAllGraphTitleHiding, 
										const BOOL bShowTrendLineText, const BOOL bShowVerticalText, 
										const BOOL bShowHorizonTextLeft, const BOOL bShowHorizonTextRight,// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
										const BOOL bShowThreeText, const BOOL bShowFourText, const BOOL bShowFiboText,
										const COLORREF clrBackground, const COLORREF clrBackBorder,
										const COLORREF clrBlock, const COLORREF clrBlockBorder, 
										const COLORREF clrScaleLine, const COLORREF clrScaleLine2, const COLORREF clrScaleText, 
										const COLORREF clrAnalysisTool, const COLORREF clrInquiryWindow, const COLORREF clrLattice,
										// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
										BOOL p_bShowPriceMinMaxText,
										// (2006/12/10 - Seung-Won, Bae) Support Map Default Info.
										const char *p_szMapIndicatorInfo,
										BOOL bDrawBaseLine,		// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
										const int p_nVertScaleType,
										const long p_nRightMargin,	// (2009/5/13 - Seung-Won, Bae) for Right Margin
										const long p_nFutureTimeCount,		// (2009/5/17 - Seung-Won, Bae) for Future Time
										const int p_nMouseWheelIndex,
										const int p_nMinMaxRatio
										)
{
	CChartCfg * pChart = new CChartCfg(	p_pAddInEnvMap,
										nRowCount, nColumnCount, 
										nRowDoubleSizeBlock, nColumnDoubleSizeBlock,
										nOnePageDataCount, nBongCountMargin, 
										strChartList, strBlockRegion,
										(bWholeView == TRUE), 
										nNumericalInquiryOption, //sy 2005.12.01 drag ���� ����(��ġ��ȸ/Ȯ�����)
										nYScalePosition,
										nUseInquiryWithCrossline, (bToolTipwithZoomIn == TRUE),
										(bUseConditiontoTitle == TRUE), (bUseDatatoTitle == TRUE),
										nVertGap, nHorzGap, 
										(bShowMinMaxbyPrice == TRUE), /*bUseFullMinMax,*/ //������ �ٸ�
										(bAllGraphTitleHiding == TRUE), 
										(bShowTrendLineText == TRUE), (bShowVerticalText == TRUE),
										(bShowHorizonTextLeft == TRUE), (bShowHorizonTextRight == TRUE),// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
										(bShowThreeText == TRUE), (bShowFourText == TRUE), (bShowFiboText == TRUE),
										clrBackground, clrBackBorder,
										clrBlock, clrBlockBorder, 
										clrScaleLine, clrScaleLine2, clrScaleText, 
										clrAnalysisTool, clrInquiryWindow, clrLattice,
										p_bShowPriceMinMaxText,
										p_szMapIndicatorInfo,
										bDrawBaseLine,			// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
										p_nVertScaleType,
										p_nRightMargin,		// (2009/5/13 - Seung-Won, Bae) for Right Margin
										p_nFutureTimeCount,		// (2009/5/17 - Seung-Won, Bae) for Future Time
										p_nMouseWheelIndex,
										p_nMinMaxRatio
										);

	m_lstChartCfg.AddTail(pChart);
	return true;
}

void CSaveChartCfgList::RemoveAll()
{
	POSITION pos = m_lstChartCfg.GetHeadPosition();
	while(pos)	{
		CChartCfg * pChart = m_lstChartCfg.GetNext(pos);
		if(pChart)
			delete pChart;
		pChart = NULL;
	}
	m_lstChartCfg.RemoveAll();
}

//sy 2004.12.06.
bool CSaveChartCfgList::RemoveTail()
{
	CChartCfg * pChart = m_lstChartCfg.GetTail();
	if(pChart == NULL)
		return false;

	delete pChart;
	m_lstChartCfg.RemoveTail();
	return true;
}
//sy end

// ----------------------------------------------------------------------------
int CSaveChartCfgList::GetCfgCount()
{
	return m_lstChartCfg.GetCount();
}

POSITION CSaveChartCfgList::GetHeadPosition() const
{
	return m_lstChartCfg.GetHeadPosition();
}

POSITION CSaveChartCfgList::GetTailPosition() const
{
	return m_lstChartCfg.GetTailPosition();
}

//sy 2004.12.06.
CChartCfg* CSaveChartCfgList::GetChartCfg(const int nIndex) const
{
	if(m_lstChartCfg.GetCount() <= 0)
		return NULL;
	if(m_lstChartCfg.GetCount() == 1)
		return m_lstChartCfg.GetTail();

	if(nIndex < m_lstChartCfg.GetCount() - 1)
		return m_lstChartCfg.GetAt(m_lstChartCfg.FindIndex(nIndex));

	return m_lstChartCfg.GetTail();
}
//sy end

//sy 2005.01.14.
CChartCfg* CSaveChartCfgList::GetChartCfg(const int nIndex, const bool bDelete)
{
	CChartCfg* pChartCfg = GetChartCfg(nIndex);
	// �޺κ��� �����Ѵ�.
	if(pChartCfg != NULL && bDelete)
	{
		int nDeleteStartIndex = GetIndex(pChartCfg);
		RemoveFromIndexToEnd(nDeleteStartIndex);
	}

	return pChartCfg;
}
//sy end

CChartCfg* CSaveChartCfgList::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_lstChartCfg.GetNext(pos);
}

CChartCfg* CSaveChartCfgList::GetPrev(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_lstChartCfg.GetPrev(pos);
}

// �ش� ��ǥ�� �ִ� chartCfg ã��
bool CSaveChartCfgList::GetChartCfg(const CString& strGraphName, CChartCfg*& pChartCfg, int& nIndex) const
{
	if(strGraphName.IsEmpty())
		return false;

	pChartCfg = NULL;
	nIndex = -1;

	POSITION pos = m_lstChartCfg.GetHeadPosition();
	for(int i = 0; pos != NULL; i++)
	{
		CChartCfg* pChart = m_lstChartCfg.GetNext(pos);
		if(pChart == NULL)
			break;

		CString strChartList = pChart->GetChartList();
		if(strChartList.Find(strGraphName + ';') > 0)
		{
			pChartCfg = pChart;
			nIndex = i;
			return true;
		}
	}

	return false;
}

// (2004.05.20, ��¿�) DLL Container���� ������ Packet �ʱ�ȭ ���±����� RollBack ��Ű�� ����� �߰��Ѵ�.
//		nIndex��° ����, �� ������ ��� ������ Clear ��Ű�� Interface�� �����Ѵ�.
// nIndex ~ tail ����.
bool CSaveChartCfgList::RemoveFromIndexToEnd(const int nIndex)
{
	// 1. ���� Index�� ��ȿ���� Ȯ���Ѵ�.
	int nCfgCount = GetCfgCount();
	if( nIndex < 0 || nCfgCount <= nIndex)
		return false;

	// 2. Index������ CFG Item�� �����Ѵ�.
	bool bResult = false;
	while( nIndex < nCfgCount - 1)
	{
		delete m_lstChartCfg.RemoveTail();
		nCfgCount--;
		bResult = true;
	}

	return bResult;
}

// ============================================================================
int CSaveChartCfgList::GetIndex(CChartCfg* pChartCfg) const
{
	if(pChartCfg == NULL)
		return -1;

	int nIndex = 0;
	POSITION pos = m_lstChartCfg.GetHeadPosition();
	while(pos != NULL)
	{
		if(pChartCfg == m_lstChartCfg.GetNext(pos))
			return nIndex;
		nIndex++;
	}
	return -1;
}
