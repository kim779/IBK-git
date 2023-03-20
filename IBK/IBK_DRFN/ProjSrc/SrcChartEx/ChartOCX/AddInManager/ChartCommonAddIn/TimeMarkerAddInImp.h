// TimeMarkerAddInImp.h: interface for the CTimeMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMARKERADDINIMP_H__B6813EBB_4701_461A_A07E_03C6F985668B__INCLUDED_)
#define AFX_TIMEMARKERADDINIMP_H__B6813EBB_4701_461A_A07E_03C6F985668B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/20
// Comments		: �ð����� ǥ�ñ� Object Class�̴�.
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CTimeMarkerAddInImp : public CChartAddInBase
{
// (2004.11.22, ��¿�) Chart Manager Interface�� �����Ͽ� ó���� ������� �Ѵ�.
public:
	CTimeMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CTimeMarkerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.11.22, ��¿�, ����) �ð������� ���޹޾�, �ð�����ǥ�ø� ����ϴ� ����� �����Ѵ�.
protected:
	// 1. ���� Mark ó������ �Ͻð��� �����Ѵ�.
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long	m_lMarkTime;
	__int64	m_lMarkTime;
	// 2011.01.06 by SYS <<

	// (2004.09.14, ��¿�) ��� �Ҹ��� �����Ѵ�.
	CPen	m_pnPen;
	// (2004.07.05, ��¿�) Time Mark�� ���� �ٸ� ���񿡼��� ��ȿ�ϵ���, 
	//		Packet�� Type�� �����Ͽ�, ����� ��찡 �ƴϸ� ��� Draw�ϵ��� �Ѵ�.
	CString	m_strPacketType;
	// (2007/1/17 - Seung-Won, Bae) Manage Double Click Time Interval for LBtnDblClick with force.
	UINT	m_uDoubleClickTime;
	DWORD	m_dwLButtonTime;
	CPoint	m_ptPrevious;

protected:
	// 2. �ð� �������� ���޹޾� TimeMark�� �����Ѵ�.
	void	SetTimeMarkString( const char *p_szMarkTime, BOOL p_bWithScroll = TRUE);

// (2004.11.22, ��¿�, ����) �ð������� �߻���Ű�� Interface�� Clear ��Ű�� Interface�� �����Ѵ�.
protected:
	// (2004.06.17, ��¿�) TimeMark�� Clear��Ű�� Interface�� �����Ѵ�.
	void	ClearTimeMark( void)	{	m_lMarkTime = 0;	}
	// 3. Block�� Double Click���� �ð� ������ �߻���Ű�鼭 TimeMarkerAddInImp�� �����ϴ� Interface�� �����Ѵ�.
	void	SetTimeMarkPoint( CPoint p_ptPoint);

// (2004.11.22, ��¿�) Mouse Click�� ���� �ð����� ��� ���� ����� �����Ѵ�.
protected:
	// (2004.11.22, ��¿�) �ð� ���� ���� ����Ʈ�� �ð��࿡ ���� Data�� ��ȯ�ϴ� Interface�� �����Ѵ�.
	//		������ 0�� Return�Ѵ�.
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long	GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex);	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
	__int64	GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex);	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
	// 2011.01.06 by SYS <<
	//
	
	// (2004.11.22, ��¿�) �ð� ���� ���� �����ϴ� Routine�� �����Ͽ�, ������ �ð� �������� Ȯ���ϴ� ����� �����Ѵ�.
	BOOL	GetMarkTimeInString( CPoint p_ptPoint, CString &p_strMarkTime);
protected:
	// (2004.11.22, ��¿�) Mouse ��ġ�� �ð�����ǥ�ð� ������ ���, �̸� �ּҽ�Ų��.
	BOOL	ClearTimeMark( CPoint p_ptPoint);

// (2004.10.11, ��¿�) �ð� ���� ����� Custom AddIn���� �ű��.
protected:
	// (2004.09.30, ��¿�) �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
	//		�߽��� �������� �ʴ´�.
	BOOL		m_bEnableReceivingTimeShare;

// (2004.09.30, ��¿�) �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
//		�߽��� �������� �ʴ´�.
protected:
	void	OnEnableReceivingTimeShare (BOOL p_bEnable);

// (2004.10.11, ��¿�) TimeMark�� �����ϴ� Interface�� �����Ѵ�.
protected:
	void	OnSetTimeMark( const char *p_szMarkTime, BOOL p_bWithScroll);

// (2006/11/16 - Seung-Won, Bae) Manage the Disable Flag of TimeMark
protected:
	BOOL			m_bEnableFiringTimeShare;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/10/11] ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare);
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void);
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
protected:
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_TIMEMARKERADDINIMP_H__B6813EBB_4701_461A_A07E_03C6F985668B__INCLUDED_)
