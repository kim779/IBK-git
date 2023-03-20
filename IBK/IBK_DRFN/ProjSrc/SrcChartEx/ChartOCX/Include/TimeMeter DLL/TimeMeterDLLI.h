// TimeMeterDLLI.h: interface for the TimeMeter_...()
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMETERDLLI_H__BD96C933_9DC2_4F99_BBB6_34AC4B0F4085__INCLUDED_)
#define AFX_TIMEMETERDLLI_H__BD96C933_9DC2_4F99_BBB6_34AC4B0F4085__INCLUDED_

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2002.10.18
//
//	����	:	TimeMeter.DLL�� Interface �����̴�.
//
//	����	:	DLL�� Interface�̹Ƿ� 2002�� 11�� 21�� ����, �� File�� �������� �ʽ��ϴ�.
//
//	MT-Sync	:	�� �� �̻��� Thread�� �� Object�� ���� �����Ͽ�, 
//					Data ������ ������ �߻��� �� �ִ°�?
//					�� Code���� ������ Member�� ��ȿ�ϴٰ� �� �� �ִ°�?
//					��, Routine�� �����Ǵ� ���� ������ �� �� �ִ°�?
//					�� Object�� �����Ǿ� ������ �����ο?
//				Data		: DLL Instance, Manager Object Pointer
//				Action Flow	: Manager Auto Creation/Deletion, StartTimeMeter/EndTimeMeter, ResetAll
//				Result		: �� DLL�� �⺻������ Manager Object�� �����ϰ�,
//					��� ��ɿ� ���� Multi-Thread�� �����Ѵ�.
//
//	�Ѱ�	:	1. �� Library�� ������ �� 100���� 1���� 2������ ������ �߻��� �� �ִ�.
///////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef TIMEMETERDLL_EXPORTS
#define TIMEMETERDLL_API extern "C" __declspec(dllexport)
#else
#define TIMEMETERDLL_API extern "C" __declspec(dllimport)
#endif

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2002.11.13
//
//	����	:	�ߺ������� ���ջ� ����� ���������� �����Ѵ�.
//
//	����	:	(2003.12.27) TimeMeter ȣ��� Stack/Queue Mode�� �����ϰ�,
//					Report DLG���� Total ���θ� ������ �� �ֵ��� Total Mode�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
	TIMEMETER_STACKMODE,
	TIMEMETER_QUEUEMODE
} TimeMeter_SumMode;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2002.10.18
//
//	����	:	TimeMeter.DLL�� Interface�� �����Ѵ�.
//
//	����	:	(2002.10.22) Metering�� Enable/Disable�� �ս��� ó���ǵ��� Target ����� �߰��Ѵ�.
//				(2002.11.12) Title���� �ջ����� �ʴ� ����� �߰��Ѵ�.
//				(2002.11.13) Title���� �ջ����� �ʴ� ����� �߰��Ѵ�.
//				(2003.12.27) Report Dialog�� �ߺ��������� �ʵ��� Report Dialog�� Global�ϰ� �����ϸ�,
//					�ܺο��� Dialog�� �������� ���ϵ��� �����Ѵ�.
//				(2003.12.27) TimeMeter ȣ��� Stack/Queue Mode�� �����ϰ�,
//					Report DLG���� Total ���θ� ������ �� �ֵ��� Total Mode�� �����Ѵ�.
//					����, ������� �ʴ� ��� Stack Mode�� �����Ѵ�.
//				(2003.12.27) Report Dialog�� ȣ�� Routine���� ����� ��û�� �� �ֵ��� ó���Ѵ�.
//				(2003.12.27) Meter Target�� Bit Value�� �����ǵ��� DWORD64���� �����Ѵ�.
//				(2003.12.27) �ܺο��� ȣ�⿡ ���� Meter Target ������ �������� �ʴ´�.
//
//	����	:	1. Title�� �ջ��� ���� �ʱ� ���ؼ�, �ڵ����� Title �ڿ� ' XXX' �Ϸù�ȣ�� ���δ�.
///////////////////////////////////////////////////////////////////////////////
TIMEMETERDLL_API void		TimeMeter_GetMeterTarget( DWORD64 &p_dwMeterTarget);
TIMEMETERDLL_API void		TimeMeter_StartTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle, TimeMeter_SumMode p_eSumMode = TIMEMETER_STACKMODE);
TIMEMETERDLL_API void		TimeMeter_EndTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);
TIMEMETERDLL_API void		TimeMeter_ResetAll( void);
TIMEMETERDLL_API void		TimeMeter_ShowReportDialog( BOOL p_bShowWindow = TRUE);

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2002.10.28
//
//	����	:	�ڵ����� Title�� �ο��ϸ鼭 ���������� ó���ϴ� Interface�� �����Ѵ�.
//
//	����	:	1. �ڵ����� Title�� �����ϸ鼭 Metering�� ó���Ѵ�.
//				2. Title�� "Default AutoTimeMeter XX"�� ���·� �����ȴ�.
//				3. Title�� �����Ǵ� ���� Default AutoTimeMeter Index�� �������� �ʴ´�.
//				4. 100��° Default AutoTimeMeter Index�� �ٽ� 0���� �ʱ�ȭ�ȴ�.
//
//	�Ѱ�	:	1. AutoTimeMeter�� �ð� Flow�󿡼� Nesting(��ø)�� �� ����.
//
//	����	:	(2003.12.27) Meter Target�� Bit Value�� �����ǵ��� Long���� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
TIMEMETERDLL_API void		TimeMeter_StartAutoTimeMeter( DWORD64 p_dwMeterTarget);
TIMEMETERDLL_API void		TimeMeter_NextAutoTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle = NULL);
TIMEMETERDLL_API void		TimeMeter_EndAutoTimeMeter( DWORD64 p_dwMeterTarget);

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2002.11.21
//
//	����	:	�ҿ� �ð��� �ƴ϶�, ���� �ð��� Ȯ���ϱ� ���� ������ Interface�� �����Ѵ�.
//
//	����	:	���������� TimeMeter_StartTimeMeter()��, �� TimeMeter_EndTimeMeter()�� ó���ϸ�,
//				Ư�� TIMEMETER_QUEUEMODE�� ó���ȴ�.
//
//	����	:	(2003.01.06) TIMEMETER_QUEUEMODE���� TIMEMETER_TOTALMODE�� �����Ѵ�. (Name�� �ڵ� ��ȣ �ٴ� �� ������)
//				(2003.01.16) TIMEMETER_TOTALMODE���� TIMEMETER_QUEUEMODE�� �����ϰ�, '000'���� Clear ��Ų��.
//					(1ȸ�� Time Check�� �Ϸù�ȣ ��� ����)
//				(2003.12.27) Meter Target�� Bit Value�� �����ǵ��� DWORD64���� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
TIMEMETERDLL_API void		TimeMeter_CheckTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle);

#endif // !defined(AFX_TIMEMETERDLLI_H__BD96C933_9DC2_4F99_BBB6_34AC4B0F4085__INCLUDED_)
