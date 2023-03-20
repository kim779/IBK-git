// SiteCustomAddin.h: interface for the CSiteCustomAddin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_)
#define AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_

// (2004.10.08, ��¿�) Site�� Custom AddIn Object�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./include_Addin/I423611/_ChartAddInBase.h"	// for CChartAddInBase
#include "./include_Addin/I423611/_IChartOcx.h"		// for IChartOCX;
#include "./include_Addin/I423611/_AddInCommand.h"		// for DECLARE_ADDIN_EVENT_MAP()

#include "PatternMaker.h"													// for CPatternMaker
#include "Include/DrdsDef.h"												// for Real Packet(Tr/Drds)
// 2008.02.20 by LYH >> 7.0 -> 6.0
#include <iostream>
// 2008.02.20 by LYH <<
// RQ�� ���� : �������� - ojtaso (20070305)
using namespace std;

#include <map>
#include <string>

// RQ�� ���� : �������� - ojtaso (20070305)
typedef map<string, CPatternMaker*>		MAP_PATTERNMAKER;

class CSiteCustomAddin : public CChartAddInBase
{
public:
	CSiteCustomAddin( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive);
	virtual ~CSiteCustomAddin();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()


// =========================================
// Pattern Maker
// =========================================
public:
	//CPatternMaker		m_objPatternMaker;
public:
	LPCTSTR m_lpszRQ;		// ���� RQ : �������� - ojtaso (20070305)
	MAP_PATTERNMAKER	m_mapPatternMaker; // RQ�� ���� : �������� - ojtaso (20070305)


// Real Data
public:
	BOOL			m_bRecvRealData_S31;

	//BOOL			m_bBlock;
	//double		m_dCurPrice;
	//double		m_dOpen;
	//double		m_dHigh;
	//double		m_dLow;



// (2004.10.22, ��¿�) AddIn DLL���� Mouse Event�� �����Ѵ�.
//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
public:
	// JSJ_ADD_50119__��Ʈ���� InvokeAddIn�Լ� ȣ����
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);


// (2004.10.15, ��¿�) Real ���� Event�� �����Ѵ�.
protected:
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	virtual void	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam);
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam);
	// (2004.10.11, ��¿�) �⺻ Event�� �����Ѵ�. (�ʿ� ���� ��� �������� �ʴ´�. Base���� �����ش�.)
protected:
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( CDC *p_pDC);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnLButtonDown(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnMouseMove(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	virtual void	OnResetChart();	//��Ʈ�ʱ�ȭ 2006.12.26 by LYH

	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	//virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam);
	//virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	// �������� �߰�/���� : �������� - ojtaso (20070302)
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);

	void GetPatternMaker(CPatternMaker*& pPatternMaker, LPCTSTR lpszRQ = NULL);
// 2008.02.20 by LYH >>
protected:
	HWND hOcxWnd;
// 2008.02.20 by LYH <<
};

#endif // !defined(AFX_SITECUSTOMADDIN_H__4DCF1075_E1B5_48E4_9C3B_FECE5FDB70A1__INCLUDED_)
