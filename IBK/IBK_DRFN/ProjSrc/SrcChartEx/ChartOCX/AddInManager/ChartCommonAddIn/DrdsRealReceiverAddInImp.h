// DrdsRealReceiverAddInImp.h: interface for the CDrdsRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRDSREALRECEIVERADDINIMP_H__7BB74194_3BD9_473E_8610_381F7D82E35F__INCLUDED_)
#define AFX_DRDSREALRECEIVERADDINIMP_H__7BB74194_3BD9_473E_8610_381F7D82E35F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include/LoadDrds.h"						// for CDrdsLoadLib
#include "RealReceiverAddInImp.h"						// for CRealReceiverAddInImp

class CRealSettingInfoList;
class CDrdsAdvisedInfoList;
class CXScaleManager;				// XScaleManager �߰� : �������� - ojtaso (20070118)
class CDrdsRealReceiverAddInImp : public CRealReceiverAddInImp
{
public:
	CDrdsRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CDrdsRealReceiverAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;


// (2006/7/25 - Seung-Won, Bae) Support Real Requesting
protected:
	// 1. XFM�� Script���� �����Ǵ� Real ���� �Ӽ� String Data�� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	ResetRealSettingInfo( const char *p_szRealSettingInfo);
	// 2. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
	//		Ư��, p_pstrShareName�� p_pstrSharedData�� �־����� ��쿡�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
	//			p_szRealSettingItem���� Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	//		(ResetRealSettingInfo()�� AddRealItem()�� ������ �� �ֵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	virtual BOOL	AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, CString *p_pstrSharedData = NULL, char p_cSeparator = '.');
	// 2. ������ Method(RemoveRealItem)�� ���Ͽ� �����Ǵ� 1���� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
	//		Ư��, Local Share Name�� ���� �ٷ� Real Key Code�� �־���,
	//			������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�ؾ� �Ѵ�.
	//		����� ���̻� �ش� ���� Key�� ������ Real�� ���� ��쿡�� �������� ��ȯ�ȴ�.
	//		(AddRealSettingInfo()�� Interface�� �����ϵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	virtual BOOL	RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, char p_cSeparator = '.');
protected:
	virtual	BOOL	SetRealAdvise( BOOL p_bAdvise);
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	virtual	BOOL	SetRealAdviseRQ(LPCTSTR lpszRQ, BOOL p_bAdvise);
	virtual BOOL	GetRealAdvisedInfo( CString &p_strAdvisedInfo);

// (2006/7/25 - Seung-Won, Bae) Support DRDS Interface
protected:
	virtual BOOL	SetDrdsData( UINT wParam, LPARAM lParam);
	virtual BOOL	SetDrdsPacketEnd( UINT wParam, LPARAM lParam);

protected:
	// RQ�� Real���� : �������� - ojtaso (20070111)
	virtual BOOL	AddRealSettingInfoRQ(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator);
	virtual BOOL	RemoveRealSettingInfoRQ(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator);
	// RQ�� Packet ���� : �������� - ojtaso (20070122)
	virtual void	RemoveRQPacket(LPCTSTR lpszRQ);

//////////////////////////////////////////////////////////////////////
// (2004.11.17, ��¿�, �м�) Real ���� Interface�̴�.
//		Real�� ó���� ���ؼ��� �⺻������ 3���� �ڷᱸ���� �̿�ȴ�.
//		�̰��� 3���� ������ ��ȣ ������ ���� ���̱� �����̸�,
//			��쿡 ���� �� 3���� �ڷᱸ���� ����ġ�ϰ�, 
//			�̸� �����ϰ� ����� ������ ������ �ʿ��ϱ� �����̴�.
//	1. �ڷᱸ��
//		1.1 DRDS Advised Info
//			- ���� DRDS�� Advise�� ������ �������� �����ϴ� �ڷᱸ���̴�.
//			- On/OffDrdsAdvise()���� Real Setting Info�� Local Share Info�� �����Ͽ�
//				�������� Advise�� ó���Ǹ鼭 �����ȴ�.
//			- Data :
//					ID (CPacket Object Pointer)
//					Real Packet Name
//					Real Field Name
//					Real Key Code
//			- �� 4���� Data�� ��� Key�� �Ǹ�, ��� ���� ��쿡�� �ߺ� Advise�� ���� �ʴ´�.
//				(��� Key�� �� ���Ƶ� Advise���� FALSE�� Return���� �ʴ´�.)
//		1.2 Real Setting Info
//			- Real�� ��û�ϱ� ���� �ڷᱸ���̴�.
//			- XFM�� ���������� ���� Real Key Code ��� Local Share Name�� �����Ѵ�.
//			- Method�� ���Ͽ� ���� Real Setting Info�� �����Ͽ��� �ٷ� Real ��û�� �ݿ����� �ʰ�,
//				OnDrdsAdvise() ó���ÿ��� Real�� �ݿ��Ѵ�. (��� ó���� �Ӹ� ��������.)
//			- Data :
//					TR Packet Name (CPacket Object Pointer)
//					Real Packet Name
//					Real Field Name
//					Local Share Name
//			- �� 4���� Data�� ��� Key�� �Ǹ�, ��� ���� ��쿡�� �ߺ� �������� �ʴ´�.
//		1.3 Local Share Info
//			- Real�� ��û�ϱ� ����, Key Code�� �����ϴ� �ڷᱸ���̴�.
//			- XFM�� ���������� ���� Local Share Name�� �� �Ӽ��� �����Ѵ�.
//			- Local �������� ����Ǿ Real�� ���� �������� �ʰ�,
//				OnDrdsAdvise() ó���ÿ��� Real�� �ݿ��Ѵ�. (��� ó���� �Ӹ� ��������.)
//			-  Data :
//					Local Share Name
//					Local Shared Data
//					Start	Share Flag
//					Set		Share Flag
//					Get		Share Flag
//			- Local Share Name�� Key�̸�, �̸� �������� Real ��û�� �����ȴ�.
//	2. ����
//		2.1 SetAllProperties2()���� XFM�� Real ������ m_strRealList�� �м��Ͽ� Real Setting Info�� �����Ѵ�.
//			(�̷��� ������ Real Setting Info�� Destructor������ ���ŵȴ�.)
//		2.2 �̾�, InitLSharedData()�� ���� Real�� ��û�ϱ����� �ʿ��� Real Key Code�� Local Share Info�� �����Ѵ�.
//		2.3 Map Loading�� �ǰ�, �ʱ� Local ������ ���޵Ǿ� �����ȴ�. (Start Flag�� �ƴϾ Start ȿ���� ����)
//		2.4 TR�� ���ŵǾ� Real�� Off/On�Ǹ鼭 ������ Real Setting Info�� Local Share Info�� �������� Real�� ��û�ϰ� �ȴ�.
//		2.5 OnDrdsData()�� OnDrdsPacketEnd()�� ���ؼ� Real Data�� ó���ϰ� �ȴ�.
//				Ư�� OnDrdsPacketEnd()�� ó�������� m_bRedrawAfterUpdatedAllPackets�� ���� ���� ��û�� Real Packet��
//					��� 1ȸ �̻� ���ŵ� ��쿡�� Drawing�� ��Ÿ Real ó�� ������ �۾��� �����ϰ� �ȴ�.
//		2.6 �̷��� Real�� ���ŵǴ� ���߿� ������ ����ǰų�, Real ������ ����Ǹ�,
//				Local Share Info�� Real Setting Info�� ����ȴ�. (��������� Real ��û�� �ݿ������� �ʴ´�.)
//		2.7 Off/OnDrdsAdvise()�� ���Ͽ� ������ ������ Real ������ ���� Real�� �ٽ� ��û�Ѵ�.
//				(�̶� Real�� �ϰ� ���û�Ǹ�, DRDS Advised Info�� �Բ� ���ŵȴ�.)
//		2.8 ����� Window�� ���ŵǸ鼭(OnDestroyWindow) OffDrdsAdvise()�� ȣ��� Real ������ ������.
//		2.* ����! Real Setting Info�� Local Shared Data�� ����Ǿ ��� Real Advise���� �������� �ʴ´�.
//				�׻� OnDrdsAdvise()�� ���Ͽ��� ����ǵ��� �Ѵ�.
//				Ư�� XFM�� �������� �ʰ�, ��������� Real�� ������ �����ϴ� ��쿡��
//				OnDrdsAdvise()�� ���ؼ��� ����ǵ��� �Ѵ�. �ȱ׷� �ʹ� �Ӹ� ������.
//		2.* ����! Method�� ���� Real Setting Info�� Local Share Info�� ���� �� ���� String��
//				Map�� ���� String�� Parsing Routine�� �����ϵ��� Format�� �����Ѵ�. (�����ڸ� ','���� '.'���� �����Ѵ�.)
//	3. �����
//		3.1 ������� ���Ͽ��� m_bNormalizedMinuteTimeRule Flag���� ������� �����ϴ� ��Ʈ������ �����Ѵ�.
//		3.2 m_bNormalizedMinuteTimeRule�� ���� Data Folder�� Config.cfg�� [CHART]�� NMTR ���� 0 or 1�� �� OCX�� �⺻���� �ǰ�,
//				���� ������ Property �������� ���� ������ �����ϴ�.
//		3.3 ���� ������� Real�� ó���������� m_bOnNMTR�� ���� �����Ѵ�. (��/��/��/ƽ�� ���ܵȴ�.)
//		3.4 OnDrdsPacketEnd()���� m_bRedrawAfterUpdatedAllPackets�� ���� ���� ��û�� Real Packet��
//				��� 1ȸ �̻� ���ŵ� ��쿡�� Drawing�ϰ� �Ǵµ�, �̶� ������� ���� �ð� ���� Packet��
//				���� ��󿡼� ���ܵȴ�. (�׻� ���ŵǾ� �ִ� ���·� �����ϴ� ����)
//		3.5 ���� m_nTimeDataCount��� �ð� Data ���� ������ �����Ͽ�,
//				OnDrdsPacketEnd()������ ��û�� Real Packet�� ��� 1ȸ �̻� ���ŵ� ���ܿ�,
//				������� ���� �ð� ���� Packet�� ���� �ð� ������ �����ϴ� ��쿡�� �ٷ� �ݿ��ǵ��� �Ѵ�.
//				�� ���, ���ŵ��� ���� Packet�� �ݿ��ǹǷ� ��� Packet�� Data���� ���� �ʴ� ��찡 �߻��Ѵ�.
//////////////////////////////////////////////////////////////////////
// DRDS DLL Data
protected:
	void *				m_DrdsVoid;					// DRDS Client ID
	CDrdsLoadLib		m_DrdsDLL;					// DRDS DLL Helper(Interface) Class Object�̴�.
// Real On/Off Routine
protected:
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	BOOL	OffDrdsAdvise(LPCTSTR lpszRQ = NULL);	
	BOOL	OnDrdsAdvise(LPCTSTR lpszRQ = NULL);

	// RQ��� : �������� - ojtaso (20070111)
	LPCSTR GetDrdsAdvisedRQ(LPVOID p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode);

// Real Setting Info
protected:
	CRealSettingInfoList *	m_plRealSettingInfoList;	// Real Setting Info�� �����ϴ� Data Object�̴�.
// DRDS Advised Info
protected:	// DRDS Advised Info
	CDrdsAdvisedInfoList *	m_plDrdsAdvisedInfoList;	// DRDS Advised Info�� �����ϴ� Data Object�̴�.
// ����� ���� ���� Data
protected:
//	BOOL	m_bNormalizedMinuteTimeRule;	// (2004.11.26, ��¿�) Real�� ����Ʈ�� ����������� ��ü �ð��� ������ �������� Flag���� ���� �����Ѵ�.
										//		Ư��, ������ Global�� �Ӽ��� �ξ� HTS �⺻ ������ �����ϰ�,
										//		������ Property�� �����Ͽ� Chart�� �Ӽ��� ������� �Ѵ�.
	BOOL	m_bOnNMTR;					// (2004.12.01, ��¿�) OnRealAdvise()�� Real�� ���۵Ǹ鼭 ������� ó�����ϴ� �������� Flag���� �����Ѵ�.
										//		OnRealAdvse()/OnRealData()/OffRealAdvise()������ �̿�ȴ�.
// ����� ���� ���� Data
protected:
	int		m_nTimeDataCount;			// (2004.12.01, ��¿�) ������� ��Ȯ�� ������ ������ Ȯ���ϱ� ���Ͽ�,
										//		���� �ڷ������� Data ������ �����Ѵ�.

	// XScaleManager �߰� : �������� - ojtaso (20070118)
	CXScaleManager*		m_pXScaleManager;

// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
protected:
	virtual _int64			GetNMTRClock( void)		{	return CRealReceiverAddInImp::GetNMTRClock();	}
};

#endif // !defined(AFX_DRDSREALRECEIVERADDINIMP_H__7BB74194_3BD9_473E_8610_381F7D82E35F__INCLUDED_)
