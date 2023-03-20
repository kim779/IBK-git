// SiteCustomAddin.h: interface for the CSiteCustomAddin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_)
#define AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_

// (2004.10.08, ��¿�) Site�� Custom AddIn Object�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/I139092/_ChartAddInBase.h"		// for CChartAddInBase
#include "./Include_AddIn/I139092/_IChartOcx.h"				// for IChartOCX;
#include "./Include_AddIn/I139092/_AddInCommand.h"

#include "WndAssistIndexManager.h"
#include "../../inc/ISTAddinManager.h"						// for ISTAddinData
#define DATAOPTION_TR	0
#define DATAOPTION_REAL	1

// RQ�� ���� : �������� - ojtaso (20070305)
#include <map>
#include <string>

// RQ�� ���� : �������� - ojtaso (20070305)
typedef std::map<std::string, CWndAssistIndexManager*>		MAP_WNDASSISTINDEXMANAGER;

class CSiteCustomAddin : public CChartAddInBase
{
public:
	CSiteCustomAddin( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CSiteCustomAddin();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

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

// *******************************************************************************
// Pattern Maker
protected:
//	CWndAssistIndexManager m_assistIndexManager;
// ********************************************************************************

public:
	LPCTSTR m_lpszRQ;		// ���� RQ : �������� - ojtaso (20070305)
	MAP_WNDASSISTINDEXMANAGER	m_mapWndAssistIndexManager; // RQ�� ���� : �������� - ojtaso (20070305)

// (2004.10.15, ��¿�) Real ���� Event�� �����Ѵ�.
protected:
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	// virtual void	OnDrawBeforeGraph( CDC *p_pDC);
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	//virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam);

// (2004.10.22, ��¿�) AddIn DLL���� Mouse Event�� �����Ѵ�.
public:
	// JSJ_ADD_50119__��Ʈ���� InvokeAddIn�Լ� ȣ����
	 virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);	
protected:
	// [06/12/22] �ʱ�ȭ �̺�Ʈ�� ���� ó��
	virtual void	OnResetChart( void);
	// [04/10/13] TR�� ������ AddIn DLL�鿡�� �˷� ������� �Ѵ�.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);
	// [04/12/07] TR�̳� Real�� ���ſ� ���� Packet Data�� Update�Ǿ����� AddIn DLL�鿡 �˷� ��ǥ���� ó����� �Ѵ�.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);

protected:
	// ��ǥ�� ����Ѵ�.(��ǥ�߰�, ��ǥ����)
	// (2006/1/9 - Seung-Won, Bae) Update for Independence of Interface Version Number.
	//			~ CObject * <= const CList<CPacket*, CPacket*> *
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	virtual BOOL CalculateIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine, CObject *p_plCPacket);
	// ��ǥ�� �����Ѵ�.
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	virtual BOOL DeleteIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine);
	virtual BOOL CalculateAll(const int nDataOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);	//JS.Kim_20100908 ����Ÿ�Ǽ� �ʰ��� �����Ҷ� ��ǥ �ݿ��ȵǴ� ���� ����
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam);

	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	// �������� �߰�/���� : �������� - ojtaso (20070302)
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);

protected:
	// virtual BOOL	OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	void GetAssistIndexManager(CWndAssistIndexManager*& pAssistIndexManager, LPCTSTR lpszRQ = NULL);

public:
	class CSTAddinData : public ISTAddinData
	{
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwDataVolume, LONG& dwDataExt);
	}m_xSTAddinData;
	friend class CSTAddinData;
};

#endif // !defined(AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_)
