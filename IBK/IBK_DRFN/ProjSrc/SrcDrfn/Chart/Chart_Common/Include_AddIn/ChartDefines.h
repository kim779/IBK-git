// ChartDefines.h: Define for Chart & Addine Common Values
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTDEFINES_H__70621873_4DC4_4F9F_AB31_11D47FACF3CC__INCLUDED_)
#define AFX_CHARTDEFINES_H__70621873_4DC4_4F9F_AB31_11D47FACF3CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"

// (2004.10.11, ��¿�) DLL Container�� Communication�� ���� Message�� �����Ѵ�.
// (2004.05.06, ��¿�) Chart OCX���� ������ Event Message�� �����Ѵ�.
//		WPARAM	: ID
//		LPARAM	: MSG Structure Address
//			MSG msg;
//			msg.hwnd	= GetSafeHwnd();
//			msg.message	= message;
//			msg.wParam	= wParam;
//			msg.lParam	= lParam;
#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"
extern const UINT RMSG_CHARTMSG_EVENTMESSAGE;

// (2004.05.18, ��¿�) RMSG_CHARTMSG_EVENTMESSAGE�� ���޵Ǵ� Event Message Structure�� ������ Message�� ���޵ȴ�.
// (2004.05.18, ��¿�) DLL Container���� �� ȭ���� Data �� (ZoomValue)�� ��ȭ�� �����Ѵ�.
//		�̶�, LParam�� ���� ZoomValue�̴�.
#define UWM_ZOOMVALUE				( WM_USER + 0x2D36)
// (2004.06.04, ��¿�) Scroll�� ����� ������ DLL Container���� ���Ͻø� �����Ѵ�.
//		�̶�, LParam�� ���� �� ȭ���� ������ Data�� �Ͻø� Long Value�� �����ϰ� �ȴ�.
#define UWM_VIEWENDTIME				( WM_USER + 0x2D37)
// (2004.10.08, ��¿�) ���ڼ� �̿���� ��ġ��ȸâ �̿뿩�� ����� Multi-Chart���� �˷�,
//		�ٸ� ������Ʈ���� �� ������ ����ǵ��� �Ѵ�.
//		�̶�, WParam�� ���� ������ ������ ��ġ��ȸâ �̿뿩�� �������̴�.
#define UWM_USEINQUIRY_CROSSLINE	( WM_USER + 0x2D39)
// (2004.11.04, ��¿�) ��Ʈ�� Global ���� Data �߻��� DLL Container���� �˸��� Message�̴�.
//		�̶� LParam���� "������;����Data"�� ���·� Data�� ���޵ȴ�.
//		�⺻������ Chart�� �ð�����, �Ϲ�ȣ������, Ư��ȣ������(ȭ�麰���ѺҰ�)�� �߻���Ų��.
#define UWM_FIREGSHAREDATA			( WM_USER + 0x2D3B)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify I (Drag Start Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGSTART				( WM_USER + 0x2D3C)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify II (Drag End Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGEND					( WM_USER + 0x2D3D)
// (2005/10/25 - Sun-Young, Kim) ��Ʈ ��ü�ʱ�ȭ
#define UWM_ORGRETURN				( WM_USER + 0x2D40)

// (2004.05.21, ��¿�) �˵���Ʈ/����Ʈ/����Ʈ�� Chart Type�� �����Ѵ�.
//		'�˵���Ʈ/����Ʈ/����Ʈ'�� '������Ʈ'�� SubType�̳� �ܺ������δ� �⺻ Type���� �νĵǵ��� �Ѵ�.
#define _CHARTDLL_CHARTTYPE_PRICE			"������Ʈ"
#define _CHARTDLL_CHARTTYPE_CANDLE			"ĵ����Ʈ"
#define _CHARTDLL_CHARTTYPE_BAR				"����Ʈ"
#define _CHARTDLL_CHARTTYPE_LINE			"����Ʈ"
#define _CHARTDLL_CHARTTYPE_INVERSELINE		"���ð�"
#define _CHARTDLL_CHARTTYPE_THREELINEBREAK	"�Ｑ��ȯ��"
#define _CHARTDLL_CHARTTYPE_PNF				"P&F"

#endif // !defined(AFX_CHARTDEFINES_H__70621873_4DC4_4F9F_AB31_11D47FACF3CC__INCLUDED_)
