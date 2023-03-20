// IndicatorSingleton.h: interface for the CIndicatorFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORFACTORY_H__1F4A506F_3636_46B2_844F_B049AA6D1A3A__INCLUDED_)
#define AFX_INDICATORFACTORY_H__1F4A506F_3636_46B2_844F_B049AA6D1A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Indicator.h"

class CIndicatorSingleton  
{
public:
	CIndicator* Create( IMetaTable::EDDC_INDICATOR_ID p_eID, HWND p_hOcxHwnd);

// (2006/9/6 - Seung-Won, Bae) Check ST Index Indicator
public:
	CIndicator *CreateSTIndexCalculator( IMetaTable::EDDC_INDICATOR_ID p_eID);
};



// ����(05/08/03) ���ڵ�!!!
class CSystemTradingCalculate : public CIndicator  
{
public:
	CSystemTradingCalculate( IMetaTable::EDDC_INDICATOR_ID p_eID) : CIndicator( p_eID)	{}

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);
private:
	void SetGraphPacketList(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo, CCalculateData& calculateData);
	void SetSingalPacket(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo);

	void SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo);
// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
// 	void SetMAPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo, CCalculateData& calculateData);
// (2007/1/25 - Seung-Won, Bae) Use High/Low Packet directly on Draw Graph.
//	void SetPacketList_Parabolic(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo);
//	bool CParabolic_SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo, CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2);
//	bool CParabolic_InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo, CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, CSubGraphPacketList* pSubGraphList);
};

//�ŷ�����
class CAccount : public CIndicator  
{
public:
	CAccount();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool IsCurrentDateCloserthanPreDate(const int nPredate, const int nCurrentdate, const int nSellbuyDate);
	
	void GetPacketDateType(CPacket* pDate, CPacket* pAccountDate, CString& strgraphdate, CString& strsellbuydate, CString& strcomparedate);
	void GetRealDate(const CString& stroriginaldatetype, const CString& strdatetype, const CList<double, double>& dDateList, CList<double, double>& dRealDateList);

	void GetSellandBuyList(const CPoint& point, const CList<double, double>& dRealGraphDateList, const CList<double, double>& dRealSellBuyDateList, CList<double, double>* pSellBuyList, CPacket* pBuy, CPacket* pSell);
	bool CompareDate(const bool bLast, const CString strPreDate, const CString strCurrentDate, const CString strSellBuyDate, const int ngraph, const int nsellbuy, double& nsell, double& nbuy, double* nSell, double* nBuy, CList<double, double>* pSellBuyList);

	CString GetCompareDateType(const CString& strgraphdate, const CString& strsellbuydate);
	CString WhenHHMMSSNN(const CString& strGraphDateType);
	CString WhenHHMMSS(const CString& strGraphDateType);
	CString WhenDDHHMMSS(const CString& strGraphDateType);
	CString WhenDDHHMM(const CString& strGraphDateType);
	CString WhenDDHH(const CString& strGraphDateType);
	CString WhenMMDD(const CString& strGraphDateType);
	CString WhenYYMM(const CString& strGraphDateType);
	CString WhenYYYYMM(const CString& strGraphDateType);
	CString WhenYYMMDD(const CString& strGraphDateType);
	CString WhenYYYYMMDD(const CString& strGraphDateType);

	CList<double, double>* m_pDateList;
	CList<double, double>* m_pHighPriceList;
	CList<double, double>* m_pLowPriceList;
	CList<double, double>* m_pAccountList;
	CList<double, double>* m_pAccountDateList;
	CPacket* m_pDatePacket;
	CPacket* m_pAccountDatePacket;
};

//BigNews
class CBigNews : public CIndicator  
{
public:
	CBigNews();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool IsValidBigNews(CList<double, double>* pGraphDateList,
					    CList<double, double>* pBigNewsDateList,
					    CPacket* pBigNews);
	void GetBigNews(const CPoint& point, CList<double, double>* pGraphDateList, CList<CString, CString>* pBigNewsList, CList<double, double>* pBigNewsDateList, CPacket* pBigNews);

	CList<double, double>* m_pDateList;
	CList<CString, CString>* m_pBignewsDataList;
	CList<double, double>*m_pNewsDateList;
};

class CBoxChart : public CIndicator  
{
public:
	CBoxChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo* pIndicatorInfo);

private:
	bool ClearPacket( const CCalculateBaseData::CALCULATETYPE calculatetype, 
		             CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList, CIndicatorInfo*& pIndicatorInfo);
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);

	// (2004.10.20, ��¿�) Box Chart ������ ���� Packet�� ������ Routine���� �������� �ʴ´�.
	void GetForwardBoxChart(const CPoint& point, const double dCondition,
							CList<double, double>* pTopPriceList,
							CList<double, double>* pBottomPriceList,
							CList<double, double>* pStartPriceList,
							CList<double, double>* pEndPriceList);
	void GetBackwardBoxChart(const CPoint& point, const double dCondition,
							CList<double, double>* pTopPriceList,
							CList<double, double>* pBottomPriceList,
							CList<double, double>* pStartPriceList,
							CList<double, double>* pEndPriceList);

private:
	// (2004.10.20, ��¿�) Box Chart�� �����ϴ� Packet�� 4���� �����Ѵ�.
	CPacket* m_pBoxStartPacket;
	CPacket* m_pBoxHighPacket;
	CPacket* m_pBoxLowPacket;
	CPacket* m_pBoxEndPacket;

	CList<double, double>* m_pStartPriceList;
	CList<double, double>* m_pHighPriceList;
	CList<double, double>* m_pLowPriceList;
	CList<double, double>* m_pEndPriceList;
};

class CFourierTransform : public CIndicator  
{
public:
	CFourierTransform();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

};


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.02.05
//
//	�м�	:	���� ���� P&F Chart (�ɼ� �ƴ�. �������� ȣ�� ���� �������� Hard Codeing �Ǿ� ����.)
//				1. �⺻����
//					1. ���� ���� P&F�� t�к� ���� Ȥ�� Tick�� Price�� �������� nȣ�� ������ ��ȯ���̴�.
//						(�Ｑ ��ȯ���� ����)
//					2. ����� 'X', �϶��� 'O'ǥ�� �Ͽ� MarkLine�� �����Ѵ�.
//					3. ����� ������ ���� ��/�� ������ nȣ�� ���� �����Ǿ�߸� ���� ��ȯ�� �����ϰ�,
//						��ȯ ���� ���������� Marking�� ó���ϰ� �ȴ�.
//					4. Mark�� �׻� ȣ������ (0.05)���� ǥ���Ѵ�.
//					5. ���ڰ� ����Ǵ� ���, �ش� Mark���� ������ �����Ѵ�. (������ û���� �ݺ��ϸ� ��ȯ�Ѵ�.)
//				2. ���� ����
//					1. TR�� ���� ��ȸ�ÿ��� Server���� �̹� P&F Chart�� �׸��� ���� Data���� �����Ͽ� �����ش�.
//						(�� Packet�� �ϳ��� ��� MarkLine�̴�.)
//							�ڷ�����	: �ش� MarkLine�� ��ϵ� ������ ���� Data�� �ð��̴�.
//										  P&F Chart���� ��µ��� ������, ���� Real�� ���Žÿ� ���� ������ ������ �ȴ�.
//							�ð�		: MarkLine�� ���� �����̴�.
//							����		: MarkLine�� �� �����̴�.
//										  MarkLine�� ���� ���ݿ� ���� ������η� ����� ��� 'X', �϶��� ��� 'O'ǥ���Ѵ�.
//							�����ڰ�	: �ϳ��� MarkLine������ ���ڰ� ����Ǵ� ��쿡 ������ ������ Mark�� �����̴�.
//					2. Server���� �����ִ� Packet���� Ư���� ��츦 ó���ϱ� ���� ���� Packet�� �����´�.
//						1. ��1 ���� Packet (�������� Packet)	: ��� Packet Data�� �ռ� �׻� ������ ù��° �⺻ Packet�̴�.
//								�ڷ�����	: ����(��ȸ����)�� ����� �ð��̴�.
//												������ �������� �ƴ� ���, (Real�� ������?) 0���� ���´�.
//												�� ���� ��ȸ����, �������, �к�����, ���� Data���� ��ȸ�ϴ� ��쿡,
//													ù ���� �����ϱ� ���� �ð����� 1�к��� ��� 1�������� Data '����'���� �����Ͽ� ���� �����ȴ�.
//								�ð�		: �ڷ���ȸ �������� '��������'
//												Server���� ������ (��/��)�߼��� ���� ���, ȭ�鿡 ����� ���ذ��̴�.
//												���߼� ������ �ʱ� ��/������ �̿�ȴ�.
//								����		: �̿����
//								�����ڰ�	: �̿����
//						2. �� 2���� Packet (���� Packet)	: ��� Packet Data�� �ռ� �׻� ������ �ι�° �⺻ Packet�̴�.
//												������ ������ Real ���ſ� ���Ͽ� �ϼ��Ǵ� �̿ϼ� ������ �����Ͽ� ������ Packet���� �޴´�.
//												�� �Ʒ��� ���߼��� �߼� Packet�� ������� �ʰ� ������ �����Ͽ� ���߼� Packet���� �����ϰ� �ȴ�.
//								�ڷ�����	: ������ ���۽ð��̴�.
//								�ð�		: �̿����
//								����		: ������ �����̴�.
//								�����ڰ�	: �̿����
//						2. ���߼� Packet Data	: Server�� ��ȸ�� Data�� ������,
//											  Server���� ������ �߼��� �ϳ��� ���� ��� ��������, �߼� ������ Packet�̴�.
//								[���߼�]	: ���� �߼��� ���� �߼��� ������ �߼�ġ�� �������� ������ Ȯ���ϰԵǴµ�,
//													ù �߼��� ���� �߼��� ���� ����� ���/�϶��� �߼��� ���ÿ� �����ϸ鼭,
//													�� ���� ���� ȣ���� �Ѿ�� ������ �߼��� �����ϰ� �ȴ�.
//												�߼� �������� ��/������ ���Žÿ��� ���������� ���ŵ� ���� ������ ��Ÿ������ �Ѵ�.
//												Ư�� ���߼��� Drawing Packet���� ��ϵ��� �ʴ´�. ���� ���߼��� �ݿ��Ǿ� ���δ�.
//								�ڷ�����	: �߼� ������ ���ȵ� ������ Data�� �ð��̴�. (�߼��� ���ڿ� �̿��� �� ������ �̿����� �ʰ��ִ�.)
//								�ð�		: �߼� ���� �ð���, ���߼��� �����ϴ� ���� �߻��� ������ Ȥ�� �ְ��̴�.
//								����		: �߼� ���� ������, ���߼��� �����ϴ� ���� �߻��� ������ Ȥ�� �ְ��̴�.
//								�����ڰ�	: �߼� �����߿� �߻��� �����ڰ���, ������ �������� �ð��� ��ϵǾ� �ִ�.
//												�����߿� �߻��� ���� �ð��� ���� ���� 0���� �����ȴ�.
//												Ư�� ��ȸ�������� ���� �ð��� ���� �߼��� �������� �����Ƿ� ���� 0���� �����Ѵ�.
//						3. �߼� Packet Data	: Server�� ������ �߼� Packet�̴�.
//								�ڷ�����	: �߼��� ���Ե� ������ Data�� �ð��̴�.
//												(�� ���ο� �߼��� ������ Data��, �ٷ� ���� Data �ð��̴�.
//												 �߼��� ���ڿ� �̿��� �� ������ �̿����� �ʰ��ִ�.)
//								�ð�		: �߼��� ���� �����̴�. ���� �߼��� �������� 0.05�� ũ�ų� (����� ���) �۴� (�϶��� ���)
//								����		: �߼��� �� �����̴�. �ð��� ������ ����߼����� �϶��߼����� �ĺ��Ѵ�.
//								�����ڰ�	: �߼� �����߿� �߻��� �����ڰ���, ������ ���ڰ� ����Ǵ� ����� ������ ���� �ð��� ��ϵȴ�.
//												�߼� �����߿� �߻��� ���� �ð��� ���� ���� 0���� �����ȴ�.
//												Ư�� ��ȸ�������� ���� �ð��� ���� �߼��� �������� �����Ƿ� ���� 0���� �����Ѵ�.
//					3. ��¿� ���� Packet
//						1. Scale Packet		: Drawing ó���� Y Scale�� ���߼��� ���� �޺��ϴ� ��츦 �����ϱ� ���Ͽ�,
//												Drawing�� ���� Min/Max Data�� �����Ͽ�, �������� ��Ҹ��� �����ϵ��� �����ϴ� Packet�̴�.
//											  Drawing ó������ �ϰ������� ó���� �����ϵ��� �׻� �� ó�� �߰��Ѵ�.
//								�ڷ�����	: �ڷᱸ�� ����
//								�ð�		: ��ü ��� Data�� �ּҰ�
//								����		: ��ü ��� Data�� �ִ밪
//								�����ڰ�	: �̿� ����
//						2. ���߼� Packet Data	: Server���� �����ִ� Packet�� �ƴ�, Real�� �к� ó���� Tic�� ���� �����Ǵ� �߼� Packet�̴�.
//								[���߼�]	: ���߼��� �к��� ���� ������ Ȯ������ �ʴ� ��Ȳ������ �ӽ� �߼��� ��Ÿ���� Packet����,
//													Server�� �ƴ�, ��ǥ ����� �� 2���� Packet�� �������� �����Ǹ�,
//													Tic�� ���� �߼� �����̳� Ȯ���� ��Ÿ���� Packet���� �׻� ��ϵȴ�.
//											  ���߼��� ��ǥ ��� Routine�� �ϰ����� ���Ͽ�, Real �Ӹ� �ƴ϶� TR�� ��쿡�� �߰��Ѵ�.
//											  ���� ���߼��� ��½� �� �߼����� Ȯ���Ͽ� �������� �Ѿ�� ��쿡�� ��������,
//													�׸���, �߼�Ȯ���� ��쿡�� Ȯ������ ���ó���Ѵ�.
//								�ڷ�����	: �ڷᱸ�� ����
//								�ð�		: Tic�� ���� �ӽ÷� ������ �߼��� �ð��̴�.
//								����		: Tic�� ���� �ӽ÷� ������ �߼��� �����̴�.
//								�����ڰ�	: ���߼��� �����ϴ� ������ �����߼��� ��¥�� �ٸ� ��� ���� �������� �߼��� �����̴�.
//					4. Real ó��
//						1. ���߼� ����	:
//								1. Server���� �߼� Packet Data�� ����, ���� Packet��, Ȥ�� ���߼� Packet�� �Բ�
//										�������� ����̴�.
//								2. ���߼��� ��ǥó���� Packet���� �������� �ʰ�, ���� ��Ȳ �����θ� �����Ѵ�.
//								3. �� �к��� �������� ���Ͽ� ���߼��� Ȯ���Ͽ� ������. (�߼��� Ȯ���ɶ�����)
//								4. ���� Packet�� �ð�(��ȸ������ ���� ����)�� �������� ������ ���߼� Packet���� ����Ѵ�.
//								5. �߼��� ������ ���, ���ο� �߼� Packet���� ��Ͻ�Ų��.
//						2. �߼� ����	:
//								1. Server���� �߼� Packet Data�� �����ְų�, Real�� ���� �߼��� Ȯ���� ����̴�.
//								2. �� Real Data�� �����Ͽ� �߼��� Ȯ�� �Ǵ� ������ ���߼� Packet�� ����Ѵ�.
//						3. Scale Packet ����	:
//								1. ���߼�/���߼�/���߼��� ������ ��� �߼� Data���� Drawing�� ���Ͽ�
//										�߼� Data���� Min/Max�� Packet���� �����Ѵ�.
//								2. ù ��ǥ Packet�� ��/�� ���� ��� Update�Ѵ�.
//						4. ���߼� Packet ����	:
//								1. �к� �߼������� Real������ ������ �̿ϼ� �к��� �߼��� �����ϴ� Packet����,
//										��ǥ Packet�� �������� �׻� ��Ͻ�Ų��.
///////////////////////////////////////////////////////////////////////////////
class CFuOpPF : public CIndicator
{
public:
	CFuOpPF();

// (2004.02.05, ��¿�, �м�) Input Packet�� �����Ѵ�.
protected:
	CPacket *	m_pIMLDatePacket;			// �ڷ����� Packet
	CPacket *	m_pIMLStartPricePacket;		// MarkLine �ð� Packet
	CPacket *	m_pIMLEndPricePacket;		// MarkLine ���� Packet (Real ä�ᰡ Packet)
	CPacket *	m_pIMLNewDatePricePacket;	// �����ڰ� Packet

// (2004.02.05, ��¿�, �м�) Output Packet�� �����Ѵ�.
//		(��½ÿ��� Date Packet�� �ʿ�� ���� �ʴ´�.
//		 �� Mark���� �ð��� ������� �ʰ�, MarkLine ������ �ð��� �ǹ̰� ����.)
protected:
	CPacket *	m_pOMLStartPricePacket;	// MarkLine �ð� Packet
	CPacket *	m_pOMLEndPricePacket;	// MarkLine ���� Packet
	CPacket *	m_pOMLNewDatePricePacket;	// �����ڰ� Packet

// (2004.02.05, ��¿�, �м�) ���� P&F Chart�� ��ǥ�� �����ϴ� �⺻ Routine�̴�.
protected:
	virtual void	GetData( const CChartFormulateData &chartFormulateData,
						CGraphCalculateData *pGraphCalculateData, CIndicatorInfo *&pIndicatorInfo,
						CCalculateData &calculateData);

// (2004.02.05, ��¿�, �м�) ���� P&F Chart�� TR & Real�� ó���ϴ� Routine�� �����Ѵ�.
protected:
	void	GetTRFuOpPF( void);
	void	GetRealFuOpPF(	const CPoint &point);

// (2004.02.24, ��¿�) P&F ��ǥ�� �����ϱ� ���� ���ǰ��� ���Ͽ� �����Ѵ�.
protected:
	int		m_nStickTimeWidth;		// Real ���� �ð� (�д���, 0�� Tick)
	int		m_nNewTrendPriceX100;	// �߼����� ȣ������

// (2004.02.24, ��¿�) Real�� ó���ϱ� ���� ���� ���¸� �����Ѵ�.
protected:
	int		m_nLastStickTime;		// ������ ���۽ð� (�̰��� �������� Real�� ������ �����Ѵ�.)

// (2004.02.24, ��¿�) ���߼� ���� ������ �����Ѵ�.
protected:
	int		m_nMiniTrendTime;				// ���߼��� ������ �ð��̴�. (�̰����� ���߼� ���� ������ ��Ÿ����.)
											//		0  <	���߼��� �������� ����
											//		0  =	�̹� ���߼��� ������ ����
											//		-1		���� ���߼��� �������� ���� ���� (������ ���Ͽ� ���߼� ���� ����)
											//		-2		���� ���߼��� �������� ���� ���� (������ ���� ���߼��� �������� ���� ����)
											//		-3		���� ����
											//	Ư��, Server�� Packet�� ������ �ִ� ��츦 �����Ͽ�
											//		Server Packet�� Ȯ���ϱ⿡ �ռ�, ���߼� ���� ������ �ʱ�ȭ�Ѵ�.
											//	Server Packet�� �����ÿ��� ���߼��� �̹� �ִ� ��ó�� �����Ͽ�,
											//		Real�� ���� ���߼� ������ ������� �ʵ��� �Ѵ�.
	int		m_nMiniTrendStartX100;			// ���߼��� �ð��̴�.
	int		m_nMiniTrendEndX100;			// ���߼��� �����̴�.
	int		m_nMiniTrendNewDatePriceX100;	// ���߼��� �����ڰ��̴�.
	int		m_nFirstBasePriceX100;			// ù ���߼��� Real�� �����ÿ� ������ ��ȸ ���� ������ �����Ѵ�.

// (2004.03.01, ��¿�) ������ ���߼��� �ð��� �����Ѵ�. (���߼��� �����ڰ� ����� �����Ѵ�.)
protected:
	int		m_nFixedTrendTime;

// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
//		��, ����۽ð��� ������ �����Ͽ�, ������ ������ ���� ���, ������ ���� ���, ���� ��츦 �����Ͽ� ������ �� �ֵ��� �Ѵ�.
protected:
	int		m_nMarketStartTime;
};


class CGlanceBalance : public CIndicator  
{
public:
	CGlanceBalance();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pTuringPoint, CPacket*& pStandardLine,
							CPacket*& pLagSpan);

	bool InCaseNoPacketPointer(const CCalculateBaseData::CALCULATETYPE calculatetype,
							   CPacketList* pPacketList,
							   CIndicatorInfo*& pIndicatorInfo,
							   CGraphCalculateData* pGraphCalculateData,
							   CPacket*& pTuringPoint, CPacket*& pStandardLine,
							   CPacket*& pLagSpan);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList,
						   CSubGraphPacketList* pSubGraphList, CIndicatorInfo*& pIndicatorInfo);

	void GetLeadSpan1(const CPoint& point, const int nDataCount, double dCondition, CList<double, double>* pTurningPointLine, CList<double, double>* pStandardLine, CList<double, double>& dLeadSpanList);
	void GetLeadSpan2(const CPoint& point, const double& dSpanCond1, const double& dSpanCond2, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dLeadSpanList);
	void GetLagSpan(const CPoint& point, double dCondition, CList<double, double>* pEndPriceList, CList<double, double>& dLagSpanList);
	void GetMA(const CPoint& point, double dCondition, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dResultList);
	void GetStandardLine(const CPoint& point, double dCondition, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dStandardLineList);
	void GetTurningPointLine(const CPoint& point, double dCondition, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dTurningPointLineList);

	CPacket* m_pLeadSpan1;
	CPacket* m_pLeadSpan2;
	CList<double, double>* m_pHighPriceList;
	CList<double, double>* m_pLowPriceList;
	CList<double, double>* m_pEndPriceList;
	CPacket* m_pHighPacket;

};


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.02.03
//
//	�м�	:	��� �Ź� Chart
//					1. ������ �ŷ��� Packet�� Input�����Ͽ�, ���Ź� ���ݴ� Packet��
//						���Ź� �ŷ������� Packet�� �����Ѵ�.
//					2. ���Ź� ���ݴ� Packet�� �ŷ������� Packet�� ������ ������ ���ݴ� ������ ��ϵǾ� �ִ�.
//					3. ���Ź� ���ݴ� Packet�� ��ϵǴ� ���� �� ���ݴ� ������ ���Ѱ��̴�. (SetPriceZon() ����)
//					4. ���Ź� �ŷ������� Packet�� ���� ����ġ���� 2�� �� �ʱ�ȭ�Ѵ�.
//						�װ��� ���� Drawing �������� ������ Data�� ���� ������ ���ݴ� Index��
//						���� �������� ���ݴ뺰 �ŷ��������� �ִ��� ���ݴ��� Index�� �����Ѵ�.
//						Drawing�ÿ� ���� ���ݴ�� �ִ� ���ݴ��� High Light�� �����ȴ�.
///////////////////////////////////////////////////////////////////////////////
class CHorizontalBar : public CIndicator  
{
public:
	CHorizontalBar();

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
					CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo,
					CCalculateData& calculateData);

// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ�� ���⿡ �ʿ��� Packet�� �����Ѵ�.
protected:
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;
	CPacket* m_pHorZonPacket;
	CPacket* m_pHorZonPricePacket;

// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ�� ���� ������ �⺻���� �����Ѵ�.
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
// (2008/4/23 - Seung-Won, Bae) Make the price unit double
	double	m_dHoga;
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
protected:
	int		m_StartPoint;	// ���Ź� ���� ������ ���� Index�̴�.
	int		m_EndPoint;		// ���Ź� ���� ������ �� Index�̴�.
	double	m_Max;			// ���Ź� ���� ������ �ְ� �����̴�.
	double	m_Min;			// ���Ź� ���� ������ ���� �����̴�.

private:
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
//	void GetHorizontalBar(const CChartFormulateData& chartFormulateData, 
//				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
//				CList<double, double>* pVolumeList);
	void GetHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,double dConditionType, double dHoga);
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
};

class CInverseLine : public CIndicator
{
public:
	CInverseLine();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);

	CPacket* m_pPriceMA;
	CPacket* m_pVolumeMA;
	CPacket* m_pEndPricePacket;
	CPacket* m_pVolumePacket;
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;

};

class CSwingChart : public CIndicator
{
public:
	CSwingChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);
	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);
	void GetSwingChart(const CPoint& point,
					   double dUnitValue,
					   double dInverseUnit,
					   CList<double, double>* pEndPriceList,
					   CList<double, double>* pDateList,
					   CPacket* pSwingPrice,
					   CPacket* pSwingDate);

private:
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType,
						   CPacketList* pPacketList);
	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	bool InCaseNoPacketPointer(CPacketList* pPacketList,
							   CIndicatorInfo*& pIndicatorInfo,
							   CSubGraphPacketList* pSubGraphList);

	CPacket* m_pEndPricePacket;
	CPacket* m_pDatePacket;
	CPacket* m_pSwingPrice;	// Swing Price ��Ŷ
	CPacket* m_pSwingDate;	// Swing �ڷ����� ��Ŷ
};

class CKagiChart : public CIndicator
{
public:
	CKagiChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);
private:
	// ���ǰ� ����� ���� ���� - ojtaso (20070913)
	void GetKagiChart(const CPoint& point,
					 double dUnitValue,
					 double dInverseUnit,
					 CList<double, double>* pEndPriceList,
					 CList<double, double>* pDateList,
					 CPacket* pKagiPrice,
					 CPacket* PKagiDate);
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType,
						   CPacketList* pPacketList);
	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	bool InCaseNoPacketPointer(CPacketList* pPacketList,
							   CIndicatorInfo*& pIndicatorInfo,
							   CSubGraphPacketList* pSubGraphList);

	CSwingChart m_swingChart;
	CPacket* m_pEndPricePacket;
	CPacket* m_pDatePacket;
	CPacket* m_pKagiPrice;		// Kagi ����.
	CPacket* m_pKagiDate;	// Kagi ��ǥ�� �ڷ�����
};

//������
class CLock : public CIndicator  
{
public:
	CLock( HWND p_hOcxWnd);
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							const CString& strPacketNames, 
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pLock);
	
	void GetLock(const CPoint& point, CList<double, double>* pLockList, CList<double, double>* pLockRatioTextList, CPacket* pLock);
	CString GetData(const int nData) const;
	CString GetRatioTextData(const double nRatioData) const;

private:
	CList<double, double>* m_pLockYesOrNotList;
	CList<double, double>* m_pLockRatioTextList;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;
};

class CMPPacket;
class CMProfile;
class CMarketProfileChart : public CIndicator
{
public:
	CMarketProfileChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	void	GetTotalMarketProfile( const CPoint& point, CList<double, double>* pConditionList,
				CMPPacket* pMarketPacket, int Start, int End, CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	void	GetDayMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
				CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	void	GetRealMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
				CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	void	InsertUp( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate);
	void	InsertDown( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate);
	int		GetMDay( int nDate);

private:
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;
	CList<double, double>* m_pDateList;
	CList<double, double>* m_pIndexList;
	CList<CString, CString>* m_pAlpabetList;

	CString m_strDateType;
	CPacketList* m_pPacketList;
	CCalculateBaseData::CALCULATETYPE m_CalType;

// (2004.01.19, ��¿�) CMPItem���� CBracket�� �ű� �߰��� �ĺ��ϱ� ���Ͽ� ���� �ð��� ������ �����Ѵ�.
// (2004.01.26, ��¿�) Real ó���ø� ���� �Ϻ�/���� ���ε� �����Ѵ�.
protected:
	int		m_nTimeUnit;
	int		m_nPriceUnitX100;
	int		m_nTotalMPChart;	// 1:����, 0:�Ϻ�

// (2004.01.19, ��¿�) ���������� ����� Data�� ���� �����Ѵ�.
//		������ Data �߰��� �����ϱ� �����̴�.
// (2004.01.20, ��¿�) ���ݸ�ŭ�� ������ �����Ѵ�. (double������ ������ ���ֱ� �����̴�.)
protected:
	int		m_CurrentIndex;			// ���������� ��ϵ� CBracket�� �ִ� CMPItem�� Index ��ȣ�̴�.
	int		m_CurrentTime;			// ���������� ��ϵ� CBracket�� Data Time�̴�.
	int		m_CurrentPriceX100;		// ���ڸ����� ��ϵ� CBracket�� �ִ� CMPItem�� �����̴�.
	int		m_CurrentBracketIndex;	// ���������� ��ϵ� CBracket�� Index���̴�.
	char	m_CurrentChar;			// ���������� ��ϵ� CBracket�� Character Item�̴�.

// (2004.01.26, ��¿�) ������ 'A'�� �ð��� �����Ͽ�, Real�� ó������Ѵ�.
protected:
	int		m_LastAMinute;			// �ð� ������ �̿����� �ʴ´�.
};


class CPF : public CIndicator  
{
public:
	CPF();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);
private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);
/*	void WhenDownduringUp(const int& nRet,
						  const double& dClose,
						  const double& dDate,
						  const double dUnitSize,
						  bool& UPCheck,
						  double& dBaseData,
						  double& conversionValue,
						  CList<double, double>& dTOPList,
						  CList<double, double>& dBOTTOMList,
						  CList<double, double>& dDATEList);
	void WhenDownduringDown(const double& dClose,
							const double& dDate,
							const double dUnitSize,
							double& dBaseData,
							CList<double, double>& dTOPList,
							CList<double, double>& dBOTTOMList,
							CList<double, double>& dDATEList);
	void WhenUpduringDown(const int& nRet,
						  const double& dClose,
						  const double& dDate,
						  const double dUnitSize,
						  bool& UPCheck,
						  double& dBaseData,
						  double& conversionValue,
						  CList<double, double>& dTOPList,
						  CList<double, double>& dBOTTOMList, 
						  CList<double, double>& dDATEList);
	void WhenUpduringUp(const double& dClose,
						const double& dDate,
						const double dUnitSize,
						double& dBaseData,
						CList<double, double>& dTOPList,
						CList<double, double>& dBOTTOMList,
						CList<double, double>& dDATEList);*/
	void GetSecondDataandStartPoint(CList<double, double>* pClosePriceList,
									const double dUnitSize,
									int& startpoint,
									double& seconddata);
	void WhenUpduringUp(const double& dClose, const double& dDate, const double dUnitSize,
						 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						 CPacket* pDATEPacket);
	void WhenUpduringDown(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket);
	void WhenDownduringDown(const double& dClose, const double& dDate, const double dUnitSize, 
							 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
							 CPacket* pDATEPacket);
	void WhenDownduringUp(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket);
/*	void GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
			   CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
			   CList<double, double>& dTOPList, CList<double, double>& dBOTTOMList,
			   CList<double, double>& dDATEList); */
//	void GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
//			   CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
//			   CPacket* dTOPList, CPacket* dBOTTOMList, CPacket* dDATEList);
	void GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
			   CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
			   CPacket* dTOPList, CPacket* dBOTTOMList, CPacket* dDATEList,
			   CList<double, double>* pDrawConditionList,BOOL bReal,BOOL bDataType, CPacketList* pPacketList);

	double GetUnitValue(const double nMin, const double nMax, CList<double, double>* pConditionList);

	CPacket* m_pTLBTop;
	CPacket* m_pTLBBottom;
	CPacket* m_pTLBDate;
	CPacket* m_pEndPricePacket;
	CPacket* m_pDatePacket;
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pDatePriceList;
};

class CRainbow : public CIndicator
{
public:
	CRainbow();
	~CRainbow();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(const CChartFormulateData& chartFormulateData,
							  CGraphCalculateData* pGraphCalculateData);

	bool IsIndicatorCalculated(const CCalculateBaseData::CALCULATETYPE calculatetype);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const double dPacketCount, CPacketList* pPacketList, CIndicatorInfo* pIndicatorInfo);
	void GetRainbow(const CPoint& point,
					const double dFirstCondition, 
				    const double dIntervalofMV, 
				    const double dPacketCount, 
					const CString& strType,
				    CList<double, double>* pEndPriceList, 
				    CPacketList* pPacketList,
					CChartInfo::CHART_MODE p_eChartMode);

	CList<CPacket*, CPacket*> m_RainbowList;
	CList<double, double>* m_pEndPriceList;
	CPacket* m_pEndPricePacket;
};

class CRelative_Strength : public CIndicator  
{
public:
	CRelative_Strength();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool GetCondition(const CIndicatorInfo* pIndicatorInfo, double& dCond) const;
	void GetRSC(const CPoint& point, const double& dCond, const CPacket* pClosePrice, CPacket* pRSC);
	double GetRSCData(const double& dFirstData, const double& dPriceData, const double& dCond) const;
	bool GetEnvironmentDataForCalRSC(const CPacket* pClosePrice, 
		CList<double, double>*& pClosePriceDataList, int& nCount, double& dMin, double& dMax, double& dFirstData) const;
	bool InitializeRSC(CPacket* pRSC, const int nStartIndex, const int nCount, const double& dCond, const double& dMin, const double& dMax);

	void GetRSC(const CPoint &point, const double dCondition, const double nMin, const double nMax, CList<double, double>* pClosePriceList, CPacket* pRSC);
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const CString& strSubName, CPacketList* pPacketList, CIndicatorInfo* pIndicatorInfo);

	void SetPacketTypes(const double& dCond);
	bool SetGraphPacketList(const CString& strSubGraphName,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);

private:
	CPacket* m_pRS;
	CList<double, double>* m_pEndPriceList;
	CPacket* m_pEndPricePacket;
};

/**************************************************************************
* Chart Name : RenKo Chart
* Made By    : Yoo Je Sun
* Date       : 2005.06.21.
**************************************************************************/
class CRenKoChart : public CIndicator  
{
public:
	CRenKoChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

	void GetRenKoChart(const CPoint &point, double dBrickCondition,
								CList<double, double>* pClosePriceList,
								CList<double, double>* pDateList);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);
	
	// ������ ����� ���񺰷� �������� �Լ�
	double GetBrickSize(CIndicatorInfo* pIndicatorInfo, CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const;
	double GetBrickSize(CIndicatorInfo* pIndicatorInfo) const;
	double GetBrickSize(CPacket* pClosePacket) const;
	double GetBrickSize_Stock(CPacket* pClosePacket) const;
	double GetBrickSize_Price(CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const;

private:
	CPacket* m_pClosePrice;
	CPacket* m_pDate;

	CPacket* m_pRenKoHighPrice;	// RenKoChart Open Price (�ð� ��Ŷ)
	CPacket* m_pRenKoLowPrice;	// RenKoChart Close Price (���� ��Ŷ)
	CPacket* m_pRenKoDate;		// RenKoChart Date(�ڷ����� ��Ŷ)
};

class CThreeLineBreak : public CIndicator  
{
public:
	CThreeLineBreak();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

//	void GetThreeLineBreak(const CPoint &point, CList<double, double>* pClosePriceList, CList<double, double>* pDateList, CList<double, double>& dTOPList, CList<double, double>& dBOTTOMList, CList<double, double>& dDATEList);
//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	void GetThreeLineBreak(const CPoint &point,
										CList<double, double>* pClosePriceList,
										CList<double, double>* pDateList,
										CPacket* dTOPList,
										CPacket* dBOTTOMList,
										CPacket* dDATEList,
										CList<double, double>* pConditionList);
//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);

	CPacket* m_pTLBTop;
	CPacket* m_pTLBBottom;
	CPacket* m_pTLBDate;
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pDateList;

};

class CZigzag : public CIndicator  
{
public:
	CZigzag();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo);

	void GetZigzag(const CPoint& point, double dCondition1, double dCondition2,
				   CList<double, double>* pEndPriceList, CList<double, double>* pZigDataList,
				   CList<double, double>* pZigIndexList, bool CLCflage, 
				   CScaleBaseData::HORZSCALEDRAWERTYPE m_nType);

private:
	double MakeChangHighValue(double prevValue, double dCondition2, double dCondition1,
						      CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	double MakeChangLowValue(double prevValue, double dCondition2, double dCondition1,
							 CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	bool ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, 
						  CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList, CIndicatorInfo*& pIndicatorInfo);
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	double GetRealDataIndex(CList<double, double>* pEndPriceList,CList<double, double>* pZigDataList, 
					             CList<double, double>* pZigIndexList, int& nPoint);
	
public:
	double m_MoveAverge;
	double   m_Startpoint;

private:
	enum DIRECTION {NONE, HIGH, LOW} m_lastDirection;
	double	m_PrevHighValue, m_PrevLowValue, m_ChangeValue;
	double m_Min;
	double m_Max;
	CPacket* m_pZigDataPacket;
	CPacket* m_pZigIndexPacket;
	CPacket* m_pEndPacket;
	CList<double, double>* m_pEndPriceList;
};

//******************************************************************************************************
//������ ��Ʈ...
//2002,9,11 SJ
//******************************************************************************************************
class CCLC : public CIndicator
{
public:
	CCLC();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo);

private:
	void GetCLC(const CPoint& point, double dCondition1, double dCondition2,
			  	  CList<double, double>* pEndPriceList, CPacket* pCLCDataPacket,
				  CPacket* pCLCIndexPacket);
	double MakeChangHighValue(double prevValue, double dCondition2, double dCondition1,
		CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	double MakeChangLowValue(double prevValue, double dCondition2, double dCondition1,
		CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	bool ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, 
					 CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList);
	bool SetCLCIndexList(double nPrevIndex, CList<double, double>* pEndPriceList, 
						 CList<double, double>& dZigzagIndexList, 
						 CList<double, double>* pCLCIndexList, CList<double, double>* pCLCDataList);

public:
	void CalculateRatio(const CPoint& point, CList<double, double>& dZigzagList, CList<double, double>* pEndPriceList);
	double m_step;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_nType;

private:
	enum DIRECTION {NONE, HIGH, LOW} m_lastDirection;
	double	m_PrevHighValue;
	double  m_PrevLowValue;
	CPacket* m_pCLCDataPacket;
	CPacket* m_pCLCIndexPacket;
	CPacket* m_pEndPacket;
	CList<double, double>* m_pEndPriceList;
};

///////////////////////////////////////////////////////////////////////////////
//�ż��ŵ� ���Ź���
////////////////////////////////////////////////////////////////////////////////
class CSellBuyHorizontalBar : public CIndicator  
{
public:
	CSellBuyHorizontalBar();

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
					CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo,
					CCalculateData& calculateData);
protected:
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;
	CList<double, double>* m_pBuyVolumeList;	//�ż��ŷ���
	CList<double, double>* m_pSellVolumeList;	//�ŵ� �ŷ���

	CPacket* m_pHorZonPacket;
	CPacket* m_pHorZonPricePacket;
	CPacket* m_pHorZonBuyPacket;
	CPacket* m_pHorZonSellPacket;

// (2008/4/23 - Seung-Won, Bae) Make the price unit double
	double	m_dHoga;
protected:
	int		m_StartPoint;	// ���Ź� ���� ������ ���� Index�̴�.
	int		m_EndPoint;		// ���Ź� ���� ������ �� Index�̴�.
	double	m_Max;			// ���Ź� ���� ������ �ְ� �����̴�.
	double	m_Min;			// ���Ź� ���� ������ ���� �����̴�.

private:
	void GetSellBuyHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,CList<double, double>* pSellVolumeList,
				CList<double, double>* pBuyVolumeList,double dConditionType,double dHoga);
};

// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
#define _MA_COUNT	20
class CMA : public CIndicator
{
typedef CList<double, double> CDoublePacketData;

public:
	CMA( IMetaTable::EDDC_INDICATOR_ID p_eID);
	virtual ~CMA();

	virtual void	GetData(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo,
							 CCalculateData& calculateData);
	inline void		GetMA(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo,
							 CCalculateData& calculateData);

protected:
	void	GetSimpleMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetExponentialMA(	const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetWeightedMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetGeometricMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetHarmonicMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetTriangularMA(	const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetAdaptiveMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);

// (2010/4/15 - Seung-Won, Bae) Manage Environment State.
protected:
	double						m_daMAPeriod[ _MA_COUNT];
	int							m_naMAType[ _MA_COUNT];
	CPacket *					m_ppaMAPacket[ _MA_COUNT];
	CDoublePacketData *			m_pdlaSourceList[ _MA_COUNT];
	int							m_naInputPacketType[ _MA_COUNT];	// 1: Normal, 2:HL/2, 3:HLC/3
	BOOL						m_bShow[ _MA_COUNT];
	CDoublePacketData *			m_pEndList;
	CDoublePacketData *			m_pHighList;
	CDoublePacketData *			m_pLowList;
	CStringArray				m_saSubGraphName;
	CList< double, double>		m_dlDoubleList1[ _MA_COUNT];
	CList< double, double>		m_dlDoubleList2[ _MA_COUNT];

// (2010/4/19 - Seung-Won, Bae) for Sub Graph Iterating.
protected:
	int									m_nSubGraphIndex;
	CCalculateBaseData::CALCULATETYPE	m_eCalcType;
	CPacketList *						m_pPacketList;

protected:
	inline void		SetMAValue( const int &p_nStartIndex, const int &nUpdateIndex, const double &dMA, CPacket *pMAPacket);

// (2010/4/19 - Seung-Won, Bae) for Signal Sub Graph Index.
protected:
	int							m_nSignalMA1;
	int							m_nSignalMA2;

// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
protected:
	int		m_nFirstVisibleSubGraph;
public:
	int		GetFirstVisibleSubGraph( void)		{	return m_nFirstVisibleSubGraph;	}
};

class CNoCalculate : public CIndicator  
{
public:
	CNoCalculate( IMetaTable::EDDC_INDICATOR_ID p_eID) : CIndicator( p_eID)	{}

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool InCaseNoPacketPointer(const CChartFormulateData& chartFormulateData, 
		const int nSubGraphCount, const int nSubGraphIndex, CSubGraphPacketList* pSubGraphList);
	CString GetPacketNames(const CString& strOrgPacketNames, const int nSubGraphCount, const int nSubGraphIndex) const;

	void SetGraphPacketList(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData);
	//�̴�����
	void CalculateBalancePacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData);
	void CalExCeptionPacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData);	
};

#endif // !defined(AFX_INDICATORFACTORY_H__1F4A506F_3636_46B2_844F_B049AA6D1A3A__INCLUDED_)
