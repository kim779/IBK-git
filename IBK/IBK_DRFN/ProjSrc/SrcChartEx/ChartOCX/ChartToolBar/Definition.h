#ifndef _DEFINITION_H
#define _DEFINITION_H

// (2006/2/17 - Seung-Won, Bae) Share ToolBar's Height with OCX
#include "../Include_Chart/DLL_Load/ToolBarManager.h"		// for _OCX_INNERTOOLBAR_HEIGHT

// ������. ��ư�� ���� ���� ������ �� �����̴����� �ʺ�. ���̴���Ʈ�� ���̵��� �����Ѵ�.
#define TB_BUTTON_WIDTH			18							// ��ư�� ���� ����
#define TB_BUTTON_HEIGHT		_OCX_INNERTOOLBAR_HEIGHT	// ��ư�� ���� ����		// (2006/2/17 - Seung-Won, Bae) Share ToolBar's Height with OCX
#define TB_BUTTON_SLIDERBAR		70					// �����̴��� �ʺ�.
#define TB_RADAR_HEIGHT			30					// Radar Chart Height
#define TB_RADAR_STATIC_WIDTH	120					// Radar Chart Static Width

// ��ġ��ȸâ(�������� �ٴ³��� ���� ����)
#define TB_NUMERICALINQIURYDLG_HEIGHT	_OCX_INNERTOOLBAR_HEIGHT	// (2006/2/17 - Seung-Won, Bae) Share ToolBar's Height with OCX

// ��ư ���ڰ�. �ϳ��� ToolOptionInfo���� �ΰ��̻��� ��ư�� �����ؾ� �ϴ� ���
// ToolOptionInfo�� �߰������� ������ ��ư�� �����ؾ��ϴ� �Ķ���Ͱ� �ʿ��ϴ�.
#define TB_BUTTONPARAM_LEFT			_T("Left")			// ����
#define TB_BUTTONPARAM_CENTER		_T("Center")		// �߾�
#define TB_BUTTONPARAM_RIGHT		_T("Right")			// ������

// ��ư �ʱ�ID��. ��ư��Ʈ���� ID���� �������� ������Ų��.
#define TB_CTRL_INITIAL_ID		18000				// ���� ��ư ��Ʈ�� �ʱ� ���̵�

// �ڵ���ũ�� Ÿ�̸� ���̵�
#define AUTOSCROLL_LEFT_TIMER_ID	200				// �����ڵ���ũ�� Ÿ�̸� ���̵�
#define AUTOSCROLL_RIGHT_TIMER_ID	201				// ������ �ڵ���ũ�� Ÿ�̸� ���̵�

#endif