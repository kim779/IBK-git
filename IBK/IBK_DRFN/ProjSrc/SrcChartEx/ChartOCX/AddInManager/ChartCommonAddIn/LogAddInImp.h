// LogAddInImp.h: interface for the CLogAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGADDINIMP_H__1C4D3A1C_AF42_4D12_83B0_9EE00F2E0932__INCLUDED_)
#define AFX_LOGADDINIMP_H__1C4D3A1C_AF42_4D12_83B0_9EE00F2E0932__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin/I000000/_ChartAddInBase.h"	// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/15
// Comments		: Loging Tool
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CDumpLogger;
class CLogAddInImp : public CChartAddInBase
{
public:
	CLogAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CLogAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
//		*. ���� ������ _IWRAPPER_IUNKNOWN_INTERFACE�� �����Ѵ�.
protected:
	virtual IUnknown *	GetAddInCustomInterface( void);
// (2005.07.08, ��¿�) AddIn Special Interface�� ������ ���Ͽ� �⺻ Interface (IUnknown��)�� �����Ѵ�.
//		*. ���� GetAddInCustomInterface()�� ��� ������ ���� ������, 
//			GetAddInCustomInterface() ������ �Բ� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2005/9/2 - Seung-Won, Bae) Manage Logger
protected:
	CDumpLogger *	m_pDumpLogger;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData); // (2005/9/2 - Seung-Won, Bae) for String Data

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/10/11] ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare);
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue);
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ);
	virtual void	OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);

// (2005/2/4 - Seung-Won, Bae) Real ���� Event�� �����Ѵ�.
protected:
	CString			m_strRealLogData;
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
protected:
	// (2005/11/21 - Seung-Won, Bae) Add General Interface
	void	OnDrdsAdvise( long p_lRequestID, const char *p_szPacket, const char *p_szField, const char *p_szAdviseKey);
	void	OffDrdsAdvise( long p_lRequestID, const char *p_szPacket, const char *p_szField, const char *p_szAdviseKey);

// (2005/9/29 - Seung-Won, Bae) Log the New Real Setting
protected:
	void	OnRealSettingChanged( const char *p_szRealSetting);

// (2005/11/7 - Seung-Won, Bae) Normalized Minute Time Rule Reset
protected:
	void	OnNormalizedMinuteTimeRuleReset( BOOL p_bNormalizedMinuteTimeRule);

// (2005/11/18 - Seung-Won, Bae) Log the Current AllProperty Info
protected:
	BOOL	LogAllProperties( void);

// (2005/11/7 - Seung-Won, Bae) Write Log Temporary Message.
protected:
	void	WriteLogMessage( const char *p_szMsg);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_LOGADDINIMP_H__1C4D3A1C_AF42_4D12_83B0_9EE00F2E0932__INCLUDED_)
