// IStrategyItem.h: interface for the CIStrategyItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTRATEGYITEM_H__5013587A_E05B_4CCC_88A3_9BC73BDDD09C__INCLUDED_)
#define AFX_ISTRATEGYITEM_H__5013587A_E05B_4CCC_88A3_9BC73BDDD09C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CIStrategyItem CIStrategyItem0001

#define PROFITRATEACCMULATED	0	// �������ͷ�
#define PROFITRATEAVG			1	// �Ǻ���ռ��ͷ�
#define PROFITRATECURRENT		2	// ���������Ǽ��ͷ�
#define PROFITLOSSALL			3	// �Ѽ���
#define PROFITALL				4	// ������
#define LOSSALL					5	// �Ѽս�
#define PROFITLOSSRATEALL		6	// �Ѽ���/�Ѽս�
#define MAXPROFIT				7	// �Ǻ��ִ�����
#define MAXLOSS					8	// �Ǻ��ִ�ս�
#define PROFITLOSSAVG			9	// �Ǻ���ռ���
#define PROFITAVG				10	// �Ǻ��������
#define LOSSAVG					11	// �Ǻ���ռս�

#define AMOUNTENTEREDALL		12	// �����Աݾ�
#define AMOUNTEXITALL			13	// ��û��

#define CNTALL					15	// �ѸŸż�
#define CNTPROFIT				16	// ���͸Ÿż�
#define CNTLOSS					17	// �սǸŸż�

#define RATEPROFITLOSS			18	// ���ͼ�/�սǼ�
#define RATEPROFIT				19	// ���͸Ÿ���
#define RATELOSS				20	// �սǸŸ���

#define CHANGERATE				21	// �Ⱓ�����
#define MAXPROFITSTRAIGHT		22	// �ִ����Ϳ��Ӽ� 
#define MAXLOSSSTRAIGHT			23	// �ִ뿬�ӼսǼ�
#define RATEPROFITEXCEED		24	// �����ʰ����ͷ�
#define PROFITSTRAIGHT			25	// �ִ뿬������
#define LOSSSTRAIGHT			26	// �ִ뿬�Ӽս�
#define CNTCANDLE				27	// �Ѻ���
#define CNTCANDLEPROFIT			28	// ���͸Ÿź���
#define CNTCANDLELOSS			29	// �սǸŸź���
#define RATEACCESS				30	// �Ⱓ������
#define CNTCANDLEACCESS			31	// ���Ի��º���
#define CNTCANDLEEXIT			32	// û����º���
#define CNTCANDLEAVGDEAL		33	// ��ոŸź���
#define CNTCANDLEAVGPROFIT		34	// ���͸Ÿ���պ���
#define CNTCANDLEAVGLOSS		35	// �սǸŸ���պ���


class CStrategyResult  
{
public:
	CStrategyResult() 
	{
		m_DealType = NONE;
		m_dTime = 0;
		m_dPrice = 0;
		m_lQty = 0;
		m_dAmount = 0;
		m_dEnteredAmount = 0;
		m_dProfit = 0;
		m_dProfitAccmulated = 0;
		m_dProfitRate = 0;
		m_dProfitRateAccmulated = 0;
		m_lCntCandleInAccess = 0;
		m_bOnlyValuate = FALSE;
	}
	enum DEALTYPE {
		NONE,
		BUY,
		EXITSHORT,
		SELL,
		EXITLONG
	};
	
	BOOL	m_bOnlyValuate;
	DEALTYPE m_DealType;			// �ŷ�����
	double	m_dTime;				// �ð�
	double  m_dPrice;				// ����
	long m_lQty;					// ����
	double m_dAmount;				// ����/û��ݾ�

	// Calculated By CStrategyWnd::CalaulateStrategyResultData()
	long m_lCntCandleInAccess;		// ����
	double m_dEnteredAmount;		// �������Աݾ�
	double m_dProfit;				// �Ǻ�����
	double m_dProfitAccmulated;		// ��������
	double m_dProfitRate;			// ���ͷ�
	double m_dProfitRateAccmulated;	// �������ͷ�
};

enum OPERATTIONSTATUS {
	STRATEGY_NONE = 0,
	STRATEGY_RUN,
	STRATEGY_PAUSE,
	STRATEGY_STOP,
	STRATEGY_STANDBY
};




class CIStrategyItem0001  
{
public:
	
	virtual ~CIStrategyItem() {};
	// 20060705 �Լ����� ���� : HWND hSocket,/*long pDBMgr,*/
	//virtual int Create(CWnd* pParentWnd,HWND hSocket,/*long pDBMgr,*/ long lKey, CStringArray &rStArrayPath) = 0;
	virtual int Create(CWnd* pParentWnd, long lKey, CStringArray &rStArrayPath,long pChartItem) = 0;

	virtual BOOL DestoryWindow() = 0;
	virtual HWND GetSafeHwnd( ) const = 0;
	// atcion command
	virtual BOOL LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption = NULL) = 0;	
	virtual BOOL LoadST(LPCTSTR lpStrategyPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate) = 0;	
	virtual BOOL Run(LPCTSTR lpOption = NULL) = 0;
	virtual BOOL Stop() = 0;
	virtual BOOL Pause() = 0;
	virtual long Update(long lPos, BOOL bIsUpdateData) = 0;		//@060922����Real���ü���
	virtual CString GetTime() = 0;
	virtual long GetCount() = 0;
	virtual CString GetInputCode() = 0;
	virtual CString GetEndDate() = 0;
	virtual long GetDataCount(long lIndex) = 0;
	virtual CString GetTime(long lIndex) = 0;
	virtual long GetPtrDrawInfo(long lIndex = 999) = 0;
	virtual BOOL CalculateStatisticalInfo(long lStartPos= -1, long lEndPos= -1) = 0;
	virtual long GetCntStrategyResult() = 0;
	virtual CStrategyResult* GetStrategyResult(long lIndex) = 0;
	virtual double GetStatisticalInfo(long lPos) = 0;
	virtual LPCTSTR GetErrorMessage() = 0;
	virtual BOOL HasAPData() = 0;
	virtual OPERATTIONSTATUS GetOperationStatus() = 0;
	virtual CString GetInputs(long lIndexST, long lIndexInput) = 0;
	virtual void GetCurrentPositonNProfit(long& rLCurrentDealPosition, double &rDCurrentProfitRate) = 0;
	virtual CString GetName(long lIndex) = 0; 
	virtual CString GetPath(long lIndex) = 0;
	virtual CString GetProfileInfo(LPCTSTR lpAppName,LPCTSTR lpKeyName) = 0;
	virtual	BOOL SetUpperLowerPrice(double dUpperPrice,double dLowerPrice) = 0;
	virtual BOOL SetVolumeForCalculatation(double dVol) = 0;
	virtual double GetVolumeInDealData() = 0;

	//KHD : �ð�����ȭ ���� ����
	virtual int GetTimeBong() = 0;
	virtual void SetTimeBong(int nBong) = 0;
	//KHD : ��������ȭ ����
	virtual void SetValOptimizer(LONG Val)=0;
	virtual LONG GetValOptimizer() =0;
	virtual CString	 GetSendValString()=0;
	virtual void SetSendValString(CString)=0;
	// atcion command
	virtual void OnSTSetRealDataST()=0;
	
	// ��Ʈ �ð����� ���� ����
	virtual void SetChartDivideInfo(int nDivideState, int nUnitPos) = 0;
};

#endif // !defined(AFX_ISTRATEGYITEM_H__5013587A_E05B_4CCC_88A3_9BC73BDDD09C__INCLUDED_)
