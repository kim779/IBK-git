// PacketBaseData.h: interface for the CPacketBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETBASEDATA_H__E4CD4DBB_D4D4_4208_9E94_68873165E11F__INCLUDED_)
#define AFX_PACKETBASEDATA_H__E4CD4DBB_D4D4_4208_9E94_68873165E11F__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItem
{
public:
	CItem& operator=(const CItem& data);

public:
	CItem(const CString& strCode = "", const CString& strName = "");

	void Clear();
	void SetItem(const CString& strCode, const CString& strName);

	CString GetCode() const;
	CString GetName() const;

private:
	CString m_strCode; //�ڵ��ȣ
	CString m_strName; //�����
};


// ============================================================================
class CDateData
{
public:
	CDateData( void);

// (2009/10/20 - Seung-Won, Bae) Comment. ������ ���ڸ� �����Ѵ�. (�ŷ��� ����)
protected:
	CString		m_strPacketType;
	int			m_nDate;
	CString		m_strPacketTypedDate;
	int			m_nPacketTypedDate;
	int			m_nPacketTypeTimeFactor;				// Packet Type�� ���� �Ͻø� ������ ���ؾ� �ϴ� �� (�ð��ڸ���)
public:
				//	PC�� ���� ���ڷ� �ʱ�ȭ�Ѵ�.
				//		Contructor���� �⺻ ������.
				//		�����޽��� ������ �ʱ�ȭ ó����.
				//		���� ������ �������� ���� ���.
				//			�����޽��� DATE ��.
				//			�����޽��� DATATIME ��.
				//			�����޽��� DATATIME ������ ���� �ŷ��� ���ڸ� �������� ������� ������.
				//				�������� �����ð� 0�ø� �������� �ٲ��, �ŷ��� �ð����� ���߿� ����ȴ�.
				//				�����ð��� ���������� �������� ��/��/������ ���� ������ �ȴ�.
	void		SetDate( void);
				// (2009/10/20 - Seung-Won, Bae) Comment. ������ ���������� ���ڷ� �ʱ�ȭ�Ѵ�.
				//		�����޽��� DATE ��.
				//		�����޽��� DATATIME ��.
				//		�����޽��� DATATIME ������ ���� �ŷ��� ���ڸ� �������� ������� ������.
				//				�������� �����ð� 0�ø� �������� �ٲ��, �ŷ��� �ð����� ���߿� ����ȴ�.
				//				�����ð��� ���������� �������� ��/��/������ ���� ������ �ȴ�.
	void		SetDate( const CString& strDate); //"YYYYMMDD" ���¿��� ��!!
				// (2009/10/21 - Seung-Won, Bae) Comment. �ڷ����� PacketType�� �����޾� �ش� Format�� �´� Date�� ������ �����Ѵ�.
	void		SetPacketType( const CString& strType);
				// (2009/10/20 - Seung-Won, Bae) Comment. �ڷ����� PacketType���� ���� ���ڸ� ���Ѵ�.
	CString		GetPacketTypedDateString( void) const		{	return m_strPacketTypedDate;	}
	int			GetPacketTypedDateInt( void) const			{	return m_nPacketTypedDate;		}
				// (2009/10/20 - Seung-Won, Bae) Comment. Full Type�� ���� ���ڸ� ���Ѵ�.
	int			GetFullDate( void) const					{	return m_nDate;					}
				// Packet Type�� ���� �Ͻø� ������ ���ؾ� �ϴ� �� (�ð��ڸ���)
				//	1		: No HH ~ YYYYMM or YYYYMMDD
				//	100		: "~HH"
				//	10000	: "~HHMM"
				//	1000000	: "~HHMMSS"
	int			GetPacketTypeTimeFactor( void) const		{	return m_nPacketTypeTimeFactor;	}
};


class CTimeData
{
public:
	CTimeData& operator=(const CTimeData& data);

public:
	CTimeData(const int nTime, const bool bTimeFromServer);

	void SetTime(const int nTime, const bool bTimeFromServer);
	
	bool IsTimeFromServer() const;
	int GetTimeFromServer() const;
	bool GetTimeFromServer(int& nTime) const;
	bool GetTimeFromServer(CString& strTime) const;

private:
	int m_nTime; // HHMMSS ������ �ð��̴�.
	bool m_bTimeFromServer;
};


// (2009/10/20 - Seung-Won, Bae) Comment. ����� �� �����ð��� �����Ѵ�.
class CMarketTimeData
{
public:
	CMarketTimeData();

private:
	CTimeData m_startTime;	// ����۽ð�
	CTimeData m_endTime;	// �帶���ð�

public:
			// (2009/10/20 - Seung-Won, Bae) ����۽ð��� �����Ѵ�.
	void	SetMarketStartTime( const int nMarketStartTime, const bool bTimeFromServer = true);
			// (2009/10/20 - Seung-Won, Bae) �帶���ð��� �����Ѵ�.
	void	SetMarketEndTime( const int nMarketEndTime, const bool bTimeFromServer = true);
			// �����/���� �ð��� ��������.
			// return : true �� ���� ������ time�� ���� ��쿡 �ش�ȴ�.
	bool	GetMarketTime( int& nMarketStartTime, int& nMarketEndTime) const;
			// ����� �ð��� ���Ѵ�.
	bool	GetMarketStartTime( CString& strMarketStartTime) const;
			// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
	bool	IsReverseTime( void) const		{	return m_endTime.GetTimeFromServer() <= m_startTime.GetTimeFromServer();		}
};


// ============================================================================
class CRealRateData
{
public:
	CRealRateData& operator=(const CRealRateData& data);

public:
	CRealRateData( HWND p_hOcxWnd, const CString& strData = "");

	void SetRealRateData(const CString& strData, const CString& strCompart = ':');
	void SetRealRateData_PartChange(const CString& strData, const CString& strCompart = ':');
	void SetRealRate(const double dRealRate);
	void SetPacketNames(const CString& strPacketNames, const CString& strCompart = ',');

	bool DoesUsingRealRate() const;
	bool DoesPacketName(const CString& strPacketName) const;
	double GetRealRate() const;
	int GetRoundIndex() const;
	CList<CString, CString>* GetPacketNameList() const;

	bool GetApplyDataInRealRate(const CString& strPacketName, const double dOrgData, double& dNewData) const;
	// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
	bool GetOrgDataInRealRate(const CString& strPacketName, const double dOrgData, double& dNewData) const;

private:
	void Initialize();

	bool AddTail_PacketName(const CString& strPacketName);

	void SetRealRate(CString& strData, const CString& strCompart);
	void SetRountIndex(CString& strData, const CString& strCompart);
	void SetPacketNameList(CString& strData, const CString& strCompart);
	void SetPacketNameList(const CList<CString, CString>* pPacketNameList);

	CString GetData(CString& strData, const CString& strCompart) const;

	double GetApplyDataInRealRate(const double dOrgData) const;
	CString GetApplyDataInRealRate(const CString& strOrgData) const;
	CString GetApplyDataInRealRate(const CString& strPacketName, const CString& strOrgData) const;
	// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
	double GetOrgDataInRealRate(const double dRealRateData) const;

	bool DoesBringPacketNameList(const CString& strPacketName) const;

private:
	double m_dRealRate;	// ����
	int m_nRoundIndex;	// �ݿø��ڸ�
	CList<CString, CString> m_packetNameList;	//������ ������ ��Ŷ��

// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
protected:
	HWND		m_hOcxWnd;
};


// ============================================================================
// TR data �� ������ ������ list�� ���� (0 ���� Ŭ ��츸 ����)
class CTRDataCountList
{
public:
	void Initialize();

	bool AddTRData(const int nCount);

	int GetTotalTRIndex() const;
	int GetTotalTRCount() const;
	int GetTRCount(const int nIndex) const;
	int GetTRCount_Tail() const;
	
	int GetRequestTRCount(const int nCurTRCount) const;

private:
	int GetTRIndex(const int nCurTRCount) const;
	int GetTotalTRCount(const int nEndIndex) const;

private:
	int m_nTotalCount;
	CList<int, int> m_dataCountList;
};

// ============================================================================
class CItemData
{
public:
	CItemData& operator=(const CItemData& data);

public:
	CItemData(const bool bUsing, const double& dData);

	void Set(const bool bUsing, const double& dData);
	void SetUsing(const bool bUsing);
	void SetData(const double& dData);

	bool IsUsing() const;
	double GetData() const;

private:
	bool m_bUsing;
	double m_dData;
};

#endif // !defined(AFX_PACKETBASEDATA_H__E4CD4DBB_D4D4_4208_9E94_68873165E11F__INCLUDED_)
