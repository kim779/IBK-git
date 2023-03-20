// TimeMeterLoader.h: interface for the CTimeMeterLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMETERLOADER_H__B028E205_E689_4401_ABFD_25E6A65046A6__INCLUDED_)
#define AFX_TIMEMETERLOADER_H__B028E205_E689_4401_ABFD_25E6A65046A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TimeMeter_PDll.h"		// for PDLL
#include "TimeMeterDLLI.h"		// for TimeMeter_SumMode

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.28
//
//	����	:	TimeMeter DLL�� �������� Loading�ϵ��� Loading Helper�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
class CTimeMeterLoader : public PDLL  
{
// (2004.09.22, ��¿�) ������ Contructor���� AutoLoading�ϴ� Code�� ������ ����,
//		Constructor ��ü�� ��������, VC Project�� �� Header�� ���ԵǾ� ���� �ʾƼ����� ������,
//		Constructor(Default)���� ��� Loading�Ǵ� ������ Ȯ�εǾ�, ��������� AutoLoading�� �����Ѵ�.
//		(*. �ٽ� �������� �� Contructor�� ����� AutoLoading�̵Ǵ� ������ �ٽ� ��Ÿ���� �ʾ�����,
//			�ٸ� Module���� Loading�ÿ��� ���� ����� �� �ֵ��� ��������� ��Ÿ����.)
public:
	CTimeMeterLoader()	{}

// (2003.03.09) OCX�� Register�ÿ��� Loading���� �ʰ� ������ Init�� ��ġ���� �����Ѵ�.
public:
	void	LoadTimeMeter( void)	{	if( !m_dllHandle) LoadDll( "TimeMeter.dll");	}

// 1. �⺻ ���� Interface�� �����Ѵ�.
public:
	// void TimeMeter_GetMeterTarget( DWORD64 &p_dwMeterTarget);
	DECLARE_FUNCTION1( TimeMeter_GetMeterTarget, DWORD64 &)
	// void TimeMeter_StartTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle, TimeMeter_SumMode p_eSumMode = TIMEMETER_STACKMODE);
	DECLARE_FUNCTION3_V3( TimeMeter_StartTimeMeter, DWORD64, const char *, TimeMeter_SumMode, TIMEMETER_STACKMODE)
	// void TimeMeter_EndTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);
	DECLARE_FUNCTION2( TimeMeter_EndTimeMeter, DWORD64, const char *)
	// void TimeMeter_ResetAll( void);
	DECLARE_FUNCTION0( TimeMeter_ResetAll)
	// void TimeMeter_ShowReportDialog( BOOL p_bShowWindow = TRUE);
	DECLARE_FUNCTION1_V1( TimeMeter_ShowReportDialog, BOOL, TRUE)

// 2. AutoMeter Interface�� �����Ѵ�.
public:
	// void TimeMeter_StartAutoTimeMeter( DWORD64 p_dwMeterTarget);
	DECLARE_FUNCTION1( TimeMeter_StartAutoTimeMeter, DWORD64)
	// void TimeMeter_NextAutoTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle = NULL);
	DECLARE_FUNCTION2_V2( TimeMeter_NextAutoTimeMeter, DWORD64, const char *, NULL)
	// void TimeMeter_EndAutoTimeMeter( DWORD64 p_dwMeterTarget);
	DECLARE_FUNCTION1( TimeMeter_EndAutoTimeMeter, DWORD64)

// 3. CheckTime Interface�� �����Ѵ�.
public:
	// void TimeMeter_CheckTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);
	DECLARE_FUNCTION2( TimeMeter_CheckTimeMeter, DWORD64, const char *)
};

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.28
//
//	����	:	TimeMeter DLL�� Loading�ϴ� Global LoadHelper�� Access�� �� �ֵ���
//					�����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
extern CTimeMeterLoader	g_dllTimeMeter;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.28
//
//	����	:	Macro Define�� ���� ȣ���� ��ȿȭ�� �� �ֵ���, ȣ�� Interface�� Macro�� �����Ѵ�.
//
//	����	:	(2003.03.09) OCX�� Register�ÿ��� Loading���� �ʰ� ������ Init�� ��ġ���� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
#if _TIMEMETER_USE
	#define _TimeMeter_LoadTimeMeter()								g_dllTimeMeter.LoadTimeMeter()
	#define _TimeMeter_GetMeterTarget( Param1)						g_dllTimeMeter.TimeMeter_GetMeterTarget( Param1)
	#define _TimeMeter_StartTimeMeter( Param1, Param2)				g_dllTimeMeter.TimeMeter_StartTimeMeter( Param1, Param2)
	#define _TimeMeter_StartTimeMeterQMode( Param1, Param2)			g_dllTimeMeter.TimeMeter_StartTimeMeter( Param1, Param2, TIMEMETER_QUEUEMODE)
	#define _TimeMeter_EndTimeMeter( Param1, Param2)				g_dllTimeMeter.TimeMeter_EndTimeMeter( Param1, Param2)
	#define _TimeMeter_ResetAll()									g_dllTimeMeter.TimeMeter_ResetAll()
	#define _TimeMeter_ShowReportDialog()							g_dllTimeMeter.TimeMeter_ShowReportDialog()
	#define _TimeMeter_HideReportDialog()							g_dllTimeMeter.TimeMeter_ShowReportDialog( FALSE)
	#define _TimeMeter_StartAutoTimeMeter( Param1)					g_dllTimeMeter.TimeMeter_StartAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeter( Param1)					g_dllTimeMeter.TimeMeter_NextAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeterWithTitle( Param1, Param2)	g_dllTimeMeter.TimeMeter_NextAutoTimeMeter( Param1, Param2)
	#define _TimeMeter_EndAutoTimeMeter( Param1)					g_dllTimeMeter.TimeMeter_EndAutoTimeMeter( Param1)
	#define _TimeMeter_CheckTimeMeter( Param1, Param2)				g_dllTimeMeter.TimeMeter_CheckTimeMeter( Param1, Param2)
#else
	#define _TimeMeter_LoadTimeMeter()
	#define _TimeMeter_GetMeterTarget( Param1)
	#define _TimeMeter_StartTimeMeter( Param1, Param2)
	#define _TimeMeter_StartTimeMeterQMode( Param1, Param2)
	#define _TimeMeter_EndTimeMeter( Param1, Param2)
	#define _TimeMeter_ResetAll()
	#define _TimeMeter_ShowReportDialog()
	#define _TimeMeter_HideReportDialog()
	#define _TimeMeter_StartAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeter( Param1)
	#define _TimeMeter_NextAutoTimeMeterWithTitle( Param1, Param2)
	#define _TimeMeter_EndAutoTimeMeter( Param1)
	#define _TimeMeter_CheckTimeMeter( Param1, Param2)
#endif

#endif // !defined(AFX_TIMEMETERLOADER_H__B028E205_E689_4401_ABFD_25E6A65046A6__INCLUDED_)
