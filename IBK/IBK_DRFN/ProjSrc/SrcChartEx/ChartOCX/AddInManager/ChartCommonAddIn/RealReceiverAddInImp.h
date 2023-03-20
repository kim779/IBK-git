// RealReceiverAddInImp.h: interface for the CRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALRECEIVERADDINIMP_H__3A920719_067B_4CD3_969C_520CD6E51C53__INCLUDED_)
#define AFX_REALRECEIVERADDINIMP_H__3A920719_067B_4CD3_969C_520CD6E51C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2009/10/31 - Seung-Won, Bae) ����� ����.
// 1. ���� �ð����� ����	: ����, Local Time, Main Time
// 2. ��� ���� ���
//		i.		��� �߰������� �����ϰ� �Ǻ� ���Ž�, �߰��� ��� Rollback ó���� ���� ��� �ʿ�.
//		ii.		��� �ð��� �ش���
//		iii.	


#include <afxmt.h>										// for CCriticalSection

#include "../../Include_Chart/TimeTypeConvert.h"		// for CTimeTypeConvert
#include "../../Include_Chart/IRealReceiver.h"			// for IRealReceiver
#include "../../Include_Chart/IOldHTSRealReceiver.h"	// for IOldHTSRealReceiver
#include "../../Include_Chart/IOldFXRealReceiver.h"		// for IOldFXRealReceiver

#include "../Include_Addin/I000000/_ChartAddInBase.h"	// for CChartAddInBase

class CDumpLogger;
class CRealReceiverAddInImp : public CChartAddInBase, public IRealReceiver, public IOldFXRealReceiver, public IOldHTSRealReceiver
{
public:
	CRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CRealReceiverAddInImp();


// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
//		*. ���� ������ _IWRAPPER_IUNKNOWN_INTERFACE�� �����Ѵ�.
private:
	virtual IUnknown *	GetAddInCustomInterface( void);
// (2005.07.08, ��¿�) AddIn Special Interface�� ������ ���Ͽ� �⺻ Interface (IUnknown��)�� �����Ѵ�.
//		*. ���� GetAddInCustomInterface()�� ��� ������ ���� ������, 
//			GetAddInCustomInterface() ������ �Բ� �����Ѵ�.
private:
	_IWRAPPER_IUNKNOWN_INTERFACE()


// (2006/7/26 - Seung-Won, Bae) Manage Removed Packet Data Count for Reset Scroll in OnDrdsPacketEnd().
private:
	int		m_nRemovePacketDataCount;


//////////////////////////////////////////////////////////////////////
// (2007/3/2 - Seung-Won, Bae) Manage Real Receiveing Check to skip the Empty PacketEnd Event.
//////////////////////////////////////////////////////////////////////
private:
	int		m_nRealReceivedType;		// 0:No Real, 1:Time Real, 2:Indexed Real


// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
private:								// Skip same '�ڷ�����'
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_eScaleDrawType;
	double								m_dPrevServerTime;
private:				// Status Data
	long				m_lRealReceiveCount;

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long				m_lServerTime;
	__int64				m_lServerTime;
	// 2011.01.06 by SYS <<

	long				m_lDrawSpeed;
private:				// Analysis Real-Delay
	BOOL				m_bSecondTime;
	long				m_lServerBaseTime;
	long				m_lClientBaseTime1000;
	long				m_lRealLoadTime;
	long				m_lRealLoadTimeMax;
private:				// Real Process Speed
	long				m_lPrevClientTime1000;
	double				m_d5RealReceiveSpeed;
	double				m_dRealReceiveSpeedAverage;
	double				m_dRealReceiveSpeedMax;
	long				m_lLastRealCountOnDraw;
	long				m_l5RealProcessCountPerDraw;
	long				m_l5RealProcessCountPerDrawAverage;
	long				m_l5RealProcessCountPerDrawMax;
private:				// Multi-Thread
	static CWinThread *	m_pWinThread;
	static HANDLE		m_hRealReceiveEvent;
private:
	void				GetRealMonitorDataInPacketEnd( void);
	long				InvalidateControl( void);
	//BOOL				GetLoadTime( long &p_lLoadTime, long &p_lDelayTime);	// (2007/4/6 - Seung-Won, Bae) Use Min Load Time for Other Map Effect.
	static UINT			RealUpdateWithClutch( LPVOID pParam);
// (2007/3/13 - Seung-Won, Bae) Synchronized Draw
private:
	BOOL														m_bDoInvalidate;
	static CCriticalSection										m_csRealReceivers;
	static CTypedPtrArray < CPtrArray, CRealReceiverAddInImp *>	m_aRealReceivers;
private:
	static long			InvalidateControls( void);
	//static long			GetMaxLoadTime( void);

// (2012/04/05) for Clutch Draw Speed.
protected:
	static double		m_dDrawSkipFactor;
	static int			m_nCpuCount;
public:
	static double		GetDrawSkipFactor( void)						{	return m_dDrawSkipFactor * m_nCpuCount;					}
	static void			SetDrawSkipFactor( double p_dDrawSkipFactor)	{	m_dDrawSkipFactor = p_dDrawSkipFactor / m_nCpuCount;	}

// (2007/4/6 - Seung-Won, Bae) Real Time Speed Logging
private:
	CDumpLogger *	m_pDumpLogger;

// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
private:
	BOOL		m_bRealLoadIndicator;
	BOOL		m_bRealSpeedLog;
	BOOL		m_bRealDataLog;
	BOOL		m_bRealNoData;

// (2008/12/26 - Seung-Won, Bae) Make cache.
private:
	BOOL		m_bIsRuntimeMode;
	BOOL		m_bInUseRealData;

// 2008.08.25 ������ ����� ó��	<<
private:
	__int64					m_nRealClockSecs;
	CTimeTypeConvert 		m_tcReal;
	static int				m_nNomalizedMinuteTimerID;
private:
	static void	CALLBACK	OnNomalizedMinuteTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	void					SetUpdateTick( void);
public:
							// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
	virtual _int64			GetNMTRClock( void)		{	return m_nRealClockSecs;	}

protected:
	virtual BOOL	UpdatePacketData( const char *p_szPacketName, const int p_nIndex, const double p_dData);
	// �������� ����ó�� : �������� - ojtaso (20070208)
	// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	virtual BOOL	SetRealDataRQ(LPCTSTR lpszRQ, const char *p_szPacketName, double p_dData, BOOL p_bRealData);
	virtual BOOL	SetRealPacketEndRQ(LPCTSTR lpszRQ, BOOL p_bRealData);

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
private:
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);

private:
	CString m_szDebugRealData;
};

#endif // !defined(AFX_REALRECEIVERADDINIMP_H__3A920719_067B_4CD3_969C_520CD6E51C53__INCLUDED_)
