#pragma once

#define WM_STOPMSG	WM_USER + 100
// wparam : LOWORD kind
//          HIWORD variable
// lparam : value

#define SK_STOP_RUNNING	0x01	// wParam : HIWORD : �ֽ�, ���� kind
				// lParam : TRUE,FALSE

#define KS_STOCK	0x01	// �ֽ�
#define KS_FUTURE	0x02	// ����

