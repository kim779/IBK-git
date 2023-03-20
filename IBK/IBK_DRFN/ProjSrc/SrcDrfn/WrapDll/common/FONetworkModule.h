// FONetworkModule.h: interface for the CFONetworkModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONETWORKMODULE_H__DA552A90_F563_4171_9EFE_335D96612D0E__INCLUDED_)
#define AFX_FONETWORKMODULE_H__DA552A90_F563_4171_9EFE_335D96612D0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dataset.h"

// class CCommonFrameWorkWnd;
class CBaseNetworkModule;
class CNetworkModule;


class CFONetworkModule  
{
public:
	CFONetworkModule(CWnd* pWndMainFrame);
	CFONetworkModule(CWnd * pWnd, CWnd* pWndMainFrame);
	virtual ~CFONetworkModule();

	//////////////////////////////////////////////////////////////////////////
	// �輺�� �߰� 2003.02.06
protected:
	char m_RankBit;								// ���� ������ ������ ���� ������

	int m_iPriceProgressRequestCount;			// �������� ��û ����

	//////////////////////////////////////////////////////////////////////////
	BOOL m_bCommUseBaseModule;
public:

	//20080308 �̹��� >>
	CPriceProgressInputList		m_PriceProgressInputList;
	//20080308 �̹��� <<

	//20080301 �̹��� >>
	CTotalIVDataArray			m_TotalIVArray;
	//20080301 �̹��� <<

	//20080304 �̹��� >>
	CIV4ItemArray				m_IVItemArray; 
	//20080304 �̹��� <<

	CIVPastData					m_IVPastData;

	COptionIVMonthSet			m_OptionIVMonth;

	CAverageIV					m_AverageIV;

	CMarginInfo	*				m_pMarginInfo;


	CItemInfoArray				m_FutureMonthInfoArray;

	//�����ڻ� �迭.
	CUnderlyingAssetsInfoArray	m_UnderlyingAssetsInfoArray;

	//������ �ɼ����簡 �������..
	COptionMonthInfo *		m_pMonthOpt;


	//���űݿ� �̷а� ����..
	CTheoryForMarginArray	m_TheoryForMarginArray;


	//���¸��� ������ Ŭ����
	CAccount		m_AccountName;


	//�����ɼǰ� �ֽİ��������� �ִ�.
	CAccountInfo	m_AccountInfo; 



	//�ܰ�/�Ϲݽü� ���..(ACCOUNT_INFO, SISE_INFO, ..)
	int		m_nRequestDataType;



	//���� ��û�� ��ȸ�����͸� �ǽð����� ����� ������ ����.
	BOOL	m_bRTData;



	//�ش� ��ü�� ������Ų �θ������� ������.
	CWnd * m_pParentWnd;



	//�⺻ ������ ��Ÿ�� ������ -> ������ ����� �����͸� ����.
	//(�� ��ü���� �����ϴ� ���� �ƴϴ�)
	CBaseNetworkModule * m_pBaseWnd;



	//��ü���� �ϳ��� ����(���� ������ ����ϴ� ���)
	//�� ����� ������ �ٲ� ������ �ٲ��� �Ѵ�.
	CNetworkModule *	 m_pNetwork;



	//��ȸ�ϰ� ���� �����͵��� ����.
	CDataArray			 m_ReceiveDataArray;



	//��ȸ�� ������� �����ϰ� �ִ� ����.
	//��ȸ�� ������ �� ���۴� ����־�� �Ѵ�.
	//CStringArray		 m_strRqCodeArray;
	CDataArray			 m_RqCodeArray;



	//�ǽð� ����...
	CDataArray			 m_RTDataArray;

	//�ǽð� �ü� �����͵��� �̸� �����ϴ� ����.
	CStringArray		 m_strAddRTDataArray;
	
	CWnd*				m_pWndMainFrame;

public:
	CString GetLastErrorMsg();
	CString GetLastMsg();
	void ResetKey();
	void SendBackKey(CString strCode);

	CString Parser(CString &strSrc, CString strSub);

	int SendReqFundID(CString strConti, CString strContiFlg);

	int RequestIVPastData(CIVPastInputData & IVPastInputData);

	int RequestOptionPastIV(CString strEndDate);

	int RequestAverageIV(CString strUnderlyingAssetsCode);

	int RequestMarginInfo(CString strAccount, CString strPassword);

	//20080228 �̹��� >>
	int RequestTotIV(CString strBaseCode, char chJanFlag, int nTotWorkDateCnt, 
					   CString strStartTime, CString strEndTime);
	//20080228 �̹��� <<
	
	//20080304 �̹��� >>
	int RequestIVDate(CDataArray & DataArray);
	//20080304 �̹��� <<

	int RequestTodayAverageIV(CString strInput);; // �ϳ�������

	int RequestDeltaSum(void* pInput);				// ��Ÿ ���� ������ ��û

	BOOL FindDataSet(CDataSet * pDataSet);

	void SetUnderlyingAssetsArray();

	//�����ɼ� ������ ��û...
	int RequestMonthData(CString strUnderlyingAssetsCode, CString strYM);

	//////////////////////////////////////////////////////////////////////////
	// �輺�� �߰� 2003.02.06

	// ��������(����&�ɼ�&�ֽĿɼ�&�ֽ�)
	int RequestPriceProgressData(CStringArray &sarrCode, LPCTSTR pszDate, LPCTSTR pszTime, 
		int iDataCount, char DataBit);

	//////////////////////////////////////////////////////////////////////////

	//���űݿ� �̷а� ��û.
	//int RequestTheoryForMargin(CStringArray & strCodeArray);
	int RequestTheoryForMargin(CDataArray & DataArray);

	/* �Էµ� ���¿� ���ؼ��� �ܰ� �ҷ����� �Լ� */
	int RequestAccountData(CString strAccount, CString strPassword);

	//�ܰ��û..
	int RequestData(CString strAccount, CString strPassword);

    int RequestData(CString strOrgSect, CString strOrgCd, CString strFundId, CString strSavekey);

	//�ü���û..
	int RequestData(CDataSet * pDataSet, BOOL bRTData = TRUE);

	int RequestData(CDataArray	& DataArray, BOOL bRTData = TRUE);

	//�ü� �ޱ�..
	int ReceiveData(WPARAM wParam, LPARAM lParam);

	//�ǽð� �ü� ���
	BOOL AddRTData(CDataSet * pSet);

	//�ǽð� �ü� �ޱ� ���� ���....
	BOOL AddRTData(CDataArray & DataArray);

	//�ǽð� �����ϴ� ���� �ϳ��� ������ ���..
	BOOL DelRTData(CString strCode);

	//�ǽð� �����Ѵ� ���񸮽�Ʈ�� ����...
	BOOL DelRTData(CStringArray strCodeArray);

	//�ǽð� �����ϴ� ��� ���񸮽�Ʈ�� ����...
	BOOL DelAllRTData();

	//�ǽð� �����ϴ� �� ���� id array�� ����� �κ�.
//	BOOL DelRTData(CDataSet * pSet);

	//�ǽð� �����ϴ� ���������� ���� id array�� ����� �κ�.
	BOOL DelRTData(CDataArray & DataArray);

	//�ǽð� �ü� �޴� �Լ�..
	BOOL ReceiveRTData(WPARAM wParam, LPARAM lParam);

	BOOL IsCommUseBaseModule()		{ return m_bCommUseBaseModule; };
};
typedef CArray < CFONetworkModule * , CFONetworkModule *> CFONetworkArray;


#endif // !defined(AFX_FONETWORKMODULE_H__DA552A90_F563_4171_9EFE_335D96612D0E__INCLUDED_)
