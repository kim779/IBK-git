// TimeMeterTarget.h: define for the TimeMeter DLL Loader
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMETERTARGET_H__B39EB2BC_8907_4783_B312_24F73E115A9A__INCLUDED_)
#define AFX_TIMEMETERTARGET_H__B39EB2BC_8907_4783_B312_24F73E115A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.28
//
//	����	:	TimeMeter ������ ��ȿȭ��Ű�� Macro�� �����Ѵ�.
//
//	����	:	1. Define ���� 0���� �ϰų� Define�� ���� �ʵ��� ����ó���ϸ� ��� TimeMeter ȣ����
//					�ڵ����� ���ŵȴ�.
//				2. ��, TimeMeter ȣ���� ���Ͽ� ���� �۾��� �߰��� ��쿡�� #if _TIMEMETER_USE
//					~ #endif�� ���� �Բ� ���ŵǵ��� �Ͽ��� �Ѵ�.
///////////////////////////////////////////////////////////////////////////////
#define	_TIMEMETER_USE	1

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.28
//
//	����	:	TimeMeter ȣ���� ���� Interface ������ Include�Ѵ�.
//
//	����	:
//				void _TimeMeter_LoadTimeMeter( void)
//						1. TimeMeter Module�� Loading�Ѵ�.
//						2. �ݺ� �����Ͽ��� �ߺ� Loading���� �ʴ´�.
//
//				void _TimeMeter_GetMeterTarget( DWORD64 &p_dwMeterTarget)
//						1. ���� �������� Target�� Ȯ���Ѵ�. Bit Mask�� ���յǾ� �ִ�.
//
//				void _TimeMeter_StartTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. p_szTitle�� �̸����� ������ ���۽ð��� �����Ѵ�.
//						2. ���� ������ Target�� �ش���� ������ �ٷ� Return�Ѵ�.
//						3. p_szTitle�� �̸����� ������ �ݺ��� ��� Stack ������� �����Ѵ�.
//							�� ���������� ������ ������ ���� ������. (�ڼ��� ���� Document ����)
//
//				void _TimeMeter_StartTimeMeterQMode( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. Queue ������� _TimeMeter_StartTimeMeter()�� ó���Ѵ�.
//							���� ������ ������ ���� ������. (�ڼ��� ���� Document ����)
//
//				void _TimeMeter_EndTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. p_szTitle�� �̸����� ���۵� ������ �����ð��� �����Ͽ� ������ ������.
//						2. ���� ������ Target�� �ش���� ������ �ٷ� Return�Ѵ�.
//
//				void _TimeMeter_ResetAll( void)
//						1. ���ݱ����� ��������� Reset ��Ų��.
//
//				void _TimeMeter_ShowReportDialog( void)
//						1. ���� ��� ��ȭ���ڸ� ��½�Ų��.
//
//				void _TimeMeter_HideReportDialog( void)
//						1. ���� ��� ��ȭ���ڸ� �����.
//
//				void _TimeMeter_StartAutoTimeMeter( DWORD64 p_dwMeterTarget)
//						1. �ڵ� ������ �����Ѵ�.
//						2. ���� ������ Target�� �ش���� ������ �ٷ� Return�Ѵ�.
//
//				void _TimeMeter_NextAutoTimeMeter( DWORD64 p_dwMeterTarget)
//						1. ���� �ڵ� ������ �������ϰ�, ���ο� �ڵ������� �����Ѵ�.
//						2. ���� ������ Target�� �ش���� ������ �ٷ� Return�Ѵ�.
//						3. Title�� �ڵ����� �����ǹǷ�, ���� ����� ��Ȯ�� �Ǵ��ϱ� ���ؼ���
//							��Ȯ�� Code�� ���� ������ �ľ��Ͽ��� �Ѵ�.
//
//				void _TimeMeter_NextAutoTimeMeterWithTitle( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. ���ο� �ڵ������� Title�� �����ϸ� _TimeMeter_NextAutoTimeMeter()�� ó���Ѵ�.
//
//				void _TimeMeter_EndAutoTimeMeter( DWORD64 p_dwMeterTarget)
//						1. �ڵ� ������ �������Ѵ�.
//						2. ���� ������ Target�� �ش���� ������ �ٷ� Return�Ѵ�.
//
//				void _TimeMeter_CheckTimeMeter( DWORD64 p_dwMeterTarget, const char *p_szTitle)
//						1. p_szTitle�� �̸����� ���� �ð��� �����Ѵ�.
//						2. ���� ������ Target�� �ش���� ������ �ٷ� Return�Ѵ�.
//						3. ���������� ������ �������ڸ��� �ٷ� ������.
//
///////////////////////////////////////////////////////////////////////////////
#include "TimeMeterLoader.h"

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.27
//
//	����	:	TimeMeter�� Start/End/Check/Auto ó���ÿ� �̿�Ǵ� Target�� ������ Define�Ͽ� �̿�����Ѵ�.
//
//	����	:	1. TimeMeter Target�� ���� ���Ǹ� ���Ͽ� Report Dialog���� Runtime�ÿ� ������ ��������Ѵ�.
//				2. Target���� ���Ǵ� Bit Value�� ���ǵǸ�, Report Dialog���� Runtime�� �����Ǵ� Target��
//					Bit AND�� ���Ͽ�, ������ �Ƿ��ϴ� Target Code�� ���Կ��η� ������ ������ �����Ѵ�.
//				3. Target���� DWORD64�� 64Bit�� �� 64���� ������ �����ϴ�.
//				4. �׷��� ������ Category ������ �̿��� ��� �� ���ϰ� �ȴ�.
//
//	����	:	1. Target���� 0�̸� �������� �ʴ´�.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.29
//
//	����	:	SK ������ HTS��, ���� Topic�� �����Ѵ�.
//
//	����	:	1. TimeMeter.cfg�� ��ϵǴ� Target List�̴�.
//				2. Target���� ù '+/-/.'�� Total Sum Mode�� '����/���/����'�� �ǹ��Ѵ�.
///////////////////////////////////////////////////////////////////////////////
//																	= 0x123456789ABCDEFG
//	+ȭ�� Loading ����												= 0xFFFFFFFFFFFF0010
//	-ȭ�� Loading ���� (Main)										= 0x0000000000010010
//	+ȭ�� Loading ���� (Desk)										= 0x0000000000040010
//	+ȭ�� Loading ���� [Control�� CreateWindowlessControl()�ð�]	= 0x0000000000040020
//	+ȭ�� Loading ���� [Control�� SetMapInfoToCtrl()�ð�]			= 0x0000000000040040
//	+ȭ�� Loading ���� [Control�� ���� �� �ʱ�ȭ �ð�]				= 0x0000000000040060
//	-TR ó�� ����													= 0x0000000000040080
//	-ȭ�� Loading�� TR ó��											= 0xFFFFFFFFFFFF0090
//	.Script ó�� ����												= 0x0000000000040100
//	-TR�� Script ó�� ����											= 0x0000000000040180
//	-ȭ�� Loading�� Script ����										= 0xFFFFFFFFFFFF0110
//	-ȭ�� Loading�� Script ���� (Desk)								= 0x0000000000040110
//	-ȭ�� Loading�� TR �� Script ����								= 0xFFFFFFFFFFFF0190
//	+GetAllProperties2 ������ Component								= 0x0000000000040200
//	-ȭ�� Loading ��ü ��� ����									= 0xFFFFFFFFFFFF0400
//	+Real ó�� �ӵ� ����											= 0xFFFFFFFFFFFF0800
//	+Drawing ó�� �ӵ� ����											= 0xFFFFFFFFFFFF1000
//	-TEMP															= 0x0000000000000001

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.29
//
//	����	:	SK ������ HTS�� Target�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
//									0x123456789ABCDEFG
// 1. Module �� Target�� �����Ѵ�. (0x0000000000FF0000)
#define	_TMT_M_MAIN					0x0000000000010000
#define _TMT_M_PRIMLIB				0x0000000000020000
#define _TMT_M_DESK					0x0000000000040000
#define _TMT_M_SOCKET				0x0000000000080000
#define _TMT_M_TICKER				0x0000000000100000
//									0x123456789ABCDEFG
// 2. Control�� Target�� �����Ѵ�. (0xFFFFFFFFFF000000)
#define _TMT_M_ACCTINPUT			0x0000000001000000
#define _TMT_M_BUFFER				0x0000000002000000
#define _TMT_M_CALENDAR				0x0000000004000000
#define _TMT_M_CHART				0x0000000008000000
#define _TMT_M_CHECKBUTTON			0x0000000010000000
#define _TMT_M_CODE					0x0000000020000000
#define _TMT_M_CODEINPUT			0x0000000040000000
#define _TMT_M_COMBO				0x0000000080000000
#define _TMT_M_GRID					0x0000000100000000
#define _TMT_M_GROUP				0x0000000200000000
#define _TMT_M_IMAGEBUTTON			0x0000000400000000
#define _TMT_M_IMAGEVIEWER			0x0000000800000000
#define _TMT_M_INPUT				0x0000001000000000
#define _TMT_M_LABEL				0x0000002000000000
#define _TMT_M_MGRID				0x0000004000000000
#define _TMT_M_OUTPUT				0x0000008000000000
#define _TMT_M_PASS					0x0000010000000000
#define _TMT_M_PROGRESS				0x0000020000000000
#define _TMT_M_RADIO				0x0000040000000000
#define _TMT_M_RICHTX				0x0000080000000000
#define _TMT_M_SHAPE				0x0000100000000000
#define _TMT_M_SPININPUT			0x0000200000000000
#define _TMT_M_TABLE				0x0000400000000000
#define _TMT_M_TIMER				0x0000800000000000
#define _TMT_M_TREE					0x0001000000000000
#define _TMT_M_WEB					0x0002000000000000
#define _TMT_M_YOUNSOK				0x0004000000000000
#define _TMT_M_SLIMCHART			0x0008000000000000
#define _TMT_M_SLIMCODEINPUT		0x0010000000000000
#define _TMT_M_SLIMCOMBO			0x0020000000000000
#define _TMT_M_SLIMGRID				0x0040000000000000
#define _TMT_M_SLIMIMAGEBUTTON		0x0080000000000000
#define _TMT_M_SLIMSPININPUT		0x0100000000000000
#define _TMT_M_SLIMTABLE			0x0200000000000000
#define _TMT_M_FOCHART				0x0400000000000000
#define _TMT_M_DLLOCX				0x0800000000000000
//									0x123456789ABCDEFG
// 3. ��ɺ� Target�� �����Ѵ�.	   (0x000000000000FFF0)
#define _TMT_F_MAP_OPEN				0x0000000000000010		// ȭ�� Loading ����
#define _TMT_F_CONTROL_CREATE		0x0000000000000020		// ȭ�� Loading ���� [Control�� CreateWindowlessControl()�ð�]
#define _TMT_F_CONTROL_SETMAPINFO	0x0000000000000040		// ȭ�� Loading ���� [Control�� SetMapInfoToCtrl()�ð�]
#define _TMT_F_TR					0x0000000000000080		// TR ó�� ����
#define _TMT_F_SCRIPT				0x0000000000000100		// Script ó�� ����
#define _TMT_F_NOT_GETBASEPROPIF	0x0000000000000200		// Control�� GetAllProperties2 ���� ���� Ȯ��
#define _TMT_F_S_MAP_T_TR			0x0000000000000400		// ȭ�� Loading ��ü ��� ����
#define _TMT_F_REAL					0x0000000000000800		// Real ó�� �ӵ� ����
#define _TMT_F_DRAW					0x0000000000001000		// Drawing ó�� �ӵ� ����
#define _TMT_F_SHARE				0x0000000000002000		// ���� ó�� �ӵ� ����
//									0x123456789ABCDEFG
// 4. �ӽ� Target�� �����Ѵ�.	   (0x000000000000000F)
#define _TMT_TEMP					0x0000000000000001
#define _TMT_TEMP2					0x0000000000000002
#define _TMT_TEMP3					0x0000000000000004
#define _TMT_TEMP4					0x0000000000000008

#endif // !defined(AFX_TIMEMETERTARGET_H__B39EB2BC_8907_4783_B312_24F73E115A9A__INCLUDED_)
