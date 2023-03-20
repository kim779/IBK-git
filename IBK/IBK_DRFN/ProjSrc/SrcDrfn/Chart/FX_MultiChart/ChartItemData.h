// ChartItemData.h: interface for the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_CHARTITEMDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./define/ChartMsgDef.h"
#include "./define/TR10450_10451.h"

#include "../COMMONTR/EU_p0602.h"
#include "../COMMONTR/EU_p0603.h"
#include "../COMMONTR/EU_p0604.h"
#include "../COMMONTR/EU_p0605.h"
#include "../COMMONTR/EU_p0606.h"
#include "../COMMONTR/EU_p0607.h"

//<<20100308_JS.Kim
////<<< 20090902 by kmlee
//#include "../COMMONTR/EU_x56000.h"
#include "../COMMONTR/SM_fx112.h"
////>>>
////>>

class CChartItem;
class CChartItemData  
{
public:
	const char* GetNextKey() { return m_chkey;	}
	const char* GetNextKeyOld() { return m_chkeyOld;	}

	CString GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt);

	BOOL	m_bNextMarketData; //���� ��ǥ ������ȸ ���� Flag
	void	SetBNextMarketData(BOOL bNext) { m_bNextMarketData = bNext; }
	CString GetCodeForRqstFromChartItemData() {return m_strCodeForRequest;}	
	BOOL	SetPacketDataInAdditionPacket(BOOL bReset); //������ǥ TR�����͸� ��Ʈ�� �ֱ� ���� 

	CString	m_strOrderType;

	//=================================================================================
	// 2005. 05. 18  add by sy, nam
	// ��Ʈ ��ø���� ���� �߰��� ����	
	int		m_nCntOverLap;
	BOOL	m_bOverLapping; 
	static CString CChartItemData::OverLapping_MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled = TRUE, BOOL bOverLapping = FALSE);
    //=================================================================================

	CChartItemData(CChartItem *pChartItem,CString m_strTRCode);
	void Init(CChartItem *pChartItem, CString strTRCode);
	virtual ~CChartItemData();
	void SetCodeForQuery(LPCTSTR lpCode, BOOL bInitKey = TRUE);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	stSend2Server* GetQueryData(BOOL bNext, ST_SELCHART* lpSelChart = NULL);
	BOOL MakeChartData(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	CString GetTrCode();
	// ���õ� ��Ʈ�� ���� ���� : �������� - ojtaso (20070215)
	void SetTRCode(LPCTSTR lpszTRCode);
	long GetLTrCode();
	CString GetCode() { return m_strCode;}
	long	GetCnt() { return m_lCnt;}
	BOOL AddPacketAndGraph(BOOL IsNoVolumeGraph = FALSE);
	BOOL AddPacketAndGraph2(BOOL bAddGraph = TRUE, BOOL bOverlapp = FALSE);		// ���ο� ��Ŷ,��Ʈ �߰� : �������� - ojtaso (20061224)
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	BOOL AddLoadedPacket(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart);
	
	CString GetPacketName();
	void RemoveRealItem();
	void AddRealItem();
	BOOL SetPacketData(BOOL bReset);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	BOOL SetLoadedPacketData(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart);
	BOOL IsFilled();
	void ClearData();
	CString GetChartItemDataTitle(long nIndex = 0);
	long GetCntChartItemDataTitle();
	BOOL ClearChartItemData();
	long GetMaximumRequestCnt();
	BOOL ProcRequestCntAfterQuery(long lCntReceived);
	BOOL RemovePacketAndGraph();
	
	// ������ �ٲ�� PacketName�� �ٲ۴� : �������� - ojtaso (20070711)
	void ChangePacketNameArray(LPCTSTR lpszTRCode);
	void SetChartNumericValue(int nValue);

	void GetOHLValue(CString &strOpen, CString &strHigh, CString &strLow);

	static CString MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled = FALSE, BOOL bOverLapping = FALSE);
	static CString MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle = 0,
									COLORREF clrGrp1 = RGB(218,46,46),COLORREF clrGrp2 = RGB(218,46,46),
									COLORREF clrGrp3 = RGB(46,46,218),COLORREF clrGrp4 = RGB(46,46,218),
									COLORREF clrGrp5 = RGB(218,46,46),COLORREF clrGrp6 = RGB(218,46,46),
									BOOL bUpFilled = 0, BOOL bDownFilled = 1,
									long lUpLineWidth = 1, long lDownLineWidth = 1, BOOL bHideGrp = 0);

private:
	CChartItem		*m_pChartItem;			// Parent Class;
	BOOL			m_bInitalChart;			// �ʱ�ȭ����
	CString			m_strCodeForRequest;	// �ڵ� (��������ȸ�� ���� �ڵ�..)
	CString			m_strCode;				// ���� ���� �������� �ڵ�

	// TR-CODE - 05. 05. 06 �߰� - ���� ������ TR�ڵ�� ������������ 
	// ��ȭ�� TR �ڵ�� "HWS + ����" �����̱� ������ CString ������ ���� �ֵ��� �߰��Ѵ�.
	CString			m_strTRCode;			
	
	CString			m_strName;
	char			m_chkey[ChartKeySize+1];//[14];		// ���� / ���� Fetch Key	
	char			m_chkeyOld[ChartKeySize+1];//[14];		// ���� / ���� Fetch Key		
	long			m_lCnt;				// ���� ��ü ������ ��
	long			m_lCntTemp;			// ���� ��ü ������ ��(�ӽ�)..
	long			m_lStartDate;
	long			m_lEndDate;
	CString			m_strOpen;			//	�ð�
	CString			m_strHigh;			//	��
	CString			m_strLow;			//	����
	CString			m_strPrice;			//	���簡
	CString			m_strSign;			//	��ȣ
	CString			m_strChange;		//	���
	CString			m_strChrate;		//	�����
	CString			m_strVolume;		//	�ŷ���
	CString			m_strPreprice;		//	���ذ�
	CString			m_strKey;			//	����Ű

	CString			m_strPosition;		//	�Ҽ��� �ڸ� ����

	long			m_lDataSize;
	CStringArray	m_strArrayRealItemRegistered;

public:
	SM_fx112_CHART*	m_pfx112_ChartData;		// íƮ����Ÿ - FX

	CStringArray	m_strArrayPacketName;
	CStringArray	m_strArrayGraphTitle;
	BOOL			m_bIsFilled;
	long			m_lCntRequired;
	long			m_lCntRequiredByPrim;
	long			GetDataSize() { return m_lDataSize;}
	long			GetData();
	CString*		GetPstrPacketName(long lPosByOutput, long lPosByInput, CString *pstrPacketName,CString *pstrPacketName1);
	long			GetCntForQuery(BOOL bNext);
	BOOL			RemoveData();

	BOOL			MaketOutputData(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	BOOL			MakeChartData_EU_p0602(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);

	//<<< 20090902 by kmlee
	stSend2Server*	GetQueryData_EU_x56000 (BOOL bNext, ST_SELCHART* lpSelChart = NULL);	//QUERY_STRJONGMOK
	//>>>
	
	CString			GetQueryDataMarket(BOOL bNext, int nRqstCnt);

	CString			GetReciveValue(short nType);
	CString			GetPipLowest();

	void AddGraphPriceChart(void);	// JSR 20060122 ADD For Making Price Chart
	void AddGraphPriceChart2NewItem(int& nRow, int& nCol, BOOL bAddGraph);	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
	void AddGraphPriceChart_Overlapp(int& nRow, int& nCol);
	//2006.08.28 - KHD
public:
	BOOL AddPacketAndGraphAnal();
	// �������� �ҷ����� : �������� - ojtaso (20070528)
	BOOL m_bResetChartCfgWith;

	void OnSelectChartItem(ST_SELCHART* lpSelChart = NULL);

};

#endif // !defined(AFX_CHARTITEMDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
