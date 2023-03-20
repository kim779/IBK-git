// PacketListModule.h: interface for the CPacketListModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLISTMODULE_H__3D5B4B26_3ADC_43FB_81DF_D419F380923E__INCLUDED_)
#define AFX_PACKETLISTMODULE_H__3D5B4B26_3ADC_43FB_81DF_D419F380923E__INCLUDED_

#include "PacketBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseRealDataModule;

// (2009/10/20 - Seung-Won, Bae) Comment.
//		�ǽð� ���Ű��� ������ ó���ϴ� Module.
//		���� �ŷ����� �޾� ó���ϴ� �⺻�ŷ����� �ŷ���� ��ɵ� �̰������ϸ� ���� ���ѵ�...
//		Type�� �ξ� Logic�� ��ü�ϵ��� �Ǿ� �ִµ�, Type�� �ΰ� �̻� �� �� �ִ� ������ �ƴϴ�.
//			(Packet ���� �ǽð� ���Žø��� �ٲ� �� �ִ� ���� �ƴ�)
//		����, �׽� �⺻������ CRealDataProcessor�� �ΰ�, ���������� Packet���� ������ ���� Logic Class�� ������� ó���ϴ� ���� ���ڴ�.
//			�翬 Logic Class�� ��ϵ��� ���� ���, ���� �״�θ� �����ϴ� ������.
//		Ư��, �ش� Logic �˻��� Lookup�� ������ ������ �ʿ��ϴ�.
//		���� �پ��� Data ���� Logic�� Ȯ�强�� ���� ���� �ʿ��ϳ� ���� ��ȸ��...
class CRealDataModule  
{
public:
	enum REALDATAMODULETYPE
	{
		BASE_TYPE = 0,
		RATE_TYPE
	};

public:
	CRealDataModule( HWND p_hOcxWnd, const REALDATAMODULETYPE eRealDataModuleType = BASE_TYPE);
	virtual ~CRealDataModule();


// (2009/10/20 - Seung-Won, Bae) Comment.
protected:
											//	���� ����ϴ� �������� 1�̰ų� 0���� Ȯ���Ѵ�. (���� ����� �ǹ̰� ���� ���)
											//	���� ��길�� ���� ���ΰ�? �׸��� ù RDC�� ER�� ������ Ȯ���ϸ� �� �ڴ� ������ ����?
	CRealDataModule::REALDATAMODULETYPE		GetMakeRealModuleType( const REALDATAMODULETYPE eRealDataModuleType, const CString& strData) const;







public:
	void SetRealDataModuleType( const REALDATAMODULETYPE eRealDataModuleType, const CString& strModuleHelpMsg);
	bool SetRealDataModuleType( const REALDATAMODULETYPE eRealDataModuleType);

	void ChangeExchangeRateData_Percent();
	bool ChangeRealDataModuleAndData(const CString& strModuleHelpMsg);

	double GetRealData(const CString& strPacketName, const double& dData) const;
	double GetRealData(const CString& strPacketName, const CString& strData) const;
	// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
	double GetOrgData(const CString& strPacketName, const double& dData) const;
	
private:
	void MakeRealDataModule(const REALDATAMODULETYPE eRealDataModuleType);
	void DeleteRealDataModule();


private:
	CBaseRealDataModule *m_pRealDataModule;

// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
protected:
	HWND		m_hOcxWnd;
};


// real ���� Module
class CBaseRealDataModule
{
public:
	virtual ~CBaseRealDataModule()	{};
public:
	virtual CRealDataModule::REALDATAMODULETYPE GetRealDataModuleType() const = 0;
	virtual double GetRealData(const CString& strPacketName, const double& dData) const = 0;
	// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
	virtual double GetOrgData(const CString& strPacketName, const double& dData) const = 0;

	virtual void SetHelpMessage(const CString& strData, const bool bIsRemove = true) = 0;

	virtual void ChangeExchangeRateData_Percent() = 0;
};


// org real ���� Module
class COrgRealDataModule : public CBaseRealDataModule
{
public:
	virtual CRealDataModule::REALDATAMODULETYPE GetRealDataModuleType() const;
	virtual double GetRealData(const CString& strPacketName, const double& dData) const;
	// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
	virtual double GetOrgData(const CString& strPacketName, const double& dData) const;

	virtual void SetHelpMessage(const CString& strData, const bool bIsRemove = true);

	virtual void ChangeExchangeRateData_Percent();
};


// ���� real ���� Module
class CRateRealDataModule : public CBaseRealDataModule
{
public:
	CRateRealDataModule( HWND p_hOcxWnd);
	virtual ~CRateRealDataModule();

public:
	virtual CRealDataModule::REALDATAMODULETYPE GetRealDataModuleType() const;
	virtual double GetRealData(const CString& strPacketName, const double& dData) const;
	// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
	virtual double GetOrgData(const CString& strPacketName, const double& dData) const;

	virtual void SetHelpMessage(const CString& strData, const bool bIsRemove = true);

	virtual void ChangeExchangeRateData_Percent();

private:
	void MakeRealRateData(const CString& strData);
	void DeleteAllRealRateData();

	// (2006/6/20 - Seung-Won, Bae) Kim Sun-Young Update! for Real data with RDC Help Message
	void SetRealRateData(const CString& strData);
	void SetRealRateData_PartChange(const CString& strData);

	bool DoesOnlyOriginalRealData(const CString& strPacketName) const;
	CString GetRealRateData(CString& strAllData, const CString& strCompart) const;

private:
	CList<CRealRateData*, CRealRateData*> m_realReteDataList;

// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
protected:
	HWND		m_hOcxWnd;
};

#endif // !defined(AFX_PACKETLISTMODULE_H__3D5B4B26_3ADC_43FB_81DF_D419F380923E__INCLUDED_)

