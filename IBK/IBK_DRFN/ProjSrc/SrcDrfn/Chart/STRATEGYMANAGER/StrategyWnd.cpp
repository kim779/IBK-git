// StrategyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "StrategyWnd.h"
#include "LogicOperation.h"
#include "LogicST.h"
#include "../Common_ST/STConfigDef.h"
#include "../Common_ST/Commonlib.h"
#include "DealCal.h"
#include "DealCalIndexCrossAboveBasis.h"
#include "DealCalIndexCrossBelowBasis.h"

#include "DealCalIndexUpwardReversal.h"
#include "DealCalIndexDownwardReversal.h"
#include "DealCalIndexCrossAboveAnother.h"
#include "DealCalIndexCrossBelowAnother.h"

#include "FeeCal.h"
#include "FeeCalByPercent.h"
#include "FeeCalByPrice.h"

#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");
//const UINT RMSG_STCONFIGURENAME = ::RegisterWindowMessage("RMSG_STCONFIGURENAME");
//const UINT RMSG_STCONTROLLER_POINTER = ::RegisterWindowMessage("RMSG_STCONTROLLER_POINTER");

#define ID_FORCEDEXITBYTIME	100

//////////////////////////////////////////////////////////////////////////
// CForcedLiquidationInfo

#define SENTINEL_FLINFO		9999
CForcedLiquidationInfo::CForcedLiquidationInfo()
: m_nExitIndex(SENTINEL_FLINFO), m_lDealValue(0L), m_pDataItem(NULL)
{
}

void CForcedLiquidationInfo::SetFLInfo(int nExitIndex, long lDealValue, CIDataItem* pDataItem)
{
	// ���� �߻��� ����û�� Index�� ����
	if(m_nExitIndex > nExitIndex)
	{
		m_nExitIndex = nExitIndex;
		m_lDealValue = lDealValue;
		m_pDataItem = pDataItem;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CStrategyWnd

CStrategyWnd::CStrategyWnd()
{
	m_pLogicRootItem = NULL;

	m_nCurrentRunningLogic = 0;
	m_bIsRunning = FALSE;
	m_bHasAPData = FALSE;
	m_pDateItem = NULL;
	m_lKey = 0;
	m_lDataCnt = 0;
	m_Status = STRATEGY_NONE;

	m_dUpperPrice =0.;
	m_dLowerPrice =0.;
	m_dVolForOrder = 0.;
	m_dSumProfit = 0.;
	m_dSumLoss = 0.;
	m_dMaxProfit = 0.;
	m_dMaxLoss = 0.;
	m_dSumAmountExit = 0.;
	m_lCntProfit = 0;
	m_lCntLoss = 0;
	m_lCntProfitStraight = 0;
	m_lCntLossStraight = 0;
	m_lSumProfitStraight = 0;
	m_lSumLossStraight = 0;
	m_lSumCandleProfit = 0;
	m_lSumCandleLoss = 0;

	m_lCntCandleAccess = 0;
	m_strategyType = NONE;
	m_lCurrentDealPosition = 0;
	m_dCurrentProfitRate = 0.;
	m_lAllCandleInCalculation = 0;
	m_nLastSize = 0;
	m_pFeeCal = NULL;
	m_pSlippageCal = NULL;
	m_pChartInterface = NULL;
	InitConfiguration();

	m_dSellOption = 0.;
	m_dBuyOption = 0.;

	m_nTimerID = 0;

	// vntsorl
	m_bBuyDealTraceInit = FALSE;
	m_bSellDealTraceInit = FALSE;
	m_bBuyOrderStartSignal = FALSE;
	m_bSellOrderStartSignal = FALSE;
}

CStrategyWnd::~CStrategyWnd()
{
	RemoveAllArray();
}


BEGIN_MESSAGE_MAP(CStrategyWnd, CWnd)
	//{{AFX_MSG_MAP(CStrategyWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStrategyWnd message handlers

int CStrategyWnd::Create(CWnd* pParentWnd, long lKey, CStringArray &rStArrayPath,long pChartItem)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(!pParentWnd)
		return -1;

	m_pChartInterface = pChartItem;
	m_lKey = lKey;
	m_strArrayPath.RemoveAll();
	int nSize = rStArrayPath.GetSize();
	if(nSize!=2) return -1;
	for(int nPos=0;nPos<nSize;nPos++)
		m_strArrayPath.Add(rStArrayPath.GetAt(nPos));

	m_logicMaker2.SetArrayPath(&m_strArrayPath);
	return CWnd::Create(NULL,"StrategyWnd",WS_CHILD,CRect(0,0,0,0),pParentWnd,0x1000);	
}

BOOL CStrategyWnd::DestoryWindow()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CWnd::DestroyWindow();
}

void CStrategyWnd::OnDestroy() 
{
	Init();	
	CWnd::OnDestroy();	
}


void CStrategyWnd::Init()
{
//	m_CriticalSection.Lock();
	m_pDateItem = NULL;
	m_strategyType = NONE;
	m_Status = STRATEGY_NONE;
	m_bHasAPData = FALSE;
	m_lDataCnt = 0;
	m_strInputCode = "";
	if(m_nTimerID) KillTimer(m_nTimerID);

	if(m_pFeeCal)
	{
		delete m_pFeeCal;
		m_pFeeCal = NULL;
	}
	if(m_pSlippageCal)
	{
		delete m_pSlippageCal;
		m_pSlippageCal = NULL;
	}

	if(m_pLogicRootItem)
	{
		if(m_pLogicRootItem->GetType()==2)
		{
			CLogicOperation* pLogicItem = (CLogicOperation*)m_pLogicRootItem;
			//delete m_pLogicRootItem;
			delete pLogicItem;
			m_pLogicRootItem = NULL;
		}
		else
		{
			CLogicST * pLogicItem = (CLogicST*)m_pLogicRootItem;
			//delete m_pLogicRootItem;
			delete pLogicItem;
			m_pLogicRootItem = NULL;
		}
	}
//	m_CriticalSection.Unlock();
}

//��ǥ����ȭ������ ���Ǵ� �Լ��̴�.(KHD)
BOOL CStrategyWnd::LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption)
{
	Init();		
	CLogicST *pLogicST = new CLogicST;

	BOOL bRetValue = pLogicST->Init(lpStrategyDllPath,0,TRUE,lpTimeTypeNum,NULL,this->GetSafeHwnd());
	if(!bRetValue) 	
	{
		delete pLogicST;
		pLogicST = NULL;
		return FALSE;
	}
	m_strStrategyPath = lpStrategyDllPath;
	m_pLogicRootItem = pLogicST;	
	m_strategyType = ST_SD;
	m_lDataCnt = lDataCnt;
	m_strInputCode = lpInputCode;
	if(lpEndDate==NULL)
	{
		CTime t = CTime::GetCurrentTime();
		m_strEndDate = t.Format("%Y%m%d");	
	}
	else
	{
		m_strEndDate = lpEndDate;
	}

	int nShifter = 0;
	int nPos = 0;
	CString strDataDivided;
	CString strTitle;
	CString strContents;
	if(lpTypeDealOption)
	{
		while(1)
		{
			strDataDivided = Parser(lpTypeDealOption,&nShifter);
			if(strDataDivided.IsEmpty()) break;
			nPos = strDataDivided.Find('=');
			if(nPos!=-1)
			{
				strTitle = strDataDivided.Left(nPos);
				strContents = strDataDivided.Right(strDataDivided.GetLength()-nPos-1);
			}
			else
			{
				continue;
			}
			/////////////////////////////////////////
			if(strTitle==OPT_SELL)
			{
				m_strTypeSell = strContents;	
			}
			if(strTitle==OPT_BUY)
			{
				m_strTypeBuy = strContents;
			}
			if(strTitle==OPT_SELL_OPTION)
			{
				m_dSellOption = atof(strContents);
			}
			if(strTitle==OPT_BUY_OPTION)
			{
				m_dBuyOption = atof(strContents);
			}			
		}
	}
	return TRUE;
}

BOOL CStrategyWnd::LoadST(LPCTSTR lpStrategyPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate)
{
	if(!lpStrategyPath||!lpInputCode||lDataCnt<=0) return FALSE;
//	Init();

	m_strStrategyPath = lpStrategyPath;//�ε�� ST ����

	m_dealDataItem.Clear();
	LoadConfiguration();

	m_lDataCnt = lDataCnt;//Data ����
	m_strInputCode = lpInputCode;//�����ڵ�
	if(m_strStrategyPath.GetLength()==0||m_strInputCode.GetLength()==0) return FALSE;
	if(lpEndDate==NULL)
	{
		CTime t = CTime::GetCurrentTime();
		m_strEndDate = t.Format("%Y%m%d");	
	}
	else
	{
		m_strEndDate = lpEndDate;
	}
	//�������� ���� ���� ���� Ȯ��////////////////////
	CFileStatus status;
	BOOL bDLLIsExist = CFile::GetStatus(lpStrategyPath,status);
	if(!bDLLIsExist) 
		return FALSE;

	char szBuffer[256];
	::GetPrivateProfileString(STRATEGYLIST, STRATEGYLOGIC, "",szBuffer, sizeof(szBuffer), m_strStrategyPath);
	CString strAllLogic = szBuffer;

	CString strAllLogicConverted;
	char szType[2];
	char szValue[2];
	int nType = 0;
	int nValue = 0;
	int nOffsetOfAllLogic = 0;
	int nLengthOfAlllogic = strAllLogic.GetLength();	
	memset(szType,0x00,sizeof(szType));
	memset(szValue,0x00,sizeof(szValue));
	char szIndex[2] = "A";
	while(1)
	{
		if(nOffsetOfAllLogic>=nLengthOfAlllogic)	break;
		szType[0] = strAllLogic[nOffsetOfAllLogic];
		nType = atol(szType);
		nOffsetOfAllLogic++;

		if(nOffsetOfAllLogic>=nLengthOfAlllogic)	break;
		szValue[0] = strAllLogic[nOffsetOfAllLogic];
		nValue = atol(szValue);
		nOffsetOfAllLogic++;
		switch(nType)
		{
		case OPERATIONINLOGIC:// ������ ���� 
			if(nValue==NEXTINLOGIC)
			{
				strAllLogicConverted += ">";
			}
			else if(nValue==ORINLOGIC)
			{
				strAllLogicConverted += "|";
			}
			else
			{
				strAllLogicConverted += "&";
			}
			break;
		case PARENTHESISINLOGIC:
			if(nValue==OPENPARENTHESISINLOGIC)
			{
				strAllLogicConverted += "(";
			}
			else
			{
				strAllLogicConverted += ")";
			}
			break;
		default:	//ELEMENTINLOGIC
			if(!nValue)
			{
				strAllLogicConverted += "!";
			}
			strAllLogicConverted += szIndex;
			szIndex[0]++;// �����ڰ� ������ A���� ������Ų��. 
			break;
		}
		
	}//������� ����� ����. EX) 112111 �̸� A|B��� ��������� ����ȴ�. : KHD
	if(!strAllLogicConverted.GetLength()) 
	{
		m_strError.Format("�ش� ���������� ���������� �������� �ʽ��ϴ�.");
		return FALSE;
	}

	//�����ڸ� ���� ���� A|B��� �� �ش��ϴ� ����SD������ �ε��ϰ� �����ڸ� ������ CLogicOperation Ŭ������ �ּҸ� ��ȯ�Ѵ�.
	m_pLogicRootItem = m_logicMaker2.Evaluates(strAllLogicConverted,m_strStrategyPath,this->GetSafeHwnd(),m_pChartInterface);//�θ�� �ٷ� �����޴���..

	if(!m_pLogicRootItem) return FALSE;
	m_strategyType = ST_ST;
	return TRUE;
}

BOOL CStrategyWnd::Stop()
{
	BOOL bRetValue = FALSE;
	if(m_pLogicRootItem)
	{
		bRetValue = m_pLogicRootItem->Stop();
	}
	if(bRetValue)
	{
		m_Status = STRATEGY_STOP;
		m_bHasAPData = FALSE;
	}
	return bRetValue;
}


long CStrategyWnd::Update(long lPos, BOOL bIsUpdateData)
{
	long lRetValue = 0;
	if(m_pLogicRootItem)
	{
		lRetValue = m_pLogicRootItem->Update(lPos, bIsUpdateData);
	}

	return lRetValue;
}
// return value: 0 = Fail, 1 = Normal Execution, 2 = Normal Execution but Raw data exists, 3 = execution after pause, 4 = cfg execuation
BOOL CStrategyWnd::Run(LPCTSTR lpOption)
{
	long lRetValue = 0;
	if(m_pLogicRootItem) 
	{
		lRetValue = m_pLogicRootItem->Run(m_strInputCode,m_lDataCnt,m_strEndDate,lpOption);			
	}
	if(lRetValue==0) return FALSE;
	if(lRetValue==3)
	{
		m_Status = STRATEGY_RUN;
	}
	else
	{
		m_Status = STRATEGY_STANDBY;
	}
	return TRUE;
}

BOOL CStrategyWnd::Pause()
{
	BOOL bRetValue = FALSE;
	if(m_pLogicRootItem)
	{
		bRetValue = m_pLogicRootItem->Pause();
	}
	if(bRetValue)	m_Status = STRATEGY_PAUSE;
	return bRetValue;
}

OPERATTIONSTATUS CStrategyWnd::GetOperationStatus()
{
	return m_Status;
}

CString CStrategyWnd::GetTime()
{
	if(m_pLogicRootItem)
	{
		return m_pLogicRootItem->GetTime();
	}
	return "";	
}


LRESULT CStrategyWnd::OnSTDataManagerMessage( WPARAM wParam, LPARAM lParam)
{
	if(m_strategyType==ST_ST)
	{
		return OnSTDataManagerMessageST( wParam, lParam);
	}
	else if(m_strategyType==ST_SD)
	{
		return OnSTDataManagerMessageSD( wParam, lParam);
	}
	return 0L;
}

LRESULT CStrategyWnd::OnSTDataManagerMessageSD( WPARAM wParam, LPARAM lParam)
{
	CLogicST *pLogicST= (CLogicST *)wParam;
	pLogicST->ReceiveFromSTDataManager(lParam);
	ST_DRAWINFO* pDrawInfo = (ST_DRAWINFO*)lParam;
	if(!m_bHasAPData)
	{
		m_ptrArraySDataItem.RemoveAll();
		m_ptrArrayPlot.RemoveAll();
		m_ptrArrayHistoryDataItem.RemoveAll();
		m_pDateItem = m_pLogicRootItem->GetDateItem();
		CPtrArray *pHistoryData = m_pLogicRootItem->GetPArrayHistoryData();
		int nSizeOfHistory = pHistoryData->GetSize();
		for(int nPosOfHistory = 0 ; nPosOfHistory<nSizeOfHistory;nPosOfHistory++)
		{
			m_ptrArrayHistoryDataItem.Add(pHistoryData->GetAt(nPosOfHistory));
		}
		CIDataItem *pCloseItem = (CIDataItem *)pHistoryData->GetAt(3);

				
		///////////////////
		// Calculatation Logic
		//////////////////
		CPtrArray* pPrtArrayPlot = pDrawInfo->pPtrArrayPlot;
		int nSizeOfPtrArrayPlot = pPrtArrayPlot->GetSize();
		ASSERT(nSizeOfPtrArrayPlot);
		CIDataItem *pPlotItem = (CIDataItem *)pPrtArrayPlot->GetAt(0);
		int nSize = pPlotItem->GetSize();
		m_dealDataItem.Clear();
		double dValue = 0;
		double dValueLast = 0;
		BOOL bSetFirst = TRUE;
		int nStartPos = pPlotItem->GetFirstDataIndex();
	
		CDealCal *pDealBuy = NULL;
		CDealCal *pDealSell = NULL;

		// buy
		/////////////////////////////////////////////////////////////////
		if(m_strTypeBuy==OPT_INDEX_CROSSABOVE_BASIS)
		{
			pDealBuy = new CDealCalIndexCrossAboveBasis(&m_dealDataItem, VALBUY, m_dBuyOption);
		}
		else if(m_strTypeBuy==OPT_INDEX_CROSSBELOW_BASIS)
		{		
			pDealBuy = new CDealCalIndexCrossBelowBasis(&m_dealDataItem, VALBUY, m_dBuyOption);
		}
		else if(m_strTypeBuy==OPT_INDEX_CROSSABOVE_ANOTHER)
		{			
			ASSERT(m_dBuyOption<pPrtArrayPlot->GetSize());
			pDealBuy = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALBUY
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dBuyOption));
		}
		else if(m_strTypeBuy==OPT_INDEX_CROSSBELOW_ANOTHER)
		{			
			ASSERT(m_dBuyOption<pPrtArrayPlot->GetSize());
			pDealBuy = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALBUY
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dBuyOption));
		}
		else if(m_strTypeBuy==OPT_PRICE_CROSSABOVE_INDEX)
		{		
			pDealBuy = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALBUY, pCloseItem);
		}
		else if(m_strTypeBuy==OPT_PRICE_CROSSBELOW_INDEX)
		{			
			pDealBuy = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALBUY, pCloseItem);
		}
		else if(m_strTypeBuy==OPT_INDEX_UPWARDREVERSAL)
		{	
			pDealBuy = new CDealCalIndexUpwardReversal(&m_dealDataItem, VALBUY);
		}
		else if(m_strTypeBuy==OPT_INDEX_DOWNWARDREVERSAL)
		{	
			pDealBuy = new CDealCalIndexDownwardReversal(&m_dealDataItem, VALBUY);
		}		
		// SELL
		/////////////////////////////////////////////////////////////////
		if(m_strTypeSell==OPT_INDEX_CROSSABOVE_BASIS)
		{
			pDealSell = new CDealCalIndexCrossAboveBasis(&m_dealDataItem, VALSELL, m_dSellOption);
		}
		else if(m_strTypeSell==OPT_INDEX_CROSSBELOW_BASIS)
		{			
			pDealSell = new CDealCalIndexCrossBelowBasis(&m_dealDataItem, VALSELL, m_dSellOption);
		}
		else if(m_strTypeSell==OPT_INDEX_CROSSABOVE_ANOTHER)
		{			
			ASSERT(m_dSellOption<pPrtArrayPlot->GetSize());
			pDealSell = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALSELL
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dSellOption));
		}
		else if(m_strTypeSell==OPT_INDEX_CROSSBELOW_ANOTHER)
		{			
			ASSERT(m_dSellOption<pPrtArrayPlot->GetSize());
			pDealSell = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALSELL
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dSellOption));
		}
		else if(m_strTypeSell==OPT_PRICE_CROSSABOVE_INDEX)
		{			
			pDealSell = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALSELL, pCloseItem);
		}
		else if(m_strTypeSell==OPT_PRICE_CROSSBELOW_INDEX)
		{	
			pDealSell = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALSELL, pCloseItem);
		}
		else if(m_strTypeSell==OPT_INDEX_UPWARDREVERSAL)
		{			
			pDealSell = new CDealCalIndexUpwardReversal(&m_dealDataItem, VALSELL);
		}
		else if(m_strTypeSell==OPT_INDEX_DOWNWARDREVERSAL)
		{	
			pDealSell = new CDealCalIndexDownwardReversal(&m_dealDataItem, VALSELL);
		}
		if(pDealBuy&&pDealSell)
		{
			for(int nPos=nStartPos;nPos<nSize;nPos++)
			{
				dValue = pPlotItem->GetAt(nPos);
				pDealBuy->SetDeal(nPos,dValue);
				pDealSell->SetDeal(nPos,dValue);
			}	
		}		
		if(pDealBuy)
		{
			delete pDealBuy;
			pDealBuy = NULL;
		}
		if(pDealSell)
		{
			delete pDealSell;
			pDealSell = NULL;
		}
		
		m_ptrArraySDataItem.Add(m_pDateItem);
		m_DrawInfo.pPtrArraySDataItem = &m_ptrArraySDataItem;
		m_DrawInfo.pPtrArrayHistoryDataItem = &m_ptrArrayHistoryDataItem;
		m_DrawInfo.pPtrArrayPlot = &m_ptrArrayPlot;
		m_DrawInfo.pDealDataItem = &m_dealDataItem;

		m_bHasAPData = TRUE;
	}
	m_Status = STRATEGY_RUN;
	GetParent()->SendMessage(RMSG_STDATAMANAGER_RESULTDATA,m_lKey,(LPARAM)&m_DrawInfo);
	return 1L;
}

void CStrategyWnd::LoadConfiguration()
{
	InitConfiguration();
	ReadConfiguration(m_strStrategyPath);
}

void CStrategyWnd::SetChartDivideInfo(int nDivideState, int nUnitPos)
{
	// ���� ���� : m_nDivideState
	// HORZ_NOTYPE,		// ����
	// HORZ_DAILY,		// �ϰ�
	// HORZ_WEEKLEY,	// �ְ�
	// HORZ_MONTHLY,	// ����
	// HORZ_TIME_ONE,	// �ð�-����
	// HORZ_TICK,		// ƽ
	// HORZ_TIME_TWO,	// �ð�-����
	// HORZ_TEXT,		// text
	// HORZ_LEFTRIGHT,	// �¿��
	// HORZ_PRICEZONE,	// ���ݴ�
	// HORZ_DATA,		// data(�ŷ��� ����)
	// HORZ_MARKETPROFILE, // MarketProfile
	// HORZ_TIME_SECOND	// ��

	// ���� ���� �ð����� : m_nUnitPos
	m_nDivideState = nDivideState;
	m_nUnitPos = nUnitPos;
}

void CStrategyWnd::InitBuyDealTrace(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;
	if(m_bBuyDealTraceInit)
	{
		// ��ȸ ������ ó��
		if(bRealState == FALSE)
		{
			if(m_buyDealTrace.IsEntryPosition())
				pPrice = pLowPrice;
			else if(m_buyDealTrace.IsExitPosition())
				pPrice = pHighPrice;

			// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
			if(m_buyDealTrace.IsEntryPosition() || m_buyDealTrace.IsExitPosition())
			{
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
					lDealValue |= m_buyDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
				}
			}

			m_buyDealTrace.Next(bRealState);
		}
		// ���� ������ ó��
		else
		{
			// �ֹ������� [���Ǹ�����] �� ���
			if(m_buyDealTrace.GetTimingOrder() == 0)
			{
				if(m_buyDealTrace.IsEntryPosition())
					pPrice = pLowPrice;
				else if(m_buyDealTrace.IsExitPosition())
					pPrice = pHighPrice;

				//-------------------------------------------------------------
				// ��ȣǥ��
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
					lDealValue |= m_buyDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));

					if(m_bBuyOrderStartSignal == FALSE)
					{
						// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
						if(m_nOrderStartSignal == 1)
							m_bBuyOrderStartSignal = TRUE;
						// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
						else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition())
							m_bBuyOrderStartSignal = TRUE;
					}
				}

				//-------------------------------------------------------------
				// �ֹ�ó��
				if(m_bBuyOrderStartSignal)
					m_dealDataItem.SetData(m_buyDealTrace.GetRawIndex(), 
											m_buyDealTrace.GetRawDealValue(),
											m_buyDealTrace.GetRawType(),
											m_buyDealTrace.GetRawDealName(),
											m_buyDealTrace.GetRawPrice(),
											m_buyDealTrace.GetRawContract(),
											m_buyDealTrace.GetRawEntryNameForExit());

				m_buyDealTrace.Next(bRealState);
			}
			// �ֹ������� [���ϼ���] �� ���
			else if(m_buyDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// ��ȣǥ��
				if(nInterval == 0)		// �� ���� �״��
				{
					m_buyDealTrace.Copy();
					if(m_buyDealTrace.GetOrdrIndex() < 0 && m_buyDealTrace.GetOrdrDealValue() == 0l)
						m_bBuyDealTraceInit = FALSE;
				}
				else					// �� ���� ����
				{
					m_buyDealTrace.Next(bRealState);

					if(m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue()))
						pPrice = pLowPrice;
					else if(m_buyDealTrace.IsExitPosition(m_buyDealTrace.GetOrdrDealValue()))
						pPrice = pHighPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetOrdrIndex());
						lDealValue |= m_buyDealTrace.GetOrdrDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetOrdrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetOrdrIndex(), m_buyDealTrace.GetOrdrDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetOrdrIndex(), pPrice->GetAt(m_buyDealTrace.GetOrdrIndex()));

						if(m_bBuyOrderStartSignal == FALSE)
						{
							// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
							if(m_nOrderStartSignal == 1)
								m_bBuyOrderStartSignal = TRUE;
							// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
							else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue()))
								m_bBuyOrderStartSignal = TRUE;
						}
					}
					else
					{
						if(m_buyDealTrace.GetOrdrIndex() < 0 && m_buyDealTrace.GetOrdrDealValue() == 0l)
							m_bBuyDealTraceInit = FALSE;
					}

					//-------------------------------------------------------------
					// �ֹ�ó��
					if(m_bBuyOrderStartSignal)
						m_dealDataItem.SetData(m_buyDealTrace.GetOrdrIndex(), 
												m_buyDealTrace.GetOrdrDealValue(),
												m_buyDealTrace.GetOrdrType(),
												m_buyDealTrace.GetOrdrDealName(),
												m_buyDealTrace.GetOrdrPrice(),
												m_buyDealTrace.GetOrdrContract(),
												m_buyDealTrace.GetOrdrEntryNameForExit());
				}
			}
		}
	}
}

void CStrategyWnd::InitSellDealTrace(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;
	if(m_bSellDealTraceInit)
	{
		// ��ȸ ������ ó��
		if(bRealState == FALSE)
		{
			if(m_sellDealTrace.IsEntryPosition())
				pPrice = pHighPrice;
			else if(m_sellDealTrace.IsExitPosition())
				pPrice = pLowPrice;

			if(m_sellDealTrace.IsEntryPosition() || m_sellDealTrace.IsExitPosition())
			{
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
					lDealValue |= m_sellDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
				}
			}

			m_sellDealTrace.Next(bRealState);
		}
		// ���� ������ ó��
		else
		{
			// �ֹ������� [���Ǹ�����] �� ���
			if(m_sellDealTrace.GetTimingOrder() == 0)
			{
				if(m_sellDealTrace.IsEntryPosition())
					pPrice = pHighPrice;
				else if(m_sellDealTrace.IsExitPosition())
					pPrice = pLowPrice;

				//-------------------------------------------------------------
				// ��ȣǥ��
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
					lDealValue |= m_sellDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));

					if(m_bSellOrderStartSignal == FALSE)
					{
						// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
						if(m_nOrderStartSignal == 1)
							m_bSellOrderStartSignal = TRUE;
						// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
						else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition())
							m_bSellOrderStartSignal = TRUE;
					}
				}

				//-------------------------------------------------------------
				// �ֹ�ó��
				if(m_bSellOrderStartSignal)
					m_dealDataItem.SetData(m_sellDealTrace.GetRawIndex(), 
											m_sellDealTrace.GetRawDealValue(),
											m_sellDealTrace.GetRawType(),
											m_sellDealTrace.GetRawDealName(),
											m_sellDealTrace.GetRawPrice(),
											m_sellDealTrace.GetRawContract(),
											m_sellDealTrace.GetRawEntryNameForExit());

				m_sellDealTrace.Next(bRealState);
			}
			// �ֹ������� [���ϼ���] �� ���
			else if(m_sellDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// ��ȣǥ��
				if(nInterval == 0)		// �� ���� �״��
				{
					m_sellDealTrace.Copy();
					if(m_sellDealTrace.GetOrdrIndex() < 0 && m_sellDealTrace.GetOrdrDealValue() == 0l)
						m_bSellDealTraceInit = FALSE;
				}
				else					// �� ���� ����
				{
					m_sellDealTrace.Next(bRealState);

					if(m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue()))
						pPrice = pHighPrice;
					else if(m_sellDealTrace.IsExitPosition(m_sellDealTrace.GetOrdrDealValue()))
						pPrice = pLowPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetOrdrIndex());
						lDealValue |= m_sellDealTrace.GetOrdrDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetOrdrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetOrdrIndex(), m_sellDealTrace.GetOrdrDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetOrdrIndex(), pPrice->GetAt(m_sellDealTrace.GetOrdrIndex()));

						if(m_bSellOrderStartSignal == FALSE)
						{
							// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
							if(m_nOrderStartSignal == 1)
								m_bSellOrderStartSignal = TRUE;
							// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
							else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue()))
								m_bSellOrderStartSignal = TRUE;
						}
					}

					//-------------------------------------------------------------
					// �ֹ�ó��
					if(m_bSellOrderStartSignal)
						m_dealDataItem.SetData(m_sellDealTrace.GetOrdrIndex(), 
												m_sellDealTrace.GetOrdrDealValue(),
												m_sellDealTrace.GetOrdrType(),
												m_sellDealTrace.GetOrdrDealName(),
												m_sellDealTrace.GetOrdrPrice(),
												m_sellDealTrace.GetOrdrContract(),
												m_sellDealTrace.GetOrdrEntryNameForExit());
				}
			}
		}
	}
}

void CStrategyWnd::ExecuteBuyDealTrace(CDealData* pDealData, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;

	// ��ȸ ������ ó��
	if(bRealState == FALSE)
	{
		if(m_buyDealTrace.Set(pDealData, bRealState))
		{
			ConfirmBuyForcedLiquidation(bRealState, nInterval);

			if(m_buyDealTrace.IsEntryPosition())
				pPrice = pLowPrice;
			else if(m_buyDealTrace.IsExitPosition())
				pPrice = pHighPrice;

			if(pPrice)
			{
				long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
				lDealValue |= m_buyDealTrace.GetRawDealValue();
				m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
				m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
				m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
			}
			
//			pDealData->m_nIndex = m_buyDealTrace.GetRawIndex();
//			pDealData->m_lDealValue = m_buyDealTrace.GetRawDealValue();
//			pDealData->m_lType = m_buyDealTrace.GetRawType();
//			pDealData->m_strDealName = m_buyDealTrace.GetRawDealName();
//			pDealData->m_dPrice = m_buyDealTrace.GetRawPrice();
//			pDealData->m_dContract = m_buyDealTrace.GetRawContract();
//			pDealData->m_strListEntryNameForExit.AddTail(m_buyDealTrace.GetRawEntryNameForExit());

			m_buyDealTrace.Next(bRealState);
		}
	}
	// ���� ������ ó��
	else
	{
		if(m_buyDealTrace.Set(pDealData, bRealState))
		{
			// �ֹ������� [���Ǹ�����] �� ���
			if(m_buyDealTrace.GetTimingOrder() == 0)
			{
				//-------------------------------------------------------------
				// ����û�� Ȯ��
				ConfirmBuyForcedLiquidation(bRealState, nInterval);

				if(m_buyDealTrace.IsEntryPosition())
					pPrice = pLowPrice;
				else if(m_buyDealTrace.IsExitPosition())
					pPrice = pHighPrice;
			
				//-------------------------------------------------------------
				// ��ȣǥ��
				if(nInterval == 0)		// �� ���� �״��
				{
					long lDealValue = 0l;
					if(pPrice)
					{
						// ��ȣ ǥ�� ����
						lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
						long lDealValueBuy = (lDealValue & (VALENTRYBUY | VALEXITLONG | 
										FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
										FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
										FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION));

						if((m_buyDealTrace.IsEntryPosition(lDealValueBuy) && m_buyDealTrace.GetRawDealValue() == 0) ||
							(m_buyDealTrace.IsExitPosition(lDealValueBuy) && m_buyDealTrace.GetRawDealValue() == 0))
						{
							// ��ȣ ǥ�� �ʱ�ȭ
							lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
							lDealValue &= ~(VALENTRYBUY | VALEXITLONG | 
											FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
											FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
											FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION);
// 2008.04.04 by LYH >>
							if(pDealData->m_lDealValue >0 && m_buyDealTrace.GetRawDealValue()==0 && m_sellDealTrace.GetCurrIndex() == m_buyDealTrace.GetRawIndex())
							{
								lDealValue |= m_buyDealTrace.GetCurrDealValue();
							}
// 2008.04.04 by LYH <<
						}
						else
						{
							lDealValue |= m_buyDealTrace.GetRawDealValue();
						}

						m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
					}
					else
					{
						// ��ȣ ǥ�� �ʱ�ȭ
						lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
						lDealValue &= ~(VALENTRYBUY | VALEXITLONG | 
										FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
										FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
										FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION);

// 2008.04.04 by LYH >>
						lDealValue |= m_buyDealTrace.GetRawDealValue();

						//if(pDealData->m_lDealValue >0 && m_buyDealTrace.GetRawDealValue()==0 && m_buyDealTrace.GetCurrIndex() == m_buyDealTrace.GetRawIndex())
						if((pDealData->m_lDealValue >0 || lDealValue&FORCEDEXIT) && m_buyDealTrace.GetRawDealValue()==0 && m_buyDealTrace.GetCurrIndex() == m_buyDealTrace.GetRawIndex())
						{
							lDealValue |= m_buyDealTrace.GetCurrDealValue();
						}
// 2008.04.04 by LYH >>
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), "");
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), 0.);
					}
				}
				else					// �� ���� ����
				{
// 2008.04.04 by LYH >>
					long lDealValueCurr = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetCurrIndex());
					int nIndex = m_buyDealTrace.GetCurrIndex();
					if(m_buyDealTrace.GetCurrDealValue() > 0 && !(IsBuy(lDealValueCurr) || IsExitLong(lDealValueCurr)))
					{
							m_arrayDealPosition.SetAt(m_buyDealTrace.GetCurrIndex(), (double)(lDealValueCurr|m_buyDealTrace.GetCurrDealValue()));
					}
// 2008.04.04 by LYH <<

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());

						lDealValue |= m_buyDealTrace.GetRawDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
					}
				}

				if(pPrice && m_bBuyOrderStartSignal == FALSE)
				{
					// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
					if(m_nOrderStartSignal == 1)
						m_bBuyOrderStartSignal = TRUE;
					// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
					else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition())
						m_bBuyOrderStartSignal = TRUE;
				}

				//-------------------------------------------------------------
				// �ֹ�ó��
				if(m_bBuyOrderStartSignal)
					m_dealDataItem.SetData(m_buyDealTrace.GetRawIndex(), 
											m_buyDealTrace.GetRawDealValue(),
											m_buyDealTrace.GetRawType(),
											m_buyDealTrace.GetRawDealName(),
											m_buyDealTrace.GetRawPrice(),
											m_buyDealTrace.GetRawContract(),
											m_buyDealTrace.GetRawEntryNameForExit());

				m_buyDealTrace.Next(bRealState);
			}
			// �ֹ������� [���ϼ���] �� ���
			else if(m_buyDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// ��ȣǥ��
				if(nInterval == 0)		// �� ���� �״��
				{
					m_buyDealTrace.Copy();

					//-------------------------------------------------------------
					// ����û�� Ȯ��
					ConfirmBuyForcedLiquidation(bRealState, nInterval);

					if(m_buyDealTrace.IsForcedLiquidation(m_buyDealTrace.GetCurrDealValue()))
						pPrice = pHighPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetCurrIndex());
						lDealValue |= m_buyDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetCurrIndex(), m_buyDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetCurrIndex(), pPrice->GetAt(m_buyDealTrace.GetCurrIndex()));

						if(m_bBuyOrderStartSignal == FALSE)
						{
							// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
							if(m_nOrderStartSignal == 1)
								m_bBuyOrderStartSignal = TRUE;
							// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
							else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue()))
								m_bBuyOrderStartSignal = TRUE;
						}

						//-------------------------------------------------------------
						// �ֹ�ó��
						if(m_bBuyOrderStartSignal)
							m_dealDataItem.SetData(m_buyDealTrace.GetCurrIndex(), 
													m_buyDealTrace.GetCurrDealValue(),
													m_buyDealTrace.GetCurrType(),
													m_buyDealTrace.GetCurrDealName(),
													m_buyDealTrace.GetCurrPrice(),
													m_buyDealTrace.GetCurrContract(),
													m_buyDealTrace.GetCurrEntryNameForExit());

						m_buyDealTrace.Next(bRealState);
					}
				}
				else					// �� ���� ����
				{
					//-------------------------------------------------------------
					// ����û�� Ȯ��
					ConfirmBuyForcedLiquidation(bRealState, nInterval);

					if(m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue()))
						pPrice = pLowPrice;
					else if(m_buyDealTrace.IsExitPosition(m_buyDealTrace.GetCurrDealValue()))
					{
						if(m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue()))
							pPrice = pHighPrice;
						else
							m_buyDealTrace.m_DealDataCurr.m_lDealValue = 0l;
					}

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetCurrIndex());
						lDealValue |= m_buyDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetCurrIndex(), m_buyDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetCurrIndex(), pPrice->GetAt(m_buyDealTrace.GetCurrIndex()));

						if(m_bBuyOrderStartSignal == FALSE)
						{
							// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
							if(m_nOrderStartSignal == 1)
								m_bBuyOrderStartSignal = TRUE;
							// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
							else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue()))
								m_bBuyOrderStartSignal = TRUE;
						}
					}

					//-------------------------------------------------------------
					// �ֹ�ó��
					if(m_bBuyOrderStartSignal)
						m_dealDataItem.SetData(m_buyDealTrace.GetCurrIndex(), 
												m_buyDealTrace.GetCurrDealValue(),
												m_buyDealTrace.GetCurrType(),
												m_buyDealTrace.GetCurrDealName(),
												m_buyDealTrace.GetCurrPrice(),
												m_buyDealTrace.GetCurrContract(),
												m_buyDealTrace.GetCurrEntryNameForExit());

					m_buyDealTrace.Next(bRealState);
				}
			}
		}
	}
}

void CStrategyWnd::ExecuteSellDealTrace(CDealData* pDealData, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;

	// ��ȸ ������ ó��
	if(bRealState == FALSE)
	{
		if(m_sellDealTrace.Set(pDealData, bRealState))
		{
			ConfirmSellForcedLiquidation(bRealState, nInterval);

			if(m_sellDealTrace.IsEntryPosition())
				pPrice = pHighPrice;
			else if(m_sellDealTrace.IsExitPosition())
				pPrice = pLowPrice;

			if(pPrice)
			{
				long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
				lDealValue |= m_sellDealTrace.GetRawDealValue();
				m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
				m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
				m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
			}

//			pDealData->m_nIndex = m_sellDealTrace.GetRawIndex();
//			pDealData->m_lDealValue = m_sellDealTrace.GetRawDealValue();
//			pDealData->m_lType = m_sellDealTrace.GetRawType();
//			pDealData->m_strDealName = m_sellDealTrace.GetRawDealName();
//			pDealData->m_dPrice = m_sellDealTrace.GetRawPrice();
//			pDealData->m_dContract = m_sellDealTrace.GetRawContract();
//			pDealData->m_strListEntryNameForExit.AddTail(m_sellDealTrace.GetRawEntryNameForExit());

			m_sellDealTrace.Next(bRealState);
		}
	}
	// ���� ������ ó��
	else
	{
		if(m_sellDealTrace.Set(pDealData, bRealState))
		{
			// �ֹ������� [���Ǹ�����] �� ���
			if(m_buyDealTrace.GetTimingOrder() == 0)
			{
				//-------------------------------------------------------------
				// ����û�� Ȯ��
				ConfirmSellForcedLiquidation(bRealState, nInterval);

				if(m_sellDealTrace.IsEntryPosition())
					pPrice = pHighPrice;
				else if(m_sellDealTrace.IsExitPosition())
					pPrice = pLowPrice;

				//-------------------------------------------------------------
				// ��ȣǥ��
				if(nInterval == 0)		// �� ���� �״��
				{
					long lDealValue = 0l;
					if(pPrice)
					{
						// ��ȣ ǥ�� ����
//						lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
//						lDealValue |= m_sellDealTrace.GetRawDealValue();
						lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
						long lDealValueSell = (lDealValue & (VALENTRYSELL | VALEXITSHORT | 
										FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
										FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
										FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION));

						if((m_sellDealTrace.IsEntryPosition(lDealValueSell) && m_sellDealTrace.GetRawDealValue() == 0) ||
							(m_sellDealTrace.IsExitPosition(lDealValueSell) && m_sellDealTrace.GetRawDealValue() == 0))
						{
							// ��ȣ ǥ�� �ʱ�ȭ
							lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
							lDealValue &= ~(VALENTRYSELL | VALEXITSHORT | 
											FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
											FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
											FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION);
						}
						else
						{
							lDealValue |= m_sellDealTrace.GetRawDealValue();
						}
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
					}
					else
					{
						// ��ȣ ǥ�� �ʱ�ȭ
						lDealValue = m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
						lDealValue &= ~(VALENTRYSELL | VALEXITSHORT | 
										FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
										FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
										FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION);

// 2008.04.04 by LYH >>
						//if(pDealData->m_lDealValue >0 && m_sellDealTrace.GetRawDealValue()==0 && m_sellDealTrace.GetCurrIndex() == m_sellDealTrace.GetRawIndex())
						if((pDealData->m_lDealValue >0 || lDealValue&FORCEDEXIT) && m_sellDealTrace.GetRawDealValue()==0 && m_sellDealTrace.GetCurrIndex() == m_sellDealTrace.GetRawIndex())
						{
							lDealValue |= m_sellDealTrace.GetCurrDealValue();
						}
// 2008.04.04 by LYH <<
						
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), "");
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), 0.);
					}
				}
				else					// �� ���� ����
				{
// 2008.04.04 by LYH >>
					long lDealValueCurr = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetCurrIndex());
					int nIndex = m_sellDealTrace.GetCurrIndex();
					if(m_sellDealTrace.GetCurrDealValue() > 0 && !(IsSell(lDealValueCurr) || IsExitShort(lDealValueCurr)))
					{
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetCurrIndex(), (double)(lDealValueCurr|m_sellDealTrace.GetCurrDealValue()));
					}
// 2008.04.04 by LYH <<
					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
						lDealValue |= m_sellDealTrace.GetRawDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
					}
				}

				if(pPrice && m_bSellOrderStartSignal == FALSE)
				{
					// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
					if(m_nOrderStartSignal == 1)
						m_bSellOrderStartSignal = TRUE;
					// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
					else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition())
						m_bSellOrderStartSignal = TRUE;
				}

				//-------------------------------------------------------------
				// �ֹ�ó��
				if(m_bSellOrderStartSignal)
				{
					int nDealTraceLast = m_sellDealTrace.GetRawIndex();
					int nDealDataLast = m_dealDataItem.GetLastIndex();
					if(nDealTraceLast == nDealDataLast)
					{
						CDealData* pDealDataLast = m_dealDataItem.GetDealDataByIndex(nDealDataLast);
						if(pDealDataLast)
						{
							long lDealValue = pDealDataLast->m_lDealValue;
							lDealValue &= ~(VALENTRYSELL | VALEXITSHORT | 
											FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
											FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
											FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION);

							lDealValue |= m_sellDealTrace.GetRawDealValue();
							if(lDealValue)
								m_dealDataItem.SetData(m_sellDealTrace.GetRawIndex(), 
														lDealValue,
														m_sellDealTrace.GetRawType(),
														m_sellDealTrace.GetRawDealName(),
														m_sellDealTrace.GetRawPrice(),
														m_sellDealTrace.GetRawContract(),
														m_sellDealTrace.GetRawEntryNameForExit());
						}
					}
					else
					{
						m_dealDataItem.SetData(m_sellDealTrace.GetRawIndex(), 
											m_sellDealTrace.GetRawDealValue(),
											m_sellDealTrace.GetRawType(),
											m_sellDealTrace.GetRawDealName(),
											m_sellDealTrace.GetRawPrice(),
											m_sellDealTrace.GetRawContract(),
											m_sellDealTrace.GetRawEntryNameForExit());
					}
				}

				m_sellDealTrace.Next(bRealState);
			}
			// �ֹ������� [���ϼ���] �� ���
			else if(m_buyDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// ��ȣǥ��
				if(nInterval == 0)		// �� ���� �״��
				{
					m_sellDealTrace.Copy();

					//-------------------------------------------------------------
					// ����û�� Ȯ��
					ConfirmSellForcedLiquidation(bRealState, nInterval);

					if(m_sellDealTrace.IsForcedLiquidation(m_sellDealTrace.GetCurrDealValue()))
						pPrice = pHighPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetCurrIndex());
						lDealValue |= m_sellDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetCurrIndex(), m_sellDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetCurrIndex(), pPrice->GetAt(m_sellDealTrace.GetCurrIndex()));

						if(m_bSellOrderStartSignal == FALSE)
						{
							// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
							if(m_nOrderStartSignal == 1)
								m_bSellOrderStartSignal = TRUE;
							// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
							else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue()))
								m_bSellOrderStartSignal = TRUE;
						}

						//-------------------------------------------------------------
						// �ֹ�ó��
						if(m_bSellOrderStartSignal)
							m_dealDataItem.SetData(m_sellDealTrace.GetCurrIndex(), 
													m_sellDealTrace.GetCurrDealValue(),
													m_sellDealTrace.GetCurrType(),
													m_sellDealTrace.GetCurrDealName(),
													m_sellDealTrace.GetCurrPrice(),
													m_sellDealTrace.GetCurrContract(),
													m_sellDealTrace.GetCurrEntryNameForExit());

						m_sellDealTrace.Next(bRealState);
					}
				}
				else					// �� ���� ����
				{
					//-------------------------------------------------------------
					// ����û�� Ȯ��
					ConfirmSellForcedLiquidation(bRealState, nInterval);

					if(m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue()))
						pPrice = pHighPrice;
					else if(m_sellDealTrace.IsExitPosition(m_sellDealTrace.GetCurrDealValue()))
					{
						if(m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue()))
							pPrice = pLowPrice;
						else
							m_sellDealTrace.m_DealDataCurr.m_lDealValue = 0l;
					}

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetCurrIndex());
						lDealValue |= m_sellDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetCurrIndex(), m_sellDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetCurrIndex(), pPrice->GetAt(m_sellDealTrace.GetCurrIndex()));

						if(m_bSellOrderStartSignal == FALSE)
						{
							// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
							if(m_nOrderStartSignal == 1)
								m_bSellOrderStartSignal = TRUE;
							// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
							else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue()))
								m_bSellOrderStartSignal = TRUE;
						}
					}

					//-------------------------------------------------------------
					// �ֹ�ó��
					if(m_bSellOrderStartSignal)
						m_dealDataItem.SetData(m_sellDealTrace.GetCurrIndex(), 
												m_sellDealTrace.GetCurrDealValue(),
												m_sellDealTrace.GetCurrType(),
												m_sellDealTrace.GetCurrDealName(),
												m_sellDealTrace.GetCurrPrice(),
												m_sellDealTrace.GetCurrContract(),
												m_sellDealTrace.GetCurrEntryNameForExit());

					m_sellDealTrace.Next(bRealState);
				}
			}
		}
	}
}

void CStrategyWnd::ExecuteDealTrace()
{
	CDealData* pDealData = NULL;
	POSITION pos = m_dealDataItem.GetHeadPosition();
	while(pos)
	{
		pDealData = m_dealDataItem.GetNext(pos);
		
		if(!m_bBuyDealTraceInit)
		{
			m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
			InitBuyDealTrace();
		}
		else if(m_bBuyDealTraceInit)
		{
			ExecuteBuyDealTrace(pDealData);
		}

		if(!m_bSellDealTraceInit)
		{
			m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
			InitSellDealTrace();
		}
		else if(m_bSellDealTraceInit)
		{
			ExecuteSellDealTrace(pDealData);
		}
	}
}

void CStrategyWnd::ExecuteDealTraceReal(CDealData* pDealData, int& nTotalCount, int& nInterval)
{
	// �������� ���
	if(ExecuteDealTraceExceptional(pDealData, nTotalCount, nInterval))
		return;

	// ������� ���
	ExecuteDealTraceUsual(pDealData, nTotalCount, nInterval);
}

BOOL CStrategyWnd::ExecuteDealTraceExceptional(CDealData* pDealData, int& nTotalCount, int& nInterval)
{
	if(pDealData == NULL || nTotalCount <= 0)
		return FALSE;

	int nCurrIndex = nTotalCount - 1;
	pDealData->m_nIndex = nCurrIndex;

	ResizeSignalInfoArray(nInterval, nTotalCount);

	// ��ȸ�����Ͱ� ���� ���
	if(m_dealDataItem.GetCount() == 0)
	{
		if(!m_bBuyDealTraceInit && !m_bSellDealTraceInit)
		{
			m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
			m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
			InitBuyDealTrace(TRUE);
			InitSellDealTrace(TRUE);
		}

		m_dealDataItem.SetData(nCurrIndex, pDealData->m_lDealValue, pDealData->m_lType, pDealData->m_strDealName, 
								pDealData->m_dPrice, pDealData->m_dContract, &pDealData->m_strListEntryNameForExit);

		m_nLastSize = nTotalCount;
		return TRUE;
	}

	// 1ƽ�� ���
	if(m_nDivideState == 5 && m_nUnitPos == 1)
	{
		if(!m_bBuyDealTraceInit)
		{
			m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
			InitBuyDealTrace(TRUE);
		}
		else
		{
			ExecuteBuyDealTrace(pDealData, TRUE, nInterval);
		}

		if(!m_bSellDealTraceInit)
		{
			m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
			InitSellDealTrace(TRUE);
		}
		else
		{
			ExecuteSellDealTrace(pDealData, TRUE, nInterval);
		}

		m_nLastSize = nTotalCount;
		return TRUE;
	}

	return FALSE;
}

BOOL CStrategyWnd::ExecuteDealTraceUsual(CDealData* pDealData, int& nTotalCount, int& nInterval)
{
	int nCurrIndex = nTotalCount - 1;
	pDealData->m_nIndex = nCurrIndex;

	if(!m_bBuyDealTraceInit && !m_bSellDealTraceInit)
	{
		m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
		m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
		InitBuyDealTrace(TRUE, nInterval);
		InitSellDealTrace(TRUE, nInterval);
	}

	ResizeSignalInfoArray(nInterval, nTotalCount);

	if(!m_bBuyDealTraceInit)
	{
		m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
		InitBuyDealTrace(TRUE);
	}
	else
	{
		ExecuteBuyDealTrace(pDealData, TRUE, nInterval);
	}

	if(!m_bSellDealTraceInit)
	{
		m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
		InitSellDealTrace(TRUE);
	}
	else
	{
		ExecuteSellDealTrace(pDealData, TRUE, nInterval);
	}

	m_nLastSize = nTotalCount;
	return TRUE;
}

void CStrategyWnd::ConfirmBuyForcedLiquidation(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	BOOL bConfirmState = FALSE;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))				// ��ȸ or ������ ��� ���Ǹ�����
		bConfirmState = m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue());
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)									// ������ ��� ���ϼ���
		bConfirmState = m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue());

	if(bConfirmState)
	{
		CForcedLiquidationInfo FLInfo;
		//-------- ����û�� �ɼǺ� --------
		// ������(�սǷ�) �ɼ� ��
		if(IsLossRate())
			BuyForcedLiquidation_LossRate(FLInfo, bRealState);
		
		// ��ǥ���� �ɼ� ��
		if(IsTargetBenefit())
			BuyForcedLiquidation_TargetBenefit(FLInfo, bRealState);
		
		// �ְ���� �϶� �ɼ� ��
		if(IsHPriceCPBear())
			BuyForcedLiquidation_HPriceCPBear(FLInfo, bRealState);
		
		// ��ǥ������ �ɼ� ��
		if(IsTargetValuePreserve())
			BuyForcedLiquidation_TargetValuePreserve(FLInfo, bRealState);
		
		// ���ͺ��� �ɼ� ��
		if(IsBenefitPreserve())
			BuyForcedLiquidation_BenefitPreserve(FLInfo, bRealState);
		
		// �ּҰ��ݺ�ȭ �ɼ� ��
		if(IsMiniPrice())
			BuyForcedLiquidation_MiniPrice(FLInfo, bRealState);
		
		// ����û�� �ɼ� ��
		if(IsDayLiquidation())
			BuyForcedLiquidation_DayLiquidation(FLInfo, bRealState);
		
		// ����û�� ����
		BuyForcedLiquidation(FLInfo, bRealState, nInterval);
	}
}

void CStrategyWnd::ConfirmSellForcedLiquidation(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	BOOL bConfirmState = FALSE;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))				// ��ȸ or ������ ��� ���Ǹ�����
		bConfirmState = m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue());
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)									// ������ ��� ���ϼ���
		bConfirmState = m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue());

	if(bConfirmState)
	{
		CForcedLiquidationInfo FLInfo;
		//-------- ����û�� �ɼǺ� --------
		// ������(�սǷ�) �ɼ� ��
		if(IsLossRate())
			SellForcedLiquidation_LossRate(FLInfo, bRealState);
		
		// ��ǥ���� �ɼ� ��
		if(IsTargetBenefit())
			SellForcedLiquidation_TargetBenefit(FLInfo, bRealState);
		
		// �ְ���� �϶� �ɼ� ��
		if(IsHPriceCPBear())
			SellForcedLiquidation_HPriceCPBear(FLInfo, bRealState);
		
		// ��ǥ������ �ɼ� ��
		if(IsTargetValuePreserve())
			SellForcedLiquidation_TargetValuePreserve(FLInfo, bRealState);
		
		// ���ͺ��� �ɼ� ��
		if(IsBenefitPreserve())
			SellForcedLiquidation_BenefitPreserve(FLInfo, bRealState);
		
		// �ּҰ��ݺ�ȭ �ɼ� ��
		if(IsMiniPrice())
			SellForcedLiquidation_MiniPrice(FLInfo, bRealState);
		
		// ����û�� �ɼ� ��
		if(IsDayLiquidation())
			SellForcedLiquidation_DayLiquidation(FLInfo, bRealState);
		
		// ����û�� ����
		SellForcedLiquidation(FLInfo, bRealState, nInterval);
	}
}

void CStrategyWnd::BuyForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)				// �����ŷ� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);	// ��ȸ or ������ ��� ���Ǹ�����
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);	// ������ ��� ���ϼ���
	
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBearValue = 0.;		// �϶��� ��
	// %
	if(m_uLossRate_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dLossRate); 	// ��������(%) �϶��� ���
	// ��
	else
		dBearValue = dClose - m_dLossRate;				// �����ݾ�(��) �϶��� ���

	// EntryPosition �� ExitPosition ���̿��� ������ �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}
	
	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� �϶��� ���
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_LOSSRATE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// ��ǥ�������� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBullValue = 0.;		// ����� ��
	// %
	if(m_uTargetBenefit_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetBenefit);	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dClose + m_dTargetBenefit;				// �����ݾ�(��) ����� ���

	// EntryPosition �� ExitPosition ���̿��� ��ǥ���� �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		//if(dValueForCompare >= dBullValue)
		//�Ҽ��� ����
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_TARGETBENEFIT, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// ������϶����� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dHighValue = 0.;
	double dCloseValue = 0.;
	double dHighEach = 0.;
	double dCloseEach = 0.;

	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dHighEach = pHigh->GetAt(nIndx);

		// ���� ���� �� ���� ��� �ְ��� ����
		if(dHighValue - dHighEach < 0.)
			dHighValue = dHighEach;


		dCloseEach = pClose->GetAt(nIndx);
		double dBearValue = 0.;		// �϶��� ��
		// %
		if(m_uHPriceCPBear_PerOrWon == 0)
			dBearValue = dHighValue - (dHighValue * m_dHPriceCPBear);		// ��������(%) �϶��� ���
		// ��
		else
			dBearValue = dHighValue - m_dHPriceCPBear;	 					// �����ݾ�(��) �϶��� ���

		// �������� �Ǵ� �����ݾ����� ������ ���� ���ذ����� �϶��� ���
		//if(dCloseEach <= dBearValue)
		int nVal1 = dCloseEach*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_HPRICECPBEAR, pHigh);
			return;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)	// ��ǥ�������� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	
	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBullValue = 0.;		// ����� ��
	// %
	if(m_uTargetValuePreserve_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetValuePreserve);	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dClose + m_dTargetValuePreserve;				// �����ݾ�(��) ����� ���

	// EntryPosition �� ExitPosition ���̿��� ��ǥ������ �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare = 0.;
	double dValueForTargetValuePreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ��ǥ������������ ����� ���
		//if(dValueForCompare >= dBullValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			dValueForTargetValuePreserve = dBullValue;
			break;
		}
	}
	
	// ��ǥ������ �ְ� ������ Position ���� ExitPosition ���̿��� �ְ� �϶� ���Ŀ� ���� ����û�� Position ��ȸ
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ��ǥ������������ �϶��� ���
		//if(dValueForCompare < dValueForTargetValuePreserve)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dValueForTargetValuePreserve*100;
		if(nVal1 < nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_TARGETVALUEPRESERVE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// ���ͺ����� ���� ����û�� 
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBullValue = 0.;		// ����� ��
	// %
	if(m_uBenefitPreserveUp_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dBenefitPreserveUp);	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dClose + m_dBenefitPreserveUp;				// �����ݾ�(��) ����� ���

	// EntryPosition �� ExitPosition ���̿��� ���ͺ��� �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare = 0.;
	double dValueForBenefitPreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		if(dValueForCompare >= dBullValue)
		{
			dValueForBenefitPreserve = dBullValue;
			break;
		}
	}

	double dBearValue = 0.;		// �϶��� ��
	// %
	if(m_uBenefitPreserveDn_PerOrWon == 0)
		dBearValue = dValueForBenefitPreserve - (dValueForBenefitPreserve * m_dBenefitPreserveDn);	// ��������(%) ����� ���
	// ��
	else
		dBearValue = dValueForBenefitPreserve - m_dBenefitPreserveDn;										// �����ݾ�(��) ����� ���

	// ���ͺ��� �ְ� ������ Position ���� ExitPosition ���̿��� �϶��� �����Ͽ� ����û���� Position ��ȸ
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� �϶��� ���
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_BENEFITPRESERVE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// �ּҰ������� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	if(m_nBongCount > 0)
	{
		// EntryPosition �� ExitPosition ���̿��� �ּҰ��ݺ�ȭ �ɼ����� ����û���� Position ��ȸ
		double dValueForCompare;
		double dRatio = m_dMiniPrice;
		int nFLIndx = 0;
		int nFLCount = 0;
		if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		{
			nFLIndx = m_buyDealTrace.GetCurrIndex() + m_nBongCount;	// ��ȸ or ������ ��� ���Ǹ�����
			nFLCount = m_buyDealTrace.GetRawIndex();
		}
		else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		{
			nFLIndx = m_buyDealTrace.GetOrdrIndex() + m_nBongCount;	// ������ ��� ���ϼ���
			nFLCount = m_buyDealTrace.GetCurrIndex();
		}

		for(int nIndx = nFLIndx; nIndx <= nFLCount; )
		{
			dValueForCompare = pClose->GetAt(nIndx);

			double dBullValue = 0.;		// ����� ��
			// %
			if(m_uMiniPrice_PerOrWon == 0)
				dBullValue = dClose + (dClose * dRatio); 	// �����Ǵ� ��������(%) ����� ���
			// ��
			else
				dBullValue = dClose + dRatio;				// �����Ǵ� �����ݾ�(��) ����� ���

			// ������ �������� �Ǵ� �����ݾ��̳��� Ⱦ���� ������ ���
			// �� ���ذ��� �������� ���� ���
			//if(dValueForCompare <= dBullValue)
			int nVal1 = dValueForCompare*100;
			int nVal2 = dBullValue*100;
			if(nVal1 <= nVal2)
			{
				FLInfo.SetFLInfo(nIndx, FL_BUY_MINIPRICE, pHigh);
				break;
			}
			nIndx += m_nBongCount;
			dRatio += m_dMiniPrice;
		}
	}	
}

void CStrategyWnd::BuyForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// ����û������ ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);

	CString szDate;
	CString szHour;
	int		nMin;
	CString szCheckHour;
	int		nCheckMin;

	// �� ����
	int	nInterval = m_nUnitPos;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;			// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;			// ������ ��� ���ϼ���
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}
	
	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		// ���Ͻú�(�� : 10281310)
		szDate.Format("%08.8s", m_pDateItem->GetAt(nIndx));
		szHour = szDate.Mid(4, 2);
		nMin = atoi(szDate.Mid(6, 2));

		// ������ �ú�(�� : 1310)
		szCheckHour = m_szTimeDayLiquidation.Mid(0, 2);
		nCheckMin = atoi(m_szTimeDayLiquidation.Mid(2));
		if(szCheckHour.Compare(szHour) == 0 &&		// Hour Check
			nCheckMin <= nMin && abs(nCheckMin - nMin) <= nInterval)		// Min Check
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_DAYLIQUIDATION, pHigh);
			break;
		}
	}	
}

void CStrategyWnd::BuyForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	if(FLInfo.m_nExitIndex == SENTINEL_FLINFO || 
		FLInfo.m_lDealValue == 0L ||
		FLInfo.m_pDataItem == NULL)
		return;

	if(bRealState && m_buyDealTrace.GetTimingOrder() == 0)			// ��ȸ or ������ ��� ���Ǹ�����
	{
		// �� �̿ϼ� ���� & ����û������� [���ϼ���] �� ���
		if(nInterval == 0 && m_buyDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)		// ������ ��� ���ϼ���
	{
		// �� �̿ϼ� ���� & ����û������� [���ϼ���] �� ���
		if(nInterval == 0 && m_buyDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}

	// �ߺ����� Ƚ�� �ʱ�ȭ
	m_buyDealTrace.InitRepeatCount();

	// ����û�� ����
	m_buyDealTrace.SetForcedLiquidation(FLInfo.m_nExitIndex, FLInfo.m_lDealValue, bRealState);
}

void CStrategyWnd::SellForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)				// �����ŷ� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);	// ��ȸ or ������ ��� ���Ǹ�����
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);	// ������ ��� ���ϼ���
	
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBullValue = 0.;		// ����� ��
	// %
	if(m_uLossRate_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dLossRate); 	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dClose + m_dLossRate;				// �����ݾ�(��) ����� ���

	// EntryPosition �� ExitPosition ���̿��� ������ �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		//if(dValueForCompare >= dBullValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_LOSSRATE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// ��ǥ�������� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBearValue = 0.;		// �϶��� ��
	// %
	if(m_uTargetBenefit_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dTargetBenefit);	// ��������(%) �϶��� ���
	// ��
	else
		dBearValue = dClose - m_dTargetBenefit;				// �����ݾ�(��) �϶��� ���

	// EntryPosition �� ExitPosition ���̿��� ��ǥ���� �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� �϶��� ���
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_TARGETBENEFIT, pHigh);
			break;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// ������϶����� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dLowValue = 999999999999.;
	double dCloseValue = 0.;
	double dLowEach = 0.;
	double dCloseEach = 0.;

	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dLowEach = pLow->GetAt(nIndx);

		// ���� ������ �� ���� ��� �������� ����
		if(dLowValue - dLowEach > 0.)
			dLowValue = dLowEach;


		dCloseEach = pClose->GetAt(nIndx);
		double dBullValue = 0.;		// ����� ��
		// %
		if(m_uHPriceCPBear_PerOrWon == 0)
			dBullValue = dLowValue + (dLowValue * m_dHPriceCPBear);		// ��������(%) ����� ���
		// ��
		else
			dBullValue = dLowValue + m_dHPriceCPBear;	 					// �����ݾ�(��) ����� ���

		// �������� �Ǵ� �����ݾ����� ������ ���� ���ذ����� ����� ���
		//if(dCloseEach >= dBullValue)
		int nVal1 = dCloseEach*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_HPRICECPBEAR, pHigh);
			return;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)	// ��ǥ�������� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBearValue = 0.;		// �϶��� ��
	// %
	if(m_uTargetValuePreserve_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dTargetValuePreserve);	// ��������(%) �϶��� ���
	// ��
	else
		dBearValue = dClose - m_dTargetValuePreserve;				// �����ݾ�(��) �϶��� ���

	// EntryPosition �� ExitPosition ���̿��� ��ǥ������ �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare = 0.;
	double dValueForTargetValuePreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ��ǥ������������ �϶��� ���
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			dValueForTargetValuePreserve = dBearValue;
			break;
		}
	}
	
	// ��ǥ������ �ְ� ������ Position ���� ExitPosition ���̿��� �ְ� ��� ���Ŀ� ���� ����û�� Position ��ȸ
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ��ǥ������������ ����� ���
		//if(dValueForCompare > dValueForTargetValuePreserve)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dValueForTargetValuePreserve*100;
		if(nVal1 > nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_TARGETVALUEPRESERVE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// ���ͺ����� ���� ����û�� 
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	double dBearValue = 0.;		// ����� ��
	// %
	if(m_uBenefitPreserveUp_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dBenefitPreserveUp);	// ��������(%) �϶��� ���
	// ��
	else
		dBearValue = dClose - m_dBenefitPreserveUp;				// �����ݾ�(��) �϶��� ���

	// EntryPosition �� ExitPosition ���̿��� ���ͺ��� �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare = 0.;
	double dValueForBenefitPreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// ������ ��� ���ϼ���
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� �϶��� ���
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			dValueForBenefitPreserve = dBearValue;
			break;
		}
	}

	double dBullValue = 0.;		// �϶��� ��
	// %
	if(m_uBenefitPreserveDn_PerOrWon == 0)
		dBullValue = dValueForBenefitPreserve + (dValueForBenefitPreserve * m_dBenefitPreserveDn);	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dValueForBenefitPreserve + m_dBenefitPreserveDn;								// �����ݾ�(��) ����� ���

	// ���ͺ��� �ְ� ������ Position ���� ExitPosition ���̿��� �϶��� �����Ͽ� ����û���� Position ��ȸ
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		//if(dValueForCompare >= dBullValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_BENEFITPRESERVE, pHigh);
			break;
		}
	}	
}

void CStrategyWnd::SellForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// �ּҰ������� ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// ��ȸ or ������ ��� ���Ǹ�����	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// ������ ��� ���ϼ���

	if(m_nBongCount > 0)
	{
		// EntryPosition �� ExitPosition ���̿��� �ּҰ��ݺ�ȭ �ɼ����� ����û���� Position ��ȸ
		double dValueForCompare;
		double dRatio = m_dMiniPrice;
		int nFLIndx = 0;
		int nFLCount = 0;
		if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		{
			nFLIndx = m_sellDealTrace.GetCurrIndex() + m_nBongCount;	// ��ȸ or ������ ��� ���Ǹ�����
			nFLCount = m_sellDealTrace.GetRawIndex();
		}
		else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		{
			nFLIndx = m_sellDealTrace.GetOrdrIndex() + m_nBongCount;	// ������ ��� ���ϼ���
			nFLCount = m_sellDealTrace.GetCurrIndex();
		}

		for(int nIndx = nFLIndx; nIndx <= nFLCount; )
		{
			dValueForCompare = pClose->GetAt(nIndx);

			double dBearValue = 0.;		// �϶��� ��
			// %
			if(m_uMiniPrice_PerOrWon == 0)
				dBearValue = dClose - (dClose * dRatio); 	// �����Ǵ� ��������(%) ����� ���
			// ��
			else
				dBearValue = dClose - dRatio;				// �����Ǵ� �����ݾ�(��) ����� ���

			// ������ �������� �Ǵ� �����ݾ��̳��� Ⱦ���� ������ ���
			// �� ���ذ��� �������� ���� ���
			//if(dValueForCompare >= dBearValue)
			int nVal1 = dValueForCompare*100;
			int nVal2 = dBearValue*100;
			if(nVal1 >= nVal2)
			{
				FLInfo.SetFLInfo(nIndx, FL_SELL_MINIPRICE, pHigh);
				break;
			}
			nIndx += m_nBongCount;
			dRatio += m_dMiniPrice;
		}
	}	
}

void CStrategyWnd::SellForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// ����û������ ���� ����û��
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);

	CString szDate;
	CString szHour;
	int		nMin;
	CString szCheckHour;
	int		nCheckMin;

	// �� ����
	int	nInterval = m_nUnitPos;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;			// ��ȸ or ������ ��� ���Ǹ�����
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;			// ������ ��� ���ϼ���
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		// ���Ͻú�(�� : 10281310)
		szDate.Format("%08.8s", m_pDateItem->GetAt(nIndx));
		szHour = szDate.Mid(4, 2);
		nMin = atoi(szDate.Mid(6, 2));

		// ������ �ú�(�� : 1310)
		szCheckHour = m_szTimeDayLiquidation.Mid(0, 2);
		nCheckMin = atoi(m_szTimeDayLiquidation.Mid(2));
		if(szCheckHour.Compare(szHour) == 0 &&		// Hour Check
			nCheckMin <= nMin && abs(nCheckMin - nMin) <= nInterval)		// Min Check
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_DAYLIQUIDATION, pHigh);
			break;
		}
	}	
}

void CStrategyWnd::SellForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	if(FLInfo.m_nExitIndex == SENTINEL_FLINFO || 
		FLInfo.m_lDealValue == 0L ||
		FLInfo.m_pDataItem == NULL)
		return;

	if(bRealState && m_sellDealTrace.GetTimingOrder() == 0)			// ��ȸ or ������ ��� ���Ǹ�����
	{
		// �� �̿ϼ� ���� & ����û������� [���ϼ���] �� ���
		if(nInterval == 0 && m_sellDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)	// ������ ��� ���ϼ���
	{
		// �� �̿ϼ� ���� & ����û������� [���ϼ���] �� ���
		if(nInterval == 0 && m_sellDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}

	// �ߺ����� Ƚ�� �ʱ�ȭ
	m_sellDealTrace.InitRepeatCount();

	// ����û�� ����
	m_sellDealTrace.SetForcedLiquidation(FLInfo.m_nExitIndex, FLInfo.m_lDealValue, bRealState);
}

//�����м��� �Ϸ� ����..
void CStrategyWnd::OnSTSetAPDataST()
{
	m_Status = STRATEGY_RUN;

	m_ptrArraySDataItem.RemoveAll();
	m_ptrArrayPlot.RemoveAll();
	m_ptrArrayHistoryDataItem.RemoveAll();			

	CPtrArray *pHistoryData = m_pLogicRootItem->GetPArrayHistoryData();
	int nSizeOfHistory = pHistoryData->GetSize();
	for(int nPosOfHistory = 0 ; nPosOfHistory<nSizeOfHistory;nPosOfHistory++)
	{
		m_ptrArrayHistoryDataItem.Add(pHistoryData->GetAt(nPosOfHistory));
	}

	CTime t = CTime::GetCurrentTime();
	m_strCurrentTime = t.Format("%Y%m%d");
	m_strCurrentTime.Delete(0,2);
	m_pDateItem = m_pLogicRootItem->GetDateItem();
	m_nLastSize = m_pDateItem->GetSize();
	m_strTimeTypeNum = m_pLogicRootItem->GetTime();

	CDealData *pDealData = NULL;
	for(int nPos=0;nPos < m_nLastSize; nPos++)
	{
		pDealData = m_pLogicRootItem->GetValue(m_pDateItem->GetAt(nPos),m_pDateItem->GetType(),(LPCTSTR)m_strCurrentTime,nPos, m_strTimeTypeNum);

		// ��ȸó���� ���� 2�� �̻����� �����Ǿ� �ְ� 
		// ��ȸ�� �� ������ ���� ��ȣ������ �����ϴ� ��� (���� ���� �ϼ����� ���� ����)
		// ������ ���� �ϼ����� �ʾ����� ��ȣ������ �����ϹǷ� ��ȣǥ�ð� �Ǿ���.
		// ������ �̷� ���� ���� ������Ʈ�� ��ȣ������ �ľ��ϴµ� ���װ� �߻��Ͽ�
		// �� ������ ���� ��ȣ������ �����ϵ��� ó���Ѵ�.
		if( (m_nDivideState == 4) && /*m_nUnitPos != 60) &&*/		// �к������� �����
			(nPos == m_nLastSize - 1) &&							// �� ������ ��
			((pDealData->m_lDealValue & VALBUY) || (pDealData->m_lDealValue & VALSELL)))	// ��ȣ����
			break;

		m_dealDataItem.SetData(nPos
								,pDealData->m_lDealValue
								,pDealData->m_lType
								,pDealData->m_strDealName
								,pDealData->m_dPrice
								,pDealData->m_dContract
								,&pDealData->m_strListEntryNameForExit);
	}
	

	//-------- �ʱ�ȭ ���� ���� -------- 
	InitStrategySignalInfo();

	m_buyDealTrace.SetRepeatInfo(m_nUseRepeatEntry, m_nRepeatEntryCount);
	m_sellDealTrace.SetRepeatInfo(m_nUseRepeatEntry, m_nRepeatEntryCount);	

	ExecuteDealTrace();

	m_buyDealTrace.SetTimingInfo(m_nTiming_Order, m_nTiming_ForcedLiquidation);
	m_sellDealTrace.SetTimingInfo(m_nTiming_Order, m_nTiming_ForcedLiquidation);

//	//-------- ������ȣ���� �ʱ�ȭ -------- 
//	CDealData* pEntryDealData	= NULL;
//	CDealData* pExitDealData	= NULL;
//	CDealData* pDealData_ForReal = NULL;
//	InitStrategySignalInfo();
//
//	//--------  ��ȣ���� ��ȸ���� -------- 
//	m_bIsFirstSignal = TRUE;
//	m_nRepeatCount = 0;								// �ߺ����� Ƚ������ �ʱ�ȭ
//	m_bCheckPrev_EntryPosition = TRUE;				// ���� ��ȣ�� ���Խ�ȣ
//	BOOL bExist_ForcedLiquidation = FALSE;			// ����û�� �߻�����
//	POSITION pos = m_dealDataItem.GetHeadPosition();
//	while(pos)
//	{
//		// ���� Position
//		pEntryDealData = m_dealDataItem.GetNext(pos);
//
//		// û���ȣ�� ��ó���� ��� (�� ���ǹ��� TRUE�� ��� û���ȣ������ �ǹ���)
//		if(IsFirstLiquidation(pEntryDealData, bExist_ForcedLiquidation))
//			continue;
//
//		// ���Խ�ȣ�� �������� ���
//		if(IsLastEntry(pEntryDealData, bExist_ForcedLiquidation, pos))
//			break;
//
//		// û�� Position
//		pExitDealData = m_dealDataItem.GetNext(pos);
//
//		// ����û�� �ɼ�Ȯ��
//		Confirm_ForcedLiquidation(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//	}
	

	//20081111 �̹��� >>
	//��������ȭ������ ������ �Ѵ�.////
	//((CLogicST*)m_pLogicRootItem)->SetPosition(NULL);	
	m_pLogicRootItem->SetPosition(NULL);


	m_DrawInfo.bIsInRealTime = FALSE;
	m_bHasAPData = TRUE;

	m_ptrArraySDataItem.Add(m_pDateItem);
	m_DrawInfo.pPtrArraySDataItem = &m_ptrArraySDataItem;

	

	m_DrawInfo.pPtrArrayHistoryDataItem = &m_ptrArrayHistoryDataItem;
	m_DrawInfo.pPtrArrayPlot = &m_ptrArrayPlot;

	
	m_DrawInfo.pDealDataItem = &m_dealDataItem;
	m_DrawInfo.pArDealPosition = &m_arrayDealPosition;
	m_DrawInfo.pArDealName = &m_arrayDealName;
	m_DrawInfo.pArDealDrawPosition = &m_arrayDealDrawPosition;
	m_DrawInfo.STPath = m_strStrategyPath;//KHD : Path : �����м����� ���������� ����������..
	

	if(m_bUseDayLiquidation)
	{
		ExitAtTodayByTimeSelected();
	}




//	// �ǽð� ������Ʈ�� ���� �غ�
//	// �ֹ����� ���ÿϷ� ���� �ʱ�ȭ
//	m_bSetComplete_OrderInfo = FALSE;
//	
//	// �ֹ����� ���Ű��� ���� �ʱ�ȭ
//	m_bCanRemove_OrderInfo = TRUE;
}

void CStrategyWnd::OnSTSetRealDataST()
{
	int nTotalCount = m_pDateItem->GetSize();
	int nCurrIndex = nTotalCount - 1;
	int nInterval = nTotalCount - m_nLastSize;

	CDealData* pDealData = m_pLogicRootItem->GetValue(m_pDateItem->GetAt(nCurrIndex), m_pDateItem->GetType(), (LPCTSTR)m_strCurrentTime, nCurrIndex, m_strTimeTypeNum);

	ExecuteDealTraceReal(pDealData, nTotalCount, nInterval);

//	// �ǽð� ������Ʈ�� �� ����ȣ ����
//	int nTotalSignalCnt = m_pDateItem->GetSize();
//	
//	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
//	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);
//	int nInterval = nTotalSignalCnt - m_nLastSize;			// �ǽð� ������Ʈ�� �߰��� ����ȣ ����
//
//	// ��ȣ������ ���� ��� �ǽð� ������Ʈ�� ����ȣ�� ����
//	CDealData* pDealDataReal = m_pLogicRootItem->GetValue(m_pDateItem->GetAt(nTotalSignalCnt - 1), m_pDateItem->GetType(), (LPCTSTR)m_strCurrentTime, nTotalSignalCnt - 1, m_strTimeTypeNum);
//
//	// ��ȸ�����Ͱ� ���ų� 1ƽ�� ���
//	if(m_dealDataItem.GetCount() == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
//	{
//		ResizeSignalInfoArray(nInterval, nTotalSignalCnt);
//
//		// ��ȸ�����Ͱ� ���� ���
//		if(m_dealDataItem.GetCount() == 0)
//		{
//			int nRealPosition = nTotalSignalCnt - 1;
//			m_dealDataItem.SetData(nRealPosition, 
//									pDealDataReal->m_lDealValue,
//									pDealDataReal->m_lType,
//									pDealDataReal->m_strDealName,
//									pDealDataReal->m_dPrice,
//									pDealDataReal->m_dContract,
//									&pDealDataReal->m_strListEntryNameForExit);
//
//			// �ֹ����� ���ÿϷ� ���� �ʱ�ȭ
//			m_bSetComplete_OrderInfo = FALSE;
//			
//			// �ֹ����� ���Ű��� ���� �ʱ�ȭ
//			m_bCanRemove_OrderInfo = TRUE;
//
//			m_nLastSize = nTotalSignalCnt;
//			return;
//		}
//
//		// 1ƽ�� ���
//		CDealData* pEntryDealData = m_dealDataItem.GetTail();
//		CDealData* pExitDealData = pDealDataReal;
//
//		// �ߺ����� ��� ���� Ȯ��
//		if((m_nUseRepeatEntry && m_nRepeatEntryCount >= m_nRepeatCount) ||
//		   (m_nUseRepeatEntry == 0 && 
//			   (IsBuyOrExitShort(pEntryDealData->m_lDealValue) && (IsSellOrExitLong(pExitDealData->m_lDealValue) || IsForcedLiquidation(pExitDealData->m_lDealValue)) ||
//			   (IsBuyOrExitShort(pExitDealData->m_lDealValue) && (IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue))))))
//		{
//			pExitDealData->m_nIndex = nTotalSignalCnt - 1;
//
//			// ���� - ����/û��/�Ϲݽ�ȣ�� ���
//			BOOL bExist_ForcedLiquidation = FALSE;		// ����û�� �߻�����
//			if(IsBuyOrExitShort(pEntryDealData->m_lDealValue))
//			{
//				// ����û�� �ɼ�Ȯ��
//				Confirm_ForcedLiquidation_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//			}
//			// // û�� - ����/�Ϲݽ�ȣ�� ���
//			else if(IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue))
//			{
//				// ���� ���� ��ȣ�� ���Խ�ȣ�� ���
//				IsLastEntry_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//			}
//		}
//
//		m_TempDealData.Clear();
//
//		// �ֹ����� ���ÿϷ� ���� �ʱ�ȭ
//		m_bSetComplete_OrderInfo = FALSE;
//		
//		// �ֹ����� ���Ű��� ���� �ʱ�ȭ
//		m_bCanRemove_OrderInfo = TRUE;
//
//		m_nLastSize = nTotalSignalCnt;
//		return;
//	}
//
//	if(nInterval != 0)
//	{
//		ResizeSignalInfoArray(nInterval, nTotalSignalCnt);
//
//		CDealData* pEntryDealData = m_dealDataItem.GetTail();
//		CDealData* pExitDealData = &m_TempDealData;
//
//		// �ߺ����� ��� ���� Ȯ��
//		if((m_nUseRepeatEntry && m_nRepeatEntryCount >= m_nRepeatCount) || m_nUseRepeatEntry == 0)
//		{
//			pExitDealData->m_nIndex = nTotalSignalCnt - 1;
//
//			// �ֹ����� (1 : �� �ϼ���)
//			if(m_nTiming_Order == 1)
//			{
//				// ����û���� ���Ǹ������� ���
//				if(!m_nTiming_ForcedLiquidation && IsForcedLiquidation(m_TempForcedDealData.m_lDealValue))
//				{
//					// ���ϼ����� �ֹ������� ���Ǹ������� ����â�� ������ ���� ���
//					if(m_TempForcedDealData.m_nIndex <= m_TempDealData.m_nIndex &&
//						(!IsSellOrExitLong(m_TempDealData.m_lDealValue) || !IsForcedLiquidation(m_TempDealData.m_lDealValue)))
//					{
//						m_TempDealData = m_TempForcedDealData;
//						m_TempForcedDealData.Clear();
//					}
//				}
//
//				int nRealPosition = -1;
//				if(IsBuyOrExitShort(pExitDealData->m_lDealValue) && (IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue)))
//				{
//					// ��ȣ����� �Ʒ��ʿ� ���̹Ƿ� ������ ����
//					nRealPosition = pExitDealData->m_nIndex;
//					m_arrayDealPosition.SetAt(nRealPosition, pExitDealData->m_lDealValue);
//					m_arrayDealName.SetAt(nRealPosition, pExitDealData->m_strDealName);
//					m_arrayDealDrawPosition.SetAt(nRealPosition, pLow->GetAt(nRealPosition));
//
//					// �ߺ����� Ƚ�� ����
//					m_nRepeatCount++;
////					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
////					if(fp != NULL) 
////					{
////						fprintf(fp, "[++DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "OnSTSetRealDataST()���ϼ�");
////						fclose(fp);
////					}
//				}
//				else if(IsBuyOrExitShort(pEntryDealData->m_lDealValue) && (IsSellOrExitLong(pExitDealData->m_lDealValue) || IsForcedLiquidation(pExitDealData->m_lDealValue)))
//				{
//					// ��ȣ����� ���ʿ� ���̹Ƿ� ���� ����
//					nRealPosition = pExitDealData->m_nIndex;
//					m_arrayDealPosition.SetAt(nRealPosition, pExitDealData->m_lDealValue);
//					m_arrayDealName.SetAt(nRealPosition, pExitDealData->m_strDealName);
//					m_arrayDealDrawPosition.SetAt(nRealPosition, pHigh->GetAt(nRealPosition));
//
//					// �ߺ����� Ƚ������ �ʱ�ȭ
//					m_nRepeatCount = 0;
////					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
////					if(fp != NULL) 
////					{
////						fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "OnSTSetRealDataST()���ϼ�");
////						fclose(fp);
////					}
//				}
//				else
//				{
//					// ��ȣ����
//					m_arrayDealPosition.SetAt(pExitDealData->m_nIndex, pExitDealData->m_nIndex);
//					m_arrayDealName.SetAt(pExitDealData->m_nIndex, "");
//					m_arrayDealDrawPosition.SetAt(pExitDealData->m_nIndex, 0);
//				}
//
//				m_dealDataItem.SetData(pExitDealData->m_nIndex, 
//										pExitDealData->m_lDealValue,
//										pExitDealData->m_lType,
//										pExitDealData->m_strDealName,
//										pExitDealData->m_dPrice,
//										pExitDealData->m_dContract,
//										&pExitDealData->m_strListEntryNameForExit);	
//			}
//			// ���Ǹ�����
//			else
//			{
//				// ����û���� ���ϼ����� ���
//				if(m_nTiming_ForcedLiquidation && IsForcedLiquidation(m_TempDealData.m_lDealValue))
//				{
//					// ���Ǹ������� �ֹ������� ���ϼ����� ����â�� ������ ���� ���
//					if(m_TempCondDealData.m_nIndex == -1 || 
//						(m_TempDealData.m_nIndex <= m_TempCondDealData.m_nIndex && 
//						(!IsSellOrExitLong(m_TempCondDealData.m_lDealValue) || !IsForcedLiquidation(m_TempCondDealData.m_lDealValue))))
//					{
//						m_TempCondDealData = m_TempDealData;
//					}
//
//					// �ߺ����� Ƚ������ �ʱ�ȭ
//					m_nRepeatCount = 0;
////					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
////					if(fp != NULL) 
////					{
////						fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "OnSTSetRealDataST()");
////						fclose(fp);
////					}
//				}
//				m_dealDataItem.SetData(m_TempCondDealData.m_nIndex, 
//										m_TempCondDealData.m_lDealValue,
//										m_TempCondDealData.m_lType,
//										m_TempCondDealData.m_strDealName,
//										m_TempCondDealData.m_dPrice,
//										m_TempCondDealData.m_dContract,
//										&m_TempCondDealData.m_strListEntryNameForExit);
//			}
//		}
//
//		m_TempDealData.Clear();
//		m_TempCondDealData.Clear();
//
//		// �ֹ����� ���ÿϷ� ���� �ʱ�ȭ
//		m_bSetComplete_OrderInfo = FALSE;
//		
//		// �ֹ����� ���Ű��� ���� �ʱ�ȭ
//		m_bCanRemove_OrderInfo = TRUE;
//
//		m_nLastSize = nTotalSignalCnt;
//		return;
//	}
//
//	// ������ �� �ǽð� ������Ʈ�� ��ȣ������ ����
//	CDealData* pEntryDealData = m_dealDataItem.GetTail();
//	CDealData* pExitDealData = pDealDataReal;
//	pExitDealData->m_nIndex = nTotalSignalCnt - 1;
//
//	// ������ ����ȣ�� �Ϲ� �ǽð� �Ǵ� ���Խ�ȣ�� ���
//	BOOL bExist_ForcedLiquidation = FALSE;		// ����û�� �߻�����
//	if(IsBuyOrExitShort(pEntryDealData->m_lDealValue))
//	{
//		// ����û�� �ɼ�Ȯ��
//		Confirm_ForcedLiquidation_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//	}
//	// ������ ����ȣ�� û�� �Ǵ� ����û���ȣ�� ���
//	else if(IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue))
//	{
//		// ���� ���� ��ȣ�� ���Խ�ȣ�� ���
//		IsLastEntry_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//	}
//
//	m_nLastSize = nTotalSignalCnt;
}

void CStrategyWnd::ResizeSignalInfoArray(int nInterval, int nTotalSignalCnt)
{
	// �� ����ȣ ������ �پ�� ��� (��Ʈ������ ������ ������ ����)
	if(nInterval < 0)
	{
		// �پ�� ����ȣ ������ŭ ���� ����ȣ�� ����
		m_arrayDealPosition.RemoveAt(0, (nInterval * (-1)) + 1);
		m_arrayDealName.RemoveAt(0, (nInterval * (-1)) + 1);
		m_arrayDealDrawPosition.RemoveAt(0, (nInterval * (-1)) + 1);

		// �ǽð� ������Ʈ�� �� ����ȣ ������ Size ������
		m_arrayDealPosition.SetSize(nTotalSignalCnt);
		m_arrayDealName.SetSize(nTotalSignalCnt);
		m_arrayDealDrawPosition.SetSize(nTotalSignalCnt);
	}
	// �� ����ȣ ������ �þ ��� 
	else if(nInterval > 0)
	{
		// �ǽð� ������Ʈ�� �� ����ȣ ������ Size ������
		m_arrayDealPosition.SetSize(nTotalSignalCnt);
		m_arrayDealName.SetSize(nTotalSignalCnt);
		m_arrayDealDrawPosition.SetSize(nTotalSignalCnt);
	}
}

LRESULT CStrategyWnd::OnSTDataManagerMessageST( WPARAM wParam, LPARAM lParam)
{
	CLogicST *pLogicST= (CLogicST *)wParam;
	pLogicST->ReceiveFromSTDataManager(lParam);//DrawInfo�� ������.:KHD
	
	if(m_pLogicRootItem->IsFilled())
	{
		m_DrawInfo.bIsInRealTime = TRUE;
		if(!m_bHasAPData)
		{
			OnSTSetAPDataST();
		}
		else
		{
			OnSTSetRealDataST();
		}		

		// ��ȣ���� �� �ֹ����� ����
		GetParent()->SendMessage(RMSG_STDATAMANAGER_RESULTDATA,m_lKey,(LPARAM)&m_DrawInfo);

//		// ���Ǹ����ÿ� ���� �ֹ����� ���� �� �ֹ����� ����
//		// ���Ǹ����� ���� ��ȣ�� ���� �ֹ��� ����Ǳ� ����
//		if(m_bSetComplete_OrderInfo && m_bCanRemove_OrderInfo && (m_nDivideState != 5 || m_nUnitPos != 1))
//		{
//			m_dealDataItem.RemoveTail();
//			m_bCanRemove_OrderInfo = FALSE;
//		}

		m_pLogicRootItem->InitFilledSet();
	}	
	return 1L;
}

BOOL CStrategyWnd::IsRunning()
{
	return m_bIsRunning;
}

BOOL CStrategyWnd::HasAPData()
{
	return m_bHasAPData;
}

CString CStrategyWnd::GetInputCode()
{
	return m_strInputCode;
}

CString CStrategyWnd::GetEndDate()
{
	return m_strEndDate;
}

long CStrategyWnd::GetCount()
{
	if(m_pLogicRootItem)
	{
		return (m_pLogicRootItem->GetCount()+1);
	}
	return 0;	
}

long CStrategyWnd::GetDataCount(long lIndex)
{
	if(m_pLogicRootItem)
	{
		CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
		if(pLogicST)
		{
			return pLogicST->GetDataCount();
		}
		else
		{
			return m_pDateItem->GetSize();
		}
	}
	return 0;
}

CString CStrategyWnd::GetInputs(long lIndexST, long lIndexInput)
{
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndexST);
	if(pLogicST)
	{
		return pLogicST->GetInputs(lIndexInput);
	}
	return "#";
}

CString CStrategyWnd::GetTime(long lIndex)
{
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		return pLogicST->GetTime();
	}
	else
	{
		return m_pLogicRootItem->GetTime();
	}

	return "";
}

CString CStrategyWnd::GetName(long lIndex)
{
	CString strName;
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		strName = pLogicST->GetPath();
	}
	else
	{
		strName = m_strStrategyPath;
	}
	int nPos = strName.ReverseFind('\\');
	if(nPos!=-1)
	{
		strName.Delete(0,nPos+1);
	}
	return strName;
}

CString CStrategyWnd::GetPath(long lIndex)
{
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		return pLogicST->GetPath();
	}
	else
	{
		return m_strStrategyPath;
	}
	return "";
}

long CStrategyWnd::GetPtrDrawInfo(long lIndex)
{
	if(!m_pLogicRootItem) return NULL;
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		return pLogicST->GetPtrST();
	}
	else
	{
		return (long)&m_DrawInfo;
	}
	return NULL;
}

void CStrategyWnd::RemoveAllArray()
{
	CStrategyResult *pStrategyResult = NULL;
	int nSize = m_arrayStrategyResult.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pStrategyResult = m_arrayStrategyResult.GetAt(nPos);
		delete pStrategyResult;
	}
	m_arrayStrategyResult.RemoveAll();
}

CStrategyResult *CStrategyWnd::CreateStrategyResultData()
{
	CStrategyResult *pStrategyResult = new CStrategyResult;
	m_arrayStrategyResult.Add(pStrategyResult);
	return pStrategyResult;
}


void CStrategyWnd::CalaulateStrategyResultData()
{
	int nLastPos = m_arrayStrategyResult.GetSize() - 1;
	CStrategyResult *pStrategyResultLast = m_arrayStrategyResult.GetAt(nLastPos);
	CStrategyResult *pStrategyResultBefore = NULL;
	if(nLastPos>0)
		pStrategyResultBefore = m_arrayStrategyResult.GetAt(nLastPos-1);

	// �ż�û��
	if(pStrategyResultLast->m_DealType==CStrategyResult::EXITLONG)
	{
		CStrategyResult *pStrategyResultInArray = NULL;
		CStrategyResult *pStrategyResultExitBefore = NULL;
		double dSumBought = 0;

		if(m_pFeeCal && m_pSlippageCal)
		{
			m_pFeeCal->Init();
			m_pSlippageCal->Init();
			
			// �ߺ������� ����Ͽ� For������ ��ȸ
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::BUY)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;
//				m_pFeeCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
//				m_pSlippageCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
			}
			pStrategyResultLast->m_dProfit = pStrategyResultLast->m_dAmount - dSumBought;
//				- m_pFeeCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty)
//				- m_pSlippageCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty);
		}
		else
		{
			// �ߺ������� ����Ͽ� For������ ��ȸ
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::BUY)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;
			}
			pStrategyResultLast->m_dProfit = pStrategyResultLast->m_dAmount - dSumBought;
		}
		pStrategyResultLast->m_dProfitRate = pStrategyResultLast->m_dProfit/dSumBought*100;
		if(pStrategyResultExitBefore)
		{
			pStrategyResultLast->m_dProfitAccmulated 
				= pStrategyResultExitBefore->m_dProfitAccmulated + pStrategyResultLast->m_dProfit;			
		}
		else
		{
			pStrategyResultLast->m_dProfitAccmulated = pStrategyResultLast->m_dProfit;
		}
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultBefore->m_dEnteredAmount;
			pStrategyResultLast->m_dProfitRateAccmulated 
				= pStrategyResultLast->m_dProfitAccmulated/pStrategyResultLast->m_dEnteredAmount*100;
		}

	}
	// �ŵ�û��
	else if(pStrategyResultLast->m_DealType==CStrategyResult::EXITSHORT)
	{
		CStrategyResult *pStrategyResultInArray = NULL;
		CStrategyResult *pStrategyResultExitBefore = NULL;
		double dSumBought = 0;
		if(m_pFeeCal&&m_pSlippageCal)
		{
			m_pFeeCal->Init();
			m_pSlippageCal->Init();

			// �ߺ������� ����Ͽ� For������ ��ȸ
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::SELL)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;			
//				m_pFeeCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
//				m_pSlippageCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
			}
			pStrategyResultLast->m_dProfit = dSumBought - pStrategyResultLast->m_dAmount;
//					- m_pFeeCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty)
//					- m_pSlippageCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty);
		}
		else
		{
			// �ߺ������� ����Ͽ� For������ ��ȸ
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::SELL)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;			
			}
			pStrategyResultLast->m_dProfit = dSumBought - pStrategyResultLast->m_dAmount;
		}

		pStrategyResultLast->m_dProfitRate = pStrategyResultLast->m_dProfit/dSumBought*100;
		if(pStrategyResultExitBefore)
		{
			pStrategyResultLast->m_dProfitAccmulated 
				= pStrategyResultExitBefore->m_dProfitAccmulated + pStrategyResultLast->m_dProfit;			
		}
		else
		{
			pStrategyResultLast->m_dProfitAccmulated = pStrategyResultLast->m_dProfit;
		}
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultBefore->m_dEnteredAmount;
			pStrategyResultLast->m_dProfitRateAccmulated 
				= pStrategyResultLast->m_dProfitAccmulated/pStrategyResultLast->m_dEnteredAmount*100;
		}
	}
	// �ż�
	else if(pStrategyResultLast->m_DealType==CStrategyResult::BUY)
	{	
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount 
				= pStrategyResultLast->m_dAmount + pStrategyResultBefore->m_dEnteredAmount;
		}
		else
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultLast->m_dAmount;		
		}		
	}
	// �ŵ�
	else if(pStrategyResultLast->m_DealType==CStrategyResult::SELL)
	{	
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount 
				= pStrategyResultLast->m_dAmount + pStrategyResultBefore->m_dEnteredAmount;
		}
		else
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultLast->m_dAmount;		
		}		
	}
}

double CStrategyWnd::ApplySlipAndTex(double dMulData, BOOL bType,double nNum)
{
	double dResultSlip;
	double dTex;
	if(m_lSlippageUnit)//0: % , 1=P
	{
		dResultSlip = dMulData + m_dSlippageForEntry*nNum;
	}
	else//%
	{
		dResultSlip = dMulData * (m_dSlippageForExit*0.01);
	}

	if(bType)//����
	{
		if(m_lFeeUnit)
		{
			dTex = ((dMulData+ dResultSlip)+m_dFeeForEntry);
			return (dMulData + dResultSlip + dTex);
		}
		else
		{	dTex = ((dMulData + dResultSlip)*(m_dFeeForEntry*0.01));
			return (dMulData + dResultSlip + dTex);
		}
	}
	else//û��
	{
		if(m_lFeeUnit)
		{
			dTex = ((dMulData+ dResultSlip)+m_dFeeForExit);
			return (dMulData + dResultSlip - dTex);
		}
		else
		{
			dTex = ((dMulData+ dResultSlip)*(m_dFeeForExit*0.01));
			return (dMulData + dResultSlip - dTex);
		}
	}

	return 0;
}

//���⼭ �����м� �����Ͱ� ����. :KHD
BOOL CStrategyWnd::CalculateStatisticalInfo(long lStartPos, long lEndPos)
{

	if(m_Status==STRATEGY_NONE||m_Status==STRATEGY_STANDBY) 
	{
		ASSERT(0);
		return FALSE;
	}
	CString strDate;
	//long  lResultDealType = 0;
	double dResultClose = 0;
	double dResultSlip =0;//�������� : KHD
	int nSize = m_pDateItem->GetSize();
	CIDataItem *pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	RemoveAllArray();

	// vntsorl
	CStrategyResult *pStrategyResultLast = NULL;
	CStrategyResult *pStrategyResult = NULL;
	double dVolume = m_nOrderAmount;//���� : �������뿡 �°�..
	long lBuyVolOfHolding = 0;
	long lSellVolOfHolding = 0;
	if(lEndPos>=nSize)
	{
		lEndPos = nSize - 1;
	}
	else if(lStartPos==-1||lEndPos==-1)
	{
		lStartPos = 0;
		lEndPos = nSize - 1;
	}
	m_lAllCandleInCalculation = lEndPos - lStartPos + 1;
	long lLastDealIndex = 0;
	CDealData* pDealData = NULL;
	
	POSITION pos = m_dealDataItem.GetHeadPosition();
	int nDealPositionSize = m_arrayDealPosition.GetSize();
	double dDealPosition = 0;
	for(int nIndx = 0; nIndx < nDealPositionSize; ++nIndx)
	{
		if(dDealPosition = m_arrayDealPosition.GetAt(nIndx))
		{

			while(pos)
			{
				if((pDealData = m_dealDataItem.GetNext(pos)) && (pDealData->m_nIndex == nIndx))
				{
					if(pDealData->m_nIndex>=lStartPos&&pDealData->m_nIndex<=lEndPos)
					{
						if(pDealData->m_lDealValue!=0) //�� ���� ã�ƾ� �Ѵ�.(�ɼǼ����̷�..)
						{

//////////////////////////////////////////////////////////////////////////
							strDate = m_pDateItem->GetAt(nIndx);		
							dResultClose = pClose->GetAt(nIndx);	// Close Price

							// �ż� / �ż�û��
							if((long)dDealPosition & (VALENTRYBUY | VALEXITLONG | 
										FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
										FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
										FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION))
							{
								if(lBuyVolOfHolding > 0)
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::EXITLONG;	//�ż�û��
									pStrategyResult->m_dTime = atof(strDate);
									pStrategyResult->m_lCntCandleInAccess = nIndx - lLastDealIndex;
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = lBuyVolOfHolding;

									//�������� ���ϱ� 
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

									CalaulateStrategyResultData();
									lBuyVolOfHolding = 0;
								}				
								if((long)dDealPosition & VALENTRYBUY)
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::BUY;//�ż�
									pStrategyResult->m_dTime = atof(strDate);					
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = (long)dVolume;

									//�������� ���ϱ� 
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, TRUE, dVolume);

									CalaulateStrategyResultData();
									if(!lBuyVolOfHolding)
									{
										lLastDealIndex = nIndx;
									}
									lBuyVolOfHolding += (long)dVolume;
								}				
							}
							
							// �ŵ� / �ŵ�û��
							if((long)dDealPosition & (VALENTRYSELL | VALEXITSHORT | 
										FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
										FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
										FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION))
							{
								if(lSellVolOfHolding < 0)
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;

									pStrategyResult->m_dTime = atof(strDate);
									pStrategyResult->m_lCntCandleInAccess = nIndx - lLastDealIndex;
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = lSellVolOfHolding * -1;
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

									CalaulateStrategyResultData();
									lSellVolOfHolding = 0;

								}				
								if(m_strInputCode.GetLength() != QUOTEINDEXLENGTH && ((long)dDealPosition & VALENTRYSELL))
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::SELL;
									
									pStrategyResult->m_dTime = atof(strDate);
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = (long)dVolume;
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, TRUE, dVolume);

									CalaulateStrategyResultData();
									if(!lSellVolOfHolding)
									{
										lLastDealIndex = (long)nIndx;
									}
									lSellVolOfHolding -= (long)dVolume;
								}
							}
//////////////////////////////////////////////////////////////////////////
						}
					}
					break;
				}
			}
		}
	}
	
//	CStrategyResult *pStrategyResultLast = NULL;
//	CStrategyResult *pStrategyResult = NULL;
//	double dVolume = m_nOrderAmount;//���� : �������뿡 �°�..
//	long lVolOfHolding = 0;
//	if(lEndPos>=nSize)
//	{
//		lEndPos = nSize - 1;
//	}
//	else if(lStartPos==-1||lEndPos==-1)
//	{
//		lStartPos = 0;
//		lEndPos = nSize - 1;
//	}
//	m_lAllCandleInCalculation = lEndPos - lStartPos + 1;
//	long lLastDealIndex = 0;
//	CDealData* pDealData = NULL;
//	
//	POSITION pos = m_dealDataItem.GetHeadPosition();
//	int nDealPositionSize = m_arrayDealPosition.GetSize();
//	double dDealPosition = 0;
//	for(int nIndx = 0; nIndx < nDealPositionSize; ++nIndx)
//	{
//		if(dDealPosition = m_arrayDealPosition.GetAt(nIndx))
//		{
//			while(pos)
//			{
//				if((pDealData = m_dealDataItem.GetNext(pos)) && (pDealData->m_nIndex == nIndx))
//				{
//					if(pDealData->m_nIndex>=lStartPos&&pDealData->m_nIndex<=lEndPos)
//					{
//						if(pDealData->m_lDealValue!=0) //�� ���� ã�ƾ� �Ѵ�.(�ɼǼ����̷�..)
//						{			
//							strDate = m_pDateItem->GetAt(pDealData->m_nIndex);		
//							dResultClose = pClose->GetAt(pDealData->m_nIndex);	// Close Price
//							if(pDealData->m_lDealValue&VALBUY)	//�ŵ�û��
//							{
//								if(lVolOfHolding<0)	// ExitShort........m_dMesuRate:������
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;//�ŵ�û��
//
//
//									pStrategyResult->m_dTime = atof(strDate);
//									pStrategyResult->m_lCntCandleInAccess = pDealData->m_nIndex - lLastDealIndex;
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = lVolOfHolding*-1;
////									pStrategyResult->m_dAmount = dResultClose*lVolOfHolding*-1;// //KHD
//									//�������� ���ϱ� 
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);
//
//									CalaulateStrategyResultData();
//									lVolOfHolding = 0;
//
//								}				
//								if((pDealData->m_lDealValue&VALENTRYBUY))	//�ż�Buy....�ż�����m_dMesuRate
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::BUY;//�ż�
//									pStrategyResult->m_dTime = atof(strDate);					
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = (long)dVolume;
////									pStrategyResult->m_dAmount = dResultClose*dVolume;//KHD
//									//�������� ���ϱ� 
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,TRUE,dVolume);
//
//									CalaulateStrategyResultData();
//									if(!lVolOfHolding)
//									{
//										lLastDealIndex = pDealData->m_nIndex;
//									}
//									lVolOfHolding+=(long)dVolume;
//								}				
//							}
//							else if(pDealData->m_lDealValue&VALSELL || IsForcedLiquidation(pDealData->m_lDealValue))	//�ż�û��	
//							{
//								if(lVolOfHolding>0)				// Exitlong....
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::EXITLONG;//�ż�û��
//
//									pStrategyResult->m_dTime = atof(strDate);
//									pStrategyResult->m_lCntCandleInAccess = pDealData->m_nIndex - lLastDealIndex;
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = lVolOfHolding;
////									pStrategyResult->m_dAmount = dResultClose*lVolOfHolding;//KHD
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);
//
//									CalaulateStrategyResultData();
//									lVolOfHolding = 0;
//
//								}				
//								if(m_strInputCode.GetLength()!=QUOTEINDEXLENGTH&&(pDealData->m_lDealValue&VALENTRYSELL))	// �ŵ�Sell....
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::SELL;//�ŵ�
//									pStrategyResult->m_dTime = atof(strDate);
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = (long)dVolume;
////									pStrategyResult->m_dAmount = dResultClose*dVolume;//KHD
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,TRUE,dVolume);
//
//									CalaulateStrategyResultData();
//									if(!lVolOfHolding)
//									{
//										lLastDealIndex = (long)pDealData->m_nIndex;
//									}
//									lVolOfHolding-=(long)dVolume;
//								}
//							}
//						}
//					}
//					break;
//				}
//			}//while
//		}
//	}

	if(lBuyVolOfHolding > 0)
	{
		pStrategyResult = CreateStrategyResultData();		
		strDate = m_pDateItem->GetAt(lEndPos);	
		dResultClose = pClose->GetAt(lEndPos);
		pStrategyResult->m_DealType = CStrategyResult::EXITLONG;

		pStrategyResult->m_dTime = atof(strDate);
		pStrategyResult->m_dPrice = dResultClose;
		pStrategyResult->m_lQty = lBuyVolOfHolding;
		pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

		pStrategyResult->m_bOnlyValuate = TRUE;

		CalaulateStrategyResultData();
		lBuyVolOfHolding = 0;
	}

	if(lSellVolOfHolding < 0)
	{
		pStrategyResult = CreateStrategyResultData();
		strDate = m_pDateItem->GetAt(lEndPos);	
		dResultClose = pClose->GetAt(lEndPos);
		pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;
		pStrategyResult->m_dTime = atof(strDate);
		pStrategyResult->m_dPrice = dResultClose;
		pStrategyResult->m_lQty = lSellVolOfHolding * -1;
		pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

		pStrategyResult->m_bOnlyValuate = TRUE;

		CalaulateStrategyResultData();
		lSellVolOfHolding = 0;
	}

//	if(lVolOfHolding>0 )//�ż�û��
//	{
//		pStrategyResult = CreateStrategyResultData();		
//		strDate = m_pDateItem->GetAt(lEndPos);//nSize-1);		
//		dResultClose = pClose->GetAt(lEndPos);//nSize-1);	// Last Close Price
//		pStrategyResult->m_DealType = CStrategyResult::EXITLONG;//�ż�û��
//
//		pStrategyResult->m_dTime = atof(strDate);
//		pStrategyResult->m_dPrice = dResultClose;
//		pStrategyResult->m_lQty = lVolOfHolding;
////		pStrategyResult->m_dAmount = dResultClose*lVolOfHolding;//KHD
//		pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);//�߰�
//		pStrategyResult->m_bOnlyValuate = TRUE;
//
//		CalaulateStrategyResultData();
//		lVolOfHolding = 0;
//	}
//
//	else if(lVolOfHolding<0)
//	{
//		pStrategyResult = CreateStrategyResultData();
//		strDate = m_pDateItem->GetAt(lEndPos);//nSize-1);		
//		dResultClose = pClose->GetAt(lEndPos);//nSize-1);	// Last Close Price
//		pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;//�ŵ� û��
//		pStrategyResult->m_dTime = atof(strDate);
//		pStrategyResult->m_dPrice = dResultClose;
//		pStrategyResult->m_lQty = lVolOfHolding*-1;
////		pStrategyResult->m_dAmount = dResultClose*lVolOfHolding*-1;//KHD
//		pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);//�߰�
//		pStrategyResult->m_bOnlyValuate = TRUE;
//
//		CalaulateStrategyResultData();
//		lVolOfHolding = 0;
//
//	}
	
	m_dSumProfit = 0; 
	m_dSumLoss = 0;
	m_dMaxProfit = 0;
	m_dMaxLoss = 0;
	m_lCntProfit = 0;
	m_lCntLoss = 0;
	m_dSumAmountExit = 0;
	m_lCntProfitStraight = 0;
	m_lCntLossStraight = 0;
	m_lSumProfitStraight = 0;
	m_lSumLossStraight = 0;
	m_lSumCandleProfit = 0;
	m_lSumCandleLoss = 0;

	double dSumStraight = 0;
	long lCntStraight = 0;
	long lTypeStraight = 0;
	
	long lSize = m_arrayStrategyResult.GetSize();
	CStrategyResult *pStrategyResultInArray = NULL;
	for(long lPos=0;lPos<lSize;lPos++)
	{
		pStrategyResultInArray = m_arrayStrategyResult.GetAt(lPos);
		if(pStrategyResultInArray->m_dProfit>0)
		{
			m_dSumProfit += pStrategyResultInArray->m_dProfit;
			m_lSumCandleProfit += pStrategyResultInArray->m_lCntCandleInAccess;
			if(m_dMaxProfit<pStrategyResultInArray->m_dProfit)
			{
				m_dMaxProfit = pStrategyResultInArray->m_dProfit;
			}
			m_lCntProfit++;
			if(lTypeStraight==1)	// Keeping straight...
			{
				dSumStraight += pStrategyResultInArray->m_dProfit;
				lCntStraight++;
			}
			else
			{	
				lTypeStraight = 1;
				lCntStraight = 1;
				dSumStraight = pStrategyResultInArray->m_dProfit;
			}
			if(lTypeStraight==1&&m_lCntProfitStraight<lCntStraight)	// Change..
			{
				m_lCntProfitStraight = lCntStraight;
				m_lSumProfitStraight = (long)dSumStraight;
			}
		}
		else if(pStrategyResultInArray->m_dProfit<0)
		{
			m_dSumLoss += pStrategyResultInArray->m_dProfit;
			m_lSumCandleLoss += pStrategyResultInArray->m_lCntCandleInAccess;
			if(m_dMaxLoss>pStrategyResultInArray->m_dProfit)
			{
				m_dMaxLoss = pStrategyResultInArray->m_dProfit;
			}
			m_lCntLoss++;			
			if(lTypeStraight==2)	// Keeping straight...
			{
				dSumStraight += pStrategyResultInArray->m_dProfit;
				lCntStraight++;
			}
			else
			{
				lTypeStraight = 2;
				lCntStraight = 1;
				dSumStraight = pStrategyResultInArray->m_dProfit;
			}
			if(lTypeStraight==2&&m_lCntLossStraight<lCntStraight)	// Change..
			{
				m_lSumLossStraight = (long)dSumStraight;
				m_lCntLossStraight = lCntStraight;
			}
		}
		if(pStrategyResultInArray->m_DealType == CStrategyResult::EXITLONG
			||pStrategyResultInArray->m_DealType == CStrategyResult::EXITSHORT)
		{
			m_dSumAmountExit += pStrategyResultInArray->m_dAmount;
		}		
	}
	return TRUE;
}

//KHD : ����û�� ���� Ȯ�� 
BOOL CStrategyWnd::IsForcedLiquidation(long lDealValue)
{
	switch(lDealValue)
	{
	case VALSONJUL:		
	case VALSOOIK:		
	case VALGOGA:		
	case VALJUGA:		
	case VALMIN:			
	case VALDANGIL:
	case VALTARGETPRESERV:
	case VALBENEFITPRESERV:
		return TRUE;
	}
	return FALSE;
}

long CStrategyWnd::GetCntStrategyResult()
{
	return m_arrayStrategyResult.GetSize();
}

CStrategyResult* CStrategyWnd::GetStrategyResult(long lIndex)
{
	return m_arrayStrategyResult.GetAt(lIndex);
}

LPCTSTR CStrategyWnd::GetErrorMessage()
{
	return m_strError;
}


double CStrategyWnd::GetStatisticalInfo(long lPos)
{

	CStrategyResult *pStrategyResult = NULL;
	long lSize = m_arrayStrategyResult.GetSize();
	if(!lSize) return 0;
	switch(lPos)
	{
	case PROFITRATEACCMULATED://�������ͷ�
		{
			for(long lIndex=lSize-1;lIndex>=0;lIndex--)
			{
				pStrategyResult = m_arrayStrategyResult.GetAt(lIndex);
				if(pStrategyResult->m_dProfitRateAccmulated!=0)
				{
					return pStrategyResult->m_dProfitRateAccmulated;
				}
			}
			return 0.;			
		}
		break;
	case PROFITRATEAVG://�Ǻ� ��� ���ͷ� 
		{	
			double Accumulate = 0;
			for(long lIndex =0;lIndex<lSize;lIndex++)
			{
				Accumulate+= m_arrayStrategyResult.GetAt(lIndex)->m_dProfitRate;
			}
			return Accumulate/(m_lCntProfit + m_lCntLoss);//�������ͷ� ���� ����:KHD
		}
		break;
	case PROFITRATECURRENT:
		{
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			return pStrategyResult->m_dProfitRate;//pStrategyResult->m_dProfitRateAccmulated; : ����KHD: �Ǻ����ͷ������������� ����
		}
		break;
	case PROFITLOSSALL:
		{
			return (m_dSumProfit+m_dSumLoss);
		}
		break;
	case PROFITALL:
		{
			return m_dSumProfit;
		}
		break;
	case LOSSALL:
		{
			return m_dSumLoss;
		}
		break;
	case PROFITLOSSRATEALL:
		{
			if(m_dSumLoss==0)
			{
				return (m_dSumProfit)*-1;
			}
			return (m_dSumProfit/m_dSumLoss)*-1;
		}
		break;
	case MAXPROFIT:
		{
			return m_dMaxProfit;
		}
		break;
	case MAXLOSS:
		{
			return m_dMaxLoss;
		}
		break;
	case PROFITLOSSAVG:
		{
			if((m_lCntProfit+m_lCntLoss)==0)
			{
				return (m_dSumProfit+m_dSumLoss);
			}
			return (m_dSumProfit+m_dSumLoss)/(m_lCntProfit+m_lCntLoss);
		}
		break;
	case PROFITAVG:
		{
			if(m_lCntProfit==0)
			{
				return m_dSumProfit;
			}
			return (m_dSumProfit)/(m_lCntProfit);
		}
		break;
	case LOSSAVG:
		{
			if(m_lCntLoss==0)
			{	
				return m_dSumLoss;
			}
			return (m_dSumLoss)/(m_lCntLoss);
		}
		break;
	case AMOUNTENTEREDALL:
		{
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			return pStrategyResult->m_dEnteredAmount;
		}
		break;
	case AMOUNTEXITALL:
		{
			return m_dSumAmountExit;
		}
		break;
	case CNTALL:
		{
			return (m_lCntProfit + m_lCntLoss);
		}
		break;
	case CNTPROFIT:
		{
			return m_lCntProfit;
		}
		break;
	case CNTLOSS:
		{
			return m_lCntLoss;
		}
		break;
	case RATEPROFITLOSS:
		{
			if(m_lCntLoss==0)
			{
				return m_lCntProfit;
			}
			return (double)(m_lCntProfit/(double)m_lCntLoss);
		}
		break;
	case RATEPROFIT:
		{
			if((m_lCntProfit + m_lCntLoss)==0)
			{
				return m_lCntProfit;
			}
			return (double)(m_lCntProfit*100)/(double)(m_lCntProfit + m_lCntLoss);
		}
		break;
	case RATELOSS:
		{
			if((m_lCntProfit + m_lCntLoss)==0)
			{
				return m_lCntLoss;
			}
			return (double)(m_lCntLoss*100)/(double)(m_lCntProfit + m_lCntLoss);
		}
		break;
	case CHANGERATE:
		{
			CStrategyResult *pStrategyResultFirst = NULL;
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			pStrategyResultFirst = m_arrayStrategyResult.GetAt(0);
			if(pStrategyResultFirst->m_dPrice==0)//����: KHD : (ù ���� - ������ ���� / ù ����)�Ⱓ�����
			{
				return ((pStrategyResult->m_dPrice - pStrategyResultFirst->m_dPrice)/1)*100;
			}
			return ((pStrategyResult->m_dPrice - pStrategyResultFirst->m_dPrice)/pStrategyResultFirst->m_dPrice)*100;
		}
		break;
	case MAXPROFITSTRAIGHT:
		{
			return m_lCntProfitStraight;
		}
		break;
	case MAXLOSSSTRAIGHT:
		{
			return m_lCntLossStraight;
		}
		break;
	case RATEPROFITEXCEED:///���� �ʰ� ���ͷ� : �������ͷ�(0) - �Ⱓ�����(21)
		{
			CStrategyResult *pStrategyResultFirst = NULL;
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			pStrategyResultFirst = m_arrayStrategyResult.GetAt(0);
			if(pStrategyResultFirst->m_dPrice==0)
			{
				return (pStrategyResult->m_dPrice-pStrategyResultFirst->m_dPrice);
			}
			return (pStrategyResult->m_dPrice-pStrategyResultFirst->m_dPrice)/pStrategyResultFirst->m_dPrice;
		}
		break;
	case PROFITSTRAIGHT:
		{
			return m_lSumProfitStraight;
		}
		break;
	case LOSSSTRAIGHT:
		{
			return m_lSumLossStraight;
		}
		break;
	case CNTCANDLE:
		{
			return m_lAllCandleInCalculation;
		}
		break;
	case CNTCANDLEPROFIT:
		{
			return m_lSumCandleProfit;	
		}
		break;
	case CNTCANDLELOSS:
		{
			return m_lSumCandleLoss;
		}
		break;
	case RATEACCESS:
		{
			return (double)(m_lSumCandleProfit+m_lSumCandleLoss)*100/(double)m_lAllCandleInCalculation;
		}
		break;
	case CNTCANDLEACCESS:
		{
			return (m_lSumCandleProfit+m_lSumCandleLoss);
		}
		break;
	case CNTCANDLEEXIT:
		{
			return (m_lAllCandleInCalculation-(m_lSumCandleProfit+m_lSumCandleLoss));
		}
		break;
	case CNTCANDLEAVGDEAL:
		{
			if((m_lCntProfit+m_lCntLoss)==0)
			{
				return (m_lSumCandleProfit+m_lSumCandleLoss);
			}
			double value =(m_lSumCandleProfit+m_lSumCandleLoss)/(double)(m_lCntProfit+m_lCntLoss);
			return value;
		}
		break;
	case CNTCANDLEAVGPROFIT:
		{
			if(m_lCntProfit==0)
			{
				return m_lSumCandleProfit;
			}
			double value = m_lSumCandleProfit/(double)m_lCntProfit; 
			return value;
		}
		break;
	case CNTCANDLEAVGLOSS:
		{
			if(m_lCntLoss==0)
			{
				return m_lSumCandleLoss;
			}
			double value = m_lSumCandleLoss/(double)m_lCntLoss;
			return value;
		}
		break;

	}
	return 0;
}


void CStrategyWnd::GetCurrentPositonNProfit(long& rLCurrentDealPosition, double &rDCurrentProfitRate)
{
	rLCurrentDealPosition = m_lCurrentDealPosition;
	rDCurrentProfitRate = m_dCurrentProfitRate;
}

CString CStrategyWnd::GetProfileInfo(LPCTSTR lpAppName,LPCTSTR lpKeyName)
{
	CString strProfileInfo;
	if(m_strategyType==ST_ST)
	{
		CString strProfilePath = m_strStrategyPath;
		char szBuffer[256];
		::GetPrivateProfileString(lpAppName,lpKeyName,"",szBuffer,sizeof(szBuffer),strProfilePath);
		strProfileInfo = szBuffer;
	}
	return strProfileInfo;
}

BOOL CStrategyWnd::SetUpperLowerPrice(double dUpperPrice,double dLowerPrice)
{
	m_dUpperPrice = dUpperPrice;
	m_dLowerPrice = dLowerPrice;
	return TRUE;
}

BOOL CStrategyWnd::SetVolumeForCalculatation(double dVol)
{
	m_dVolForOrder = dVol;
	return TRUE;
}

double CStrategyWnd::GetVolumeInDealData()
{
	POSITION pos = m_dealDataItem.GetTailPosition();
	if(pos)
	{
		CDealData* pDealData = m_dealDataItem.GetPrev(pos);
		if(pDealData->m_lDealValue&&VALBUY||pDealData->m_lDealValue&&VALSELL)
		{
			return pDealData->m_dContract;
		}
		else// ExitLong, ExitShort
		{
			if(pDealData->m_lType&DEALEXITDEFINED)
			{
				return pDealData->m_dContract;
			}
			else
			{
				double dSumOfContract = 0.;
				long lValueToKeep = 0;
				while(pos)
				{
					CDealData* pDealData = m_dealDataItem.GetPrev(pos);
					if(!lValueToKeep)
					{
						if(pDealData->m_lDealValue&&VALBUY)
						{
							lValueToKeep = VALBUY;
							dSumOfContract = pDealData->m_dContract;
						}
						else if(pDealData->m_lDealValue&&VALSELL)
						{
							lValueToKeep = VALSELL;
							dSumOfContract = pDealData->m_dContract;
						}
						else
						{
							break;
						}
					}
					else
					{
						if(pDealData->m_lDealValue&&VALSELL&&lValueToKeep==VALSELL)
						{
							dSumOfContract += pDealData->m_dContract;
						}
						else if(pDealData->m_lDealValue&&VALBUY&&lValueToKeep==VALBUY)
						{
							dSumOfContract += pDealData->m_dContract;
						}
						else
						{
							break;
						}
					}
				}
				return dSumOfContract;
			}			
		}

	}
	return 0;
}

void CStrategyWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==m_nTimerID)
	{
		ExitAtTodayByTimeSelected(FALSE);
	}
	CWnd::OnTimer(nIDEvent);
}


void CStrategyWnd::ExitAtTodayByTimeSelected(BOOL bStartTimer/*=TRUE*/)
{	
	if(m_bUseDayLiquidation)
	{
		CTime t = CTime::GetCurrentTime();
		long lCurrentTime = atol(t.Format("%H%M"));
		if(lCurrentTime >= atol(m_szTimeDayLiquidation))
		{
			OnSTSetRealDataST();
			GetParent()->SendMessage(RMSG_STDATAMANAGER_RESULTDATA, m_lKey, (LPARAM)&m_DrawInfo);
			if(m_nTimerID) KillTimer(m_nTimerID);
		}		
		if(bStartTimer)
			m_nTimerID = SetTimer(ID_FORCEDEXITBYTIME, 60000, NULL);
	}
	else
	{
		if(m_nTimerID) KillTimer(m_nTimerID);
	}
}

void CStrategyWnd::InitConfiguration()
{
	m_dFeeForEntry = 0.; 
	m_dFeeForExit = 0.;
	m_lFeeUnit = 0;
	m_dSlippageForEntry = 0.;
	m_dSlippageForExit = 0.;
	m_lSlippageUnit = 0;

	if(m_pFeeCal)
	{
		delete m_pFeeCal;
		m_pFeeCal = NULL;
	}
	if(m_pSlippageCal)
	{
		delete m_pSlippageCal;
		m_pSlippageCal = NULL;
	}
}

HWND CStrategyWnd::GetSafeHwnd( ) const
{
	return CWnd::GetSafeHwnd();
}

//�ð�����ȭ���� ���� ������ �������� Set�ؾ� �Ѵ�.

void CStrategyWnd::ReadConfiguration(CString szPath)
{
//-------- ��ȣ �������� -------- 
	ReadConfig_Signal(szPath);
//-------- ����û�� �������� --------
	ReadConfig_ForcedLiquidation(szPath);
//-------- ��� �������� --------
	ReadConfig_Setup(szPath);
//-------- �ֹ� �������� --------
	ReadConfig_Order(szPath);
}

void CStrategyWnd::ReadConfig_Signal(CString szPath)
{
	char		szBuffer[256];
	CString		strComment;
	long		lShapeSignal;
	COLORREF	clrSignal;

	// Buy Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPEBUY, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPEBUY, strComment);

	// Buy Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLORBUY, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALBUYDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLORBUY, strComment);

	// Sell Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPESELL, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPESELL, strComment);

	// Sell Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLORSELL, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALSELLDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLORSELL, strComment);

	// ExitShort Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPEEXITSHORT, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPEEXITSHORT, strComment);

	// ExitShort Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLOREXITSHORT, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALEXITSHORTDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLOREXITSHORT, strComment);

	// ExitLong Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPEEXITLONG, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPEEXITLONG, strComment);

	// ExitLong Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLOREXITLONG, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALEXITLONGDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLOREXITLONG, strComment);

	// Signal Name
	::GetPrivateProfileString(STCONFIG_SIGNAL, SHOWSIGNALNAME, "0", szBuffer, sizeof(szBuffer), szPath);
	strComment.Format("%d,", atol(szBuffer));
	m_dealDataItem.SetComment(SHOWSIGNALNAME, strComment);

	// Signal Qty
	::GetPrivateProfileString(STCONFIG_SIGNAL, SHOWSIGNALQTY, "0", szBuffer, sizeof(szBuffer), szPath);
	strComment.Format("%d,", atol(szBuffer));
	m_dealDataItem.SetComment(SHOWSIGNALQTY, strComment);
}

void CStrategyWnd::ReadConfig_ForcedLiquidation(CString szPath)
{
	CString szTmp;
	COLORREF clrSignal = RGB(0,0,0);
	szTmp.GetBufferSetLength(MAX_PATH);

	// ����û����� (0 : ���� ������ / 1 : ���ϼ���)
	m_nTiming_ForcedLiquidation = ::GetPrivateProfileInt(STCONFIG_CLEAR, CLEARPOINT, 0, szPath);
	
	// ������(�սǷ�)
	GetPrivateProfileString(STCONFIG_CLEAR, USELOSSRATE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseLossRate = atoi(szTmp);

	// �����ſɼ� ����� ���
	if(IsLossRate())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, LOSSRATE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or ��
		m_uLossRate_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, LOSSRATEUNIT, 0, szPath);
		if(m_uLossRate_PerOrWon == 0)
			m_dLossRate = atof(szTmp) * 0.01;	// %
		else
//			m_dLossRate = atoi(szTmp);			// ��
			m_dLossRate = atof(szTmp);			// ��,Point
	}

	// ��ǥ����	
	GetPrivateProfileString(STCONFIG_CLEAR, USETARGETBENEFIT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseTargetBenefit = atoi(szTmp);

	// ��ǥ���Ϳɼ� ����� ���
	if(IsTargetBenefit())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, TARGETBENEFIT, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or ��
		m_uTargetBenefit_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, TARGETBENEFITUNIT, 0, szPath);
		if(m_uTargetBenefit_PerOrWon == 0)
			m_dTargetBenefit = atof(szTmp) * 0.01;	// %
		else
//			m_dTargetBenefit = atoi(szTmp);			// ��
			m_dTargetBenefit = atof(szTmp);			// ��,Point
	}

	// �ְ���� �϶�
	GetPrivateProfileString(STCONFIG_CLEAR, USEHIGHPRICE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseHPriceCPBear = atoi(szTmp);

	// �ְ���� �϶��ɼ� ����� ���
	if(IsHPriceCPBear())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, HIGHPRICE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or ��
		m_uHPriceCPBear_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, HIGHPRICEUNIT, 0, szPath);
		if(m_uHPriceCPBear_PerOrWon == 0)
			m_dHPriceCPBear = atof(szTmp) * 0.01;		// %
		else
//			m_dHPriceCPBear = atoi(szTmp);				// ��
			m_dHPriceCPBear = atof(szTmp);				// ��,Point
	}

//	// ���� ������� ���
//	GetPrivateProfileString(STCONFIG_CLEAR, USELOWPRICE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
//	m_bUseLPriceCPBull = atoi(szTmp);
//
//	// ���� ������� ��¿ɼ� ����� ���
//	if(IsLPriceCPBull())
//	{
//		GetPrivateProfileString(STCONFIG_CLEAR, LOWPRICE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
//
//		// % or ��
//		m_uLPriceCPBull_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, LOWPRICEUNIT, 0, szPath);
//		if(m_uLPriceCPBull_PerOrWon == 0)
//			m_dLPriceCPBull = atof(szTmp) * 0.01;		// %
//		else
//			m_dLPriceCPBull = atoi(szTmp);				// ��
//	}

	// ��ǥ������
	GetPrivateProfileString(STCONFIG_CLEAR, USETARGETVALUEPRESERV, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseTargetValuePreserve = atoi(szTmp);

	// ��ǥ�������ɼ� ����� ���
	if(IsTargetValuePreserve())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, TARGETVALUEPRESERV, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or ��
		m_uTargetValuePreserve_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, TARGETVALUEPRESERVUNIT, 0, szPath);
		if(m_uTargetValuePreserve_PerOrWon == 0)
			m_dTargetValuePreserve = atof(szTmp) * 0.01;		// %
		else
//			m_dTargetValuePreserve = atoi(szTmp);				// ��
			m_dTargetValuePreserve = atof(szTmp);				// ��,Point
	}

	// ���ͺ���
	GetPrivateProfileString(STCONFIG_CLEAR, USEBENEFITPRESERV, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseBenefitPreserve = atoi(szTmp);

	// ���ͺ����ɼ� ����� ���
	if(IsBenefitPreserve())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, BENEFITPRESERVUP, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// ���ͺ���(�����)% or ��
		m_uBenefitPreserveUp_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, BENEFITPRESERVUPUNIT, 0, szPath);
		if(m_uBenefitPreserveUp_PerOrWon == 0)
			m_dBenefitPreserveUp = atof(szTmp) * 0.01;		// %
		else
//			m_dBenefitPreserveUp = atoi(szTmp);				// ��
			m_dBenefitPreserveUp = atof(szTmp);				// ��,Point

		GetPrivateProfileString(STCONFIG_CLEAR, BENEFITPRESERVDN, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// ���ͺ���(�϶���)% or ��
		m_uBenefitPreserveDn_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, BENEFITPRESERVDNUNIT, 0, szPath);
		if(m_uBenefitPreserveDn_PerOrWon == 0)
			m_dBenefitPreserveDn = atof(szTmp) * 0.01;		// %
		else
//			m_dBenefitPreserveDn = atoi(szTmp);				// ��
			m_dBenefitPreserveDn = atof(szTmp);				// ��,Point
	}

	// �ּҰ��ݺ�ȭ
	GetPrivateProfileString(STCONFIG_CLEAR, USEMINIPRICE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseMiniPrice = atoi(szTmp);

	// �ּҰ��ݺ�ȭ�ɼ� ����� ���
	if(IsMiniPrice())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, MINIPRICE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or ��
		m_uMiniPrice_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, MINIPRICEUNIT, 0, szPath);
		if(m_uMiniPrice_PerOrWon == 0)
			m_dMiniPrice = atof(szTmp) * 0.01;		// %
		else
//			m_dMiniPrice = atoi(szTmp);				// ��
			m_dMiniPrice = atof(szTmp);				// ��,Point

		// ��
		GetPrivateProfileString(STCONFIG_CLEAR, MINIPRICEBONGCOUNT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
		m_nBongCount = atoi(szTmp);
	}

	// ����û��
	GetPrivateProfileString(STCONFIG_CLEAR, USETODAYCLEAR, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseDayLiquidation = atoi(szTmp);

	// ����û��ɼ� ����� ���
	if(IsDayLiquidation())
	{
		// 0 : ���� / 1 : ����
		m_uDayLiquidation_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, TODAYCLEARUINT, 0, szPath);

		// �ð�
		GetPrivateProfileString(STCONFIG_CLEAR, TODAYCLEARTIME, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
		m_szTimeDayLiquidation = szTmp;
		m_szTimeDayLiquidation.Remove(':');

		// ������ ���
		if(m_uDayLiquidation_PerOrWon > 0)
			m_szTimeDayLiquidation.Format("%ld", atol(m_szTimeDayLiquidation) + 1200);
	}

	szTmp.ReleaseBuffer();
}

void CStrategyWnd::ReadConfig_Setup(CString szPath)
{
	CString szTmp;
	szTmp.GetBufferSetLength(MAX_PATH);

	// �ߺ����� ����(0 : �ߺ����� ���, 1 : �ߺ����� ������)
	m_nUseRepeatEntry = GetPrivateProfileInt(STCONFIG_SETUP, USEREPEATBUY, 1, szPath) ? FALSE : TRUE;

	// �ߺ����� ���Ƚ��
	m_nRepeatEntryCount = GetPrivateProfileInt(STCONFIG_SETUP, REPEATBUYCOUNT, 0, szPath);
	if(m_nRepeatEntryCount < 2)
		m_nRepeatEntryCount = 1;

	// �ŷ���� ����
	::GetPrivateProfileString(STCONFIG_SETUP, TAXBUY, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dFeeForEntry = atof(szTmp);		// ������ + ���� (����)

	::GetPrivateProfileString(STCONFIG_SETUP, TAXSELL, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dFeeForExit = atof(szTmp);		// ������ + ���� (û��)

	::GetPrivateProfileString(STCONFIG_SETUP, TAXUNIT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_lFeeUnit = atol(szTmp);			// ������ + ���� (����)

	::GetPrivateProfileString(STCONFIG_SETUP, SLIPIGYBUY, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dSlippageForEntry = atof(szTmp);	// �������� (����)

	::GetPrivateProfileString(STCONFIG_SETUP, SLIPIGYSELL, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dSlippageForExit = atof(szTmp);	// �������� (û��)

	::GetPrivateProfileString(STCONFIG_SETUP, SLIPIGYUNIT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_lSlippageUnit = atol(szTmp);		// �������� (����)

	szTmp.ReleaseBuffer();

	// ������ + ����
	if(m_pFeeCal)
	{
		delete m_pFeeCal;
		m_pFeeCal = NULL;
	}
	if(m_lFeeUnit == 0)				// Percent
		m_pFeeCal = new CFeeCalByPercent(m_dFeeForEntry, m_dFeeForExit);
	else if(m_lFeeUnit == 1)		// Pt(��)
		m_pFeeCal = new CFeeCalByPrice(m_dFeeForEntry, m_dFeeForExit);

	// ��������
	if(m_pSlippageCal)
	{
		delete m_pSlippageCal;
		m_pSlippageCal = NULL;
	}
	if(m_lSlippageUnit == 0)		// Percent
		m_pSlippageCal = new CFeeCalByPercent(m_dSlippageForEntry, m_dSlippageForExit);
	else if(m_lSlippageUnit == 1)	// Pt(��)
		m_pSlippageCal = new CFeeCalByPrice(m_dSlippageForEntry, m_dSlippageForExit);
}

void CStrategyWnd::ReadConfig_Order(CString szPath)
{
	CString szTmp;
	szTmp.GetBufferSetLength(MAX_PATH);

	// �ֹ�����
	m_nOrderAmount = ::GetPrivateProfileInt(STCONFIG_ORDER, ORDERAMOUNT, 1, szPath);
	if(m_nOrderAmount <= 0)
		m_nOrderAmount = 1;

	// �ֹ����� (0 : ���� ������ / 1 : ���ϼ���)
	m_nTiming_Order = ::GetPrivateProfileInt(STCONFIG_ORDER, ORDERTIME, 0, szPath);

	// �ֹ����۽�ȣ (0 : ���Խ�ȣ / 1 : ����ȣ)
	m_nOrderStartSignal = ::GetPrivateProfileInt(STCONFIG_ORDER, ORDERSTARTSIGNAL, 0, szPath);

	szTmp.ReleaseBuffer();
}

void CStrategyWnd::InitStrategySignalInfo()
{
	int nTotalDataCnt = m_pDateItem->GetSize();
	m_arrayDealPosition.RemoveAll();
	m_arrayDealName.RemoveAll();
	m_arrayDealDrawPosition.RemoveAll();
	m_arrayDealPosition.SetSize(nTotalDataCnt);
	m_arrayDealName.SetSize(nTotalDataCnt);
	m_arrayDealDrawPosition.SetSize(nTotalDataCnt);
}

BOOL CStrategyWnd::IsFirstLiquidation(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation)
{
	int nEntryPosition = pEntryDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	BOOL bRet = FALSE;

	// û���ȣ
	if(IsSellOrExitLong(pEntryDealData->m_lDealValue))
	{
		// ��ó���� ������ û���ȣ
		if(m_bIsFirstSignal)
		{
			// �ֹ����۽�ȣ -> ���Խ�ȣ�� ������ ���
			if(m_nOrderStartSignal == 0);
			// �ֹ����۽�ȣ -> ����ȣ�� ������ ���
			else if(m_nOrderStartSignal == 1)
			{
				// ��ȣ����� ���ʿ� ���̹Ƿ� ���� ����
				m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
				m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nEntryPosition, pHigh->GetAt(nEntryPosition));

				// �������� ó�� �̿��� ��ȣ�� �ǹ�
				m_bIsFirstSignal = FALSE;
			}
		}
		// ��ó���� ������ û���ȣ (�ߺ����� ������� ���� ��ȣ�� �����̾��� ���)
		// ���� ��ȣ�� ������ �ƴ� ���� �����ϰ� ���� ��ȣ�� ��ȸ��
		else if(m_bCheckPrev_EntryPosition)
		{
			// ��ȣ����� ���ʿ� ���̹Ƿ� ���� ����
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pHigh->GetAt(nEntryPosition));
		}

		// ���� ��ȣ�� û���ȣ�� ����
		m_bCheckPrev_EntryPosition = FALSE;

		// �ߺ����� Ƚ������ �ʱ�ȭ
		m_nRepeatCount = 0;

		// û���ȣ�̹Ƿ� TRUE ����
		bRet = TRUE;
	}

	return bRet;
}

BOOL CStrategyWnd::IsLastEntry(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation, POSITION& posLast)
{
	int nEntryPosition = pEntryDealData->m_nIndex;
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	if(posLast == NULL && IsBuyOrExitShort(pEntryDealData->m_lDealValue))
	{
		// ���� ��ȣ�� ������ ���
		if(m_bCheckPrev_EntryPosition)
		{
			// �ߺ����� ����� ���
			if(m_nUseRepeatEntry && m_nRepeatEntryCount > m_nRepeatCount++)
			{
				// ��ȣ����� �Ʒ��ʿ� ���̹Ƿ� ������ ����
				m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);		
				m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));
			}
		}
		// ���� ��ȣ�� û���� ���
		else
		{
			// ��ȣ����� �Ʒ��ʿ� ���̹Ƿ� ������ ����
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);		
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));

			m_bCheckPrev_EntryPosition = FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CStrategyWnd::IsLastEntry_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	int nExitPosition = pExitDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	if(IsBuyOrExitShort(pExitDealData->m_lDealValue))
	{
		// �� �ϼ��ø� ���� ���
		m_TempDealData = *pExitDealData;
		
		// �ֹ����� (0 : ���� ������)
		if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
		{
			// ��ȣ����� �Ʒ��ʿ� ���̹Ƿ� ������ ����
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);		
			m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nExitPosition, pLow->GetAt(nExitPosition));

			// ���� �ֹ������� ���õ��� ���� ���
			if(m_bSetComplete_OrderInfo == FALSE)
			{
				m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
										m_TempDealData.m_lDealValue,
										m_TempDealData.m_lType,
										m_TempDealData.m_strDealName,
										m_TempDealData.m_dPrice,
										m_TempDealData.m_dContract,
										&m_TempDealData.m_strListEntryNameForExit);

				m_TempCondDealData = m_TempDealData;

				// �ֹ����� ���ÿϷ�
				m_bSetComplete_OrderInfo = TRUE;

				// �ߺ����� Ƚ������
				m_nRepeatCount++;
//				FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//				if(fp != NULL) 
//				{
//					fprintf(fp, "++[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "IsLastEntry_ForReal");
//					fclose(fp);
//				}
			}
		}

		return TRUE;
	}
	// DealValue�� 0���� ��ȣó��
	else if(pExitDealData->m_lDealValue == 0)
	{
		// �� �ϼ��ø� ���� ���
		m_TempDealData = *pExitDealData;

		// �ֹ������� ���� �������� ���
		if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
		{
			// ��ȣ ���� ���
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
			m_arrayDealName.SetAt(nExitPosition, "");
			m_arrayDealDrawPosition.SetAt(nExitPosition, 0);

			m_TempDealData.Clear();
		}
	}

	return FALSE;
}

void CStrategyWnd::Confirm_ForcedLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	//-------- �ż� / �ŵ�û��-------- 
	BuyOrExitShort(pEntryDealData, bExist_ForcedLiquidation);

	CForcedLiquidationInfo FLInfo;
	//-------- ����û�� �ɼǺ� --------
	// ������(�սǷ�) �ɼ� ��
	if(IsLossRate())
		ForcedLiquidation_ForLossRate(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// ��ǥ���� �ɼ� ��
	if(IsTargetBenefit())
		ForcedLiquidation_ForTargetBenefit(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// �ְ���� �϶� �ɼ� ��
	if(IsHPriceCPBear())
		ForcedLiquidation_ForHPriceCPBear(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

//	// ���� ������� ��� �ɼ� ��
//	if(IsLPriceCPBull())
//		ForcedLiquidation_ForLPriceCPBull(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// ��ǥ������ �ɼ� ��
	if(IsTargetValuePreserve())
		ForcedLiquidation_ForTargetValuePreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// ���ͺ��� �ɼ� ��
	if(IsBenefitPreserve())
		ForcedLiquidation_ForBenefitPreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// �ּҰ��ݺ�ȭ �ɼ� ��
	if(IsMiniPrice())
		ForcedLiquidation_ForMiniPrice(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// ����û�� �ɼ� ��
	if(IsDayLiquidation())
		ForcedLiquidation_ForDayLiquidation(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// ����û�� ���� (�߻��� ����û�� �� ���� �߻��� ��ġ�� ����)
	if(bExist_ForcedLiquidation)
		ForcedLiquidation(pExitDealData, FLInfo);

	//-------- �ŵ� / �ż�û�� -------- 
	SellOrExitLong(pExitDealData, bExist_ForcedLiquidation);
}

void CStrategyWnd::Confirm_ForcedLiquidation_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	CForcedLiquidationInfo FLInfo;
	//-------- ����û�� �ɼǺ� --------
	// ������(�սǷ�) �ɼ� ��
	if(IsLossRate())
		ForcedLiquidation_ForLossRate(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// ��ǥ���� �ɼ� ��
	if(IsTargetBenefit())
		ForcedLiquidation_ForTargetBenefit(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// �ְ���� �϶� �ɼ� ��
	if(IsHPriceCPBear())
		ForcedLiquidation_ForHPriceCPBear(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	//	// ���� ������� ��� �ɼ� ��
	//	if(IsLPriceCPBull())
	//		ForcedLiquidation_ForLPriceCPBull(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// ��ǥ������ �ɼ� ��
	if(IsTargetValuePreserve())
		ForcedLiquidation_ForTargetValuePreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// ���ͺ��� �ɼ� ��
	if(IsBenefitPreserve())
		ForcedLiquidation_ForBenefitPreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// �ּҰ��ݺ�ȭ �ɼ� ��
	if(IsMiniPrice())
		ForcedLiquidation_ForMiniPrice(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// ����û�� �ɼ� ��
	if(IsDayLiquidation())
		ForcedLiquidation_ForDayLiquidation(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// ����û�� ���� (�߻��� ����û�� �� ���� �߻��� ��ġ�� ����)
	if(bExist_ForcedLiquidation)
		ForcedLiquidation_ForReal(pExitDealData, FLInfo);

	//-------- �ŵ� / �ż�û�� -------- 
	SellOrExitLong_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
}

// �ż� / �ŵ�û��
void CStrategyWnd::BuyOrExitShort(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation)
{
	int nEntryPosition = pEntryDealData->m_nIndex;
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	// ������ ���Խ�ȣ�� ����
	// �ߺ����� ����� ��� 
	if(m_nUseRepeatEntry)
	{
		// �ߺ����� ��밹����ŭ ����ǥ�� ����
		if(m_nRepeatEntryCount > m_nRepeatCount++)
		{
			// ��ȣ����� �Ʒ��ʿ� ���̹Ƿ� ������ ����
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));

			// �������� ó�� �̿��� ��ȣ�� �ǹ�
			m_bIsFirstSignal = FALSE;
		}
	}
	// �ߺ����� ������� ���� ���
	else
	{
		// ó������ ������ ���Խ�ȣ�� ���
		if(m_bIsFirstSignal)
		{
			// ��ȣ����� �Ʒ��ʿ� ���̹Ƿ� ������ ����
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));

			// �������� ó�� �̿��� ��ȣ�� �ǹ�
			m_bIsFirstSignal = FALSE;
		}
		// ���� ��ȣ�� û���� ���
		else if(!m_bCheckPrev_EntryPosition)
		{
			// ��ȣ����� �Ʒ��ʿ� ���̹Ƿ� ������ ����
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));
		}
	}

	// ���� ��ȣ�� ���Խ�ȣ�� ����
	m_bCheckPrev_EntryPosition = TRUE;

	// ����û�� ���� �ʱ�ȭ
	bExist_ForcedLiquidation = FALSE;
}

// �ŵ� / �ż�û��
void CStrategyWnd::SellOrExitLong(CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	int nExitPosition = pExitDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	// �Ϲ� û��
	if(!bExist_ForcedLiquidation && IsSellOrExitLong(pExitDealData->m_lDealValue))
	{
		// ��ȣ����� ���ʿ� ���̹Ƿ� ���� ����
		m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
		m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
		m_arrayDealDrawPosition.SetAt(nExitPosition, pHigh->GetAt(nExitPosition));

		// ���� ��ȣ�� û���ȣ�� ����
		m_bCheckPrev_EntryPosition = FALSE;		
		
		// �ߺ����� Ƚ������ �ʱ�ȭ
		m_nRepeatCount = 0;
	}
	// �ߺ����� ����Ȯ��
	else if(!bExist_ForcedLiquidation && IsBuyOrExitShort(pExitDealData->m_lDealValue))
	{
		// �ߺ����� ����� ��� ��밹����ŭ ����ǥ�� ����
		if(m_nUseRepeatEntry && m_nRepeatEntryCount > m_nRepeatCount++)
		{
			// ��ȣ����� ���ʿ� ���̹Ƿ� ���� ����
			m_arrayDealDrawPosition.SetAt(nExitPosition, pLow->GetAt(nExitPosition));
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
			m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
		}
	}
}

// �ŵ� / �ż�û��
void CStrategyWnd::SellOrExitLong_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	int nExitPosition = pExitDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	// �Ϲ� û��
	if(!bExist_ForcedLiquidation && IsSellOrExitLong(pExitDealData->m_lDealValue))
	{
		// �ߺ����� Ȯ�θ� (�ߺ����� ī��Ʈ�� �������� ����)
		if((m_nUseRepeatEntry && m_nRepeatEntryCount >= m_nRepeatCount) || m_nUseRepeatEntry == 0)
		{
			// �� �ϼ��ø� ���� ���
			m_TempDealData = *pExitDealData;
			
			// �ֹ������� ���� �������� ���
			if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
			{
				// ��ȣ����� ���ʿ� ���̹Ƿ� ���� ����
				m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
				m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nExitPosition, pHigh->GetAt(nExitPosition));
				
				// ���� �ֹ������� ���õ��� ���� ���
				if(m_bSetComplete_OrderInfo == FALSE)
				{
					m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
											m_TempDealData.m_lDealValue,
											m_TempDealData.m_lType,
											m_TempDealData.m_strDealName,
											m_TempDealData.m_dPrice,
											m_TempDealData.m_dContract,
											&m_TempDealData.m_strListEntryNameForExit);
					
					m_TempCondDealData = m_TempDealData;
					
					// �ֹ����� ���ÿϷ�
					m_bSetComplete_OrderInfo = TRUE;
					
					// �ߺ����� Ƚ������ �ʱ�ȭ
					m_nRepeatCount = 0;
//					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//					if(fp != NULL) 
//					{
//						fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "SellOrExitLong_ForReal");
//						fclose(fp);
//					}
				}
			}
		}
	}
	// �ߺ����� ����Ȯ��
	else if(!bExist_ForcedLiquidation && IsBuyOrExitShort(pExitDealData->m_lDealValue))
	{
		// �ߺ����� Ȯ�θ� (�ߺ����� ī��Ʈ�� �������� ����)
		if(m_nUseRepeatEntry && m_nRepeatEntryCount > m_nRepeatCount)
		{
			// �� �ϼ��ø� ���� ���
			m_TempDealData = *pExitDealData;

			// �ֹ������� ���� �������� ���
			if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
			{
				// 1ƽ�� ��� �ߺ���� 1ȸ�̻��� ���
				if(m_nRepeatEntryCount == m_nRepeatCount)
					return;

				// ��ȣ����� ���ʿ� ���̹Ƿ� ���� ����
				m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
				m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nExitPosition, pLow->GetAt(nExitPosition));

				// ���� �ֹ������� ���õ��� ���� ���
				if(m_bSetComplete_OrderInfo == FALSE)
				{
					m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
											m_TempDealData.m_lDealValue,
											m_TempDealData.m_lType,
											m_TempDealData.m_strDealName,
											m_TempDealData.m_dPrice,
											m_TempDealData.m_dContract,
											&m_TempDealData.m_strListEntryNameForExit);

					m_TempCondDealData = m_TempDealData;

					// �ֹ����� ���ÿϷ�
					m_bSetComplete_OrderInfo = TRUE;

					// �ߺ����� Ƚ������
					m_nRepeatCount++;
//					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//					if(fp != NULL) 
//					{
//						fprintf(fp, "++[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "SellOrExitLong_ForReal");
//						fclose(fp);
//					}
				}
			}
		}
	}
	// DealValue�� 0���� ��ȣó��
	else if(!bExist_ForcedLiquidation && !pExitDealData->m_lDealValue)
	{
		// �� �ϼ��ø� ���� ���
		m_TempDealData = *pExitDealData;

		// �ֹ������� ���� �������� ���
		if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
		{
			// ��ȣ ���� ���
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
			m_arrayDealName.SetAt(nExitPosition, "");
			m_arrayDealDrawPosition.SetAt(nExitPosition, 0);

			m_TempDealData.Clear();
		}
	}
}

// �ż� ��뿩��
BOOL CStrategyWnd::IsBuy(long lDealType)
{
	return lDealType & VALENTRYBUY;
}

// �ŵ�û�� ��뿩��
BOOL CStrategyWnd::IsExitShort(long lDealType)
{
	return lDealType & VALEXITSHORT;
}

// �ŵ� ��뿩��
BOOL CStrategyWnd::IsSell(long lDealType)
{
	return lDealType & VALENTRYSELL;
}

// �ż�û�� ��뿩��
BOOL CStrategyWnd::IsExitLong(long lDealType)
{
	return lDealType & VALEXITLONG;
}

// �ż� / �ŵ�û�� ��뿩��
BOOL CStrategyWnd::IsBuyOrExitShort(long lDealType)
{
	return IsBuy(lDealType) || IsExitShort(lDealType);
}

// �ŵ� / �ż�û�� ��뿩��
BOOL CStrategyWnd::IsSellOrExitLong(long lDealType)
{
	return IsSell(lDealType) || IsExitLong(lDealType);
}

// ������ ��뿩��
BOOL CStrategyWnd::IsLossRate()
{
	return m_bUseLossRate;
}

// ��ǥ���� ��뿩��
BOOL CStrategyWnd::IsTargetBenefit()
{
	return m_bUseTargetBenefit;
}

// �ְ�����϶� ��뿩��
BOOL CStrategyWnd::IsHPriceCPBear()
{
	return m_bUseHPriceCPBear;
}

//// ��������� ��뿩��
//BOOL CStrategyWnd::IsLPriceCPBull()
//{
//	return m_bUseLPriceCPBull;
//}

BOOL CStrategyWnd::IsTargetValuePreserve()
{
	return m_bUseTargetValuePreserve;
}

BOOL CStrategyWnd::IsBenefitPreserve()
{
	return m_bUseBenefitPreserve;
}

// �ּҰ��� ��뿩��
BOOL CStrategyWnd::IsMiniPrice()
{
	return m_bUseMiniPrice;
}

// ����û�� ��뿩��
BOOL CStrategyWnd::IsDayLiquidation()
{
//	// ���Ŀ� m_nDivideState�� ���
//	int nDivideState = 0;
//	if(m_pDateItem)
//		nDivideState = m_pDateItem->GetType();

	return (m_bUseDayLiquidation && (m_nDivideState == 4 && m_nUnitPos != 60));	// �к������� �����
}

// �����ŷ� ���� ����û��
void CStrategyWnd::ForcedLiquidation_ForLossRate(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// ���� ��ġ
	int nExitPosition = pExitDealData->m_nIndex;			// û�� ��ġ
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// ����

	double dBearValue = 0.;		// �϶��� ��
	// %
	if(m_uLossRate_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dLossRate); 	// ��������(%) �϶��� ���
	// ��
	else
		dBearValue = dClose - m_dLossRate;				// �����ݾ�(��) �϶��� ���

	// EntryPosition �� ExitPosition ���̿��� ������ �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare;
	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� �϶��� ���
		if(dValueForCompare <= dBearValue)
		{
			FLInfo.SetFLInfo(nIndx, VALSONJUL, pHigh);

			// ����û�� �߻�
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// ��ǥ�������� ���� ����û��
void CStrategyWnd::ForcedLiquidation_ForTargetBenefit(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// ���� ��ġ
	int nExitPosition = pExitDealData->m_nIndex;			// û�� ��ġ
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// ����

	double dBullValue = 0.;		// ����� ��
	// %
	if(m_uTargetBenefit_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetBenefit);	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dClose + m_dTargetBenefit;				// �����ݾ�(��) ����� ���

	// EntryPosition �� ExitPosition ���̿��� ��ǥ���� �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare;
	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		if(dValueForCompare >= dBullValue)
		{
			FLInfo.SetFLInfo(nIndx, VALSOOIK, pHigh);

			// ����û�� �߻�
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// ������϶����� ���� ����û��
void CStrategyWnd::ForcedLiquidation_ForHPriceCPBear(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;		// ���� ��ġ
	int nExitPosition = pExitDealData->m_nIndex;		// û�� ��ġ
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dHighValue = 0.;
	double dCloseValue = 0.;
	double dHighEach = 0.;
	double dCloseEach = 0.;

	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dHighEach = pHigh->GetAt(nIndx);

		// ���� ���� �� ���� ��� �ְ��� ����
		if(dHighValue - dHighEach < 0.)
			dHighValue = dHighEach;


		dCloseEach = pClose->GetAt(nIndx);
		double dBearValue = 0.;		// �϶��� ��
		// %
		if(m_uHPriceCPBear_PerOrWon == 0)
			dBearValue = dHighValue - (dHighValue * m_dHPriceCPBear);		// ��������(%) �϶��� ���
		// ��
		else
			dBearValue = dHighValue - m_dHPriceCPBear;	 					// �����ݾ�(��) �϶��� ���

		// �������� �Ǵ� �����ݾ����� ������ ���� ���ذ����� �϶��� ���
		if(dCloseEach <= dBearValue)
		{
			FLInfo.SetFLInfo(nIndx, VALGOGA, pHigh);

			// ����û�� �߻�
			bExist_ForcedLiquidation = TRUE;
			return;
		}
	}
}

//// ������������� ���� ����û��
//void CStrategyWnd::ForcedLiquidation_ForLPriceCPBull(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
//{
//	int nEntryPosition = pEntryDealData->m_nIndex;		// ���� ��ġ
//	int nExitPosition = pExitDealData->m_nIndex;		// û�� ��ġ
//	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
//	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);
//	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
//	double dLowValue = 0.;
//	double dCloseValue = 0.;
//	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
//	{
//		double dLowEach = pLow->GetAt(nIndx);
//		double dCloseEach = pClose->GetAt(nIndx);
//		// ���� ������ �� ���� ��� �������� ����
//		if(dLowValue - dLowEach > 0.)
//			dLowValue = dLowEach;
//		double dBullValue = 0.;		// ����� ��
//		// %
//		if(m_uLPriceCPBull_PerOrWon == 0)
//			dBullValue = dLowValue + (dLowValue * m_dLPriceCPBull);	// ��������(%) ����� ���
//		// ��
//		else
//			dBullValue = dLowValue + m_dLPriceCPBull; 				// �����ݾ�(��) ����� ���
//		// �������� �Ǵ� �����ݾ����� ������ ���� ���ذ����� ����� ���
//		if(dCloseEach >= dBullValue)
//		{
//			FLInfo.SetFLInfo(nIndx, VALJUGA, pHigh);
//			// ����û�� �߻�
//			bExist_ForcedLiquidation = TRUE;
//			break;
//		}
//	}
//}

// ��ǥ�������� ���� ����û��
void CStrategyWnd::ForcedLiquidation_ForTargetValuePreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// ���� ��ġ
	int nExitPosition = pExitDealData->m_nIndex;			// û�� ��ġ
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// ����

	double dBullValue = 0.;		// ����� ��
	int nIndx = 0;
	// %
	if(m_uTargetValuePreserve_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetValuePreserve);	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dClose + m_dTargetValuePreserve;				// �����ݾ�(��) ����� ���

	// EntryPosition �� ExitPosition ���̿��� ��ǥ������ �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare = 0.;
	double dValueForTargetValuePreserve = 0.;
	for( nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		if(dValueForCompare >= dBullValue)
		{
			dValueForTargetValuePreserve = dBullValue;
			break;
		}
	}
	
	// ��ǥ������ �ְ� ������ Position ���� ExitPosition ���̿��� �ְ� �϶� ���Ŀ� ���� ����û�� Position ��ȸ
	for(nIndx = nIndx + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		if(dValueForCompare < dValueForTargetValuePreserve)
		{
			FLInfo.SetFLInfo(nIndx, VALTARGETPRESERV, pHigh);

			// ����û�� �߻�
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// ���ͺ����� ���� ����û��
void CStrategyWnd::ForcedLiquidation_ForBenefitPreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// ���� ��ġ
	int nExitPosition = pExitDealData->m_nIndex;			// û�� ��ġ
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// ����

	double dBullValue = 0.;		// ����� ��
	int nIndx = 0;
	// %
	if(m_uBenefitPreserveUp_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dBenefitPreserveUp);	// ��������(%) ����� ���
	// ��
	else
		dBullValue = dClose + m_dBenefitPreserveUp;				// �����ݾ�(��) ����� ���

	// EntryPosition �� ExitPosition ���̿��� ���ͺ��� �ɼ����� ����û���� Position ��ȸ
	double dValueForCompare = 0.;
	double dValueForBenefitPreserve = 0.;
	for( nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� ����� ���
		if(dValueForCompare >= dBullValue)
		{
			dValueForBenefitPreserve = dBullValue;
			break;
		}
	}

	double dBearValue = 0.;		// �϶��� ��
	// %
	if(m_uBenefitPreserveDn_PerOrWon == 0)
		dBearValue = dValueForBenefitPreserve - (dValueForBenefitPreserve * m_dBenefitPreserveDn);	// ��������(%) ����� ���
	// ��
	else
		dBearValue = dValueForBenefitPreserve - m_dBenefitPreserveDn;										// �����ݾ�(��) ����� ���

	// ���ͺ��� �ְ� ������ Position ���� ExitPosition ���̿��� �϶��� �����Ͽ� ����û���� Position ��ȸ
	for(nIndx = nIndx + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// �ż���ȣ�� ���� �������� �������� �Ǵ� �����ݾ׺��� �϶��� ���
		if(dValueForCompare <= dBearValue)
		{
			FLInfo.SetFLInfo(nIndx, VALBENEFITPRESERV, pHigh);

			// ����û�� �߻�
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// �ּҰ������� ���� ����û��
void CStrategyWnd::ForcedLiquidation_ForMiniPrice(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// ���� ��ġ
	int nExitPosition = pExitDealData->m_nIndex;			// û�� ��ġ
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// ����

	if(m_nBongCount > 0)
	{
		// EntryPosition �� ExitPosition ���̿��� �ּҰ��ݺ�ȭ �ɼ����� ����û���� Position ��ȸ
		double dValueForCompare;
		double dRatio = m_dMiniPrice;
		for(int nIndx = nEntryPosition + m_nBongCount; nIndx <= nExitPosition; )
		{
			dValueForCompare = pClose->GetAt(nIndx);

			double dBullValue = 0.;		// ����� ��
			// %
			if(m_uMiniPrice_PerOrWon == 0)
				dBullValue = dClose + (dClose * dRatio); 	// �����Ǵ� ��������(%) ����� ���
			// ��
			else
				dBullValue = dClose + dRatio;				// �����Ǵ� �����ݾ�(��) ����� ���

			// ������ �������� �Ǵ� �����ݾ��̳��� Ⱦ���� ������ ���
			// �� ���ذ��� �������� ���� ���
			if(dValueForCompare <= dBullValue)
			{
				FLInfo.SetFLInfo(nIndx, VALMIN, pHigh);

				// ����û�� �߻�
				bExist_ForcedLiquidation = TRUE;
				break;
			}
			nIndx += m_nBongCount;
			dRatio += m_dMiniPrice;
		}
	}
}

// ����û������ ���� ����û��
void CStrategyWnd::ForcedLiquidation_ForDayLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;		// ���� ��ġ
	int nExitPosition = pExitDealData->m_nIndex;		// û�� ��ġ
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);

	CString szDate;
	CString szHour;
	int		nMin;
	CString szCheckHour;
	int		nCheckMin;

	// �� ����
	int	nInterval = m_nUnitPos;

	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		// ���Ͻú�(�� : 10281310)
		szDate.Format("%08.8s", m_pDateItem->GetAt(nIndx));
		szHour = szDate.Mid(4, 2);
		nMin = atoi(szDate.Mid(6, 2));

		// ������ �ú�(�� : 1310)
		szCheckHour = m_szTimeDayLiquidation.Mid(0, 2);
		nCheckMin = atoi(m_szTimeDayLiquidation.Mid(2));
		if(szCheckHour.Compare(szHour) == 0 &&		// Hour Check
			nCheckMin <= nMin && abs(nCheckMin - nMin) <= nInterval)		// Min Check
		{
			FLInfo.SetFLInfo(nIndx, VALDANGIL, pHigh);

			// ����û�� �߻�
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

void CStrategyWnd::ForcedLiquidation(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo)
{
	// �ߺ����� Ƚ������ �ʱ�ȭ
	m_nRepeatCount = 0;

	// ����û�� �ɼ����� �ٽ� ����
	pExitDealData->m_lDealValue = FLInfo.m_lDealValue;
	pExitDealData->m_nIndex = FLInfo.m_nExitIndex;

	// ��ȣ���� ����
	m_arrayDealPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_lDealValue);
	m_arrayDealDrawPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_pDataItem->GetAt(FLInfo.m_nExitIndex));

	// ���� ��ȣ�� û���ȣ�� ����
	m_bCheckPrev_EntryPosition = FALSE;
}

void CStrategyWnd::ForcedLiquidation_ForReal(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo)
{
	// ����û�� �ɼ����� �ٽ� ����
	pExitDealData->m_lDealValue = FLInfo.m_lDealValue;
	pExitDealData->m_nIndex = FLInfo.m_nExitIndex;

	// �� �ϼ��ø� ���� ���
	m_TempDealData = *pExitDealData;

	// ����û������� ���� �������� ���
	if(m_nTiming_ForcedLiquidation == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
	{
		// ��ȣ���� ����
		m_arrayDealPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_lDealValue);
		m_arrayDealDrawPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_pDataItem->GetAt(FLInfo.m_nExitIndex));

		// ���� �ֹ������� ���õ��� ���� ���
		if(m_bSetComplete_OrderInfo == FALSE)
		{
			m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
									m_TempDealData.m_lDealValue,
									m_TempDealData.m_lType,
									m_TempDealData.m_strDealName,
									m_TempDealData.m_dPrice,
									m_TempDealData.m_dContract,
									&m_TempDealData.m_strListEntryNameForExit);

			m_TempCondDealData = m_TempDealData;
			m_TempForcedDealData = m_TempDealData;

			// �ֹ����� ���ÿϷ�
			m_bSetComplete_OrderInfo = TRUE;

			// �ߺ����� Ƚ������ �ʱ�ȭ
			m_nRepeatCount = 0;
//			FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//			if(fp != NULL) 
//			{
//				fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "ForcedLiquidation_ForReal");
//				fclose(fp);
//			}
		}
	}
}