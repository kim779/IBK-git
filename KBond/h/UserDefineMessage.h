/*-----------------------------------------------------------
//
//	��������� Message
//	file	: UserDefineMessage.h
//	written	: scbang
//	date	: 2003. 12. 24
//	ver	: 1.0
-------------------------------------------------------------*/

#ifndef _USER_DEFINE_MESSAGE
#define _USER_DEFINE_MESSAGE


#define	UDM_MESSAGE		WM_USER + 888


// ȯ��
#define	UDM_GET_COLOR		1000		// Color		// GetColor(lParam);
#define	UDM_GET_ROOT		1001		// Root Dir		// GetRoot();
#define	UDM_GET_PARENT_WND	1002		// Parent Wnd		// m_pwndParent

// ��������
#define	UDM_GET_JCOD_NAME	1100		// �����
#define	UDM_GET_JCOD		1101		// �����ڵ�

// Type
#define	UDM_GET_TYPE		1200		// ����, �ɼ� ����

// Mouse Action
#define	UDM_LBUTTON_DOWN	1300		// LButton Down
#define	UDM_LBUTTON_UP		1301		// LButton Up
#define	UDM_RBUTTON_DOWN	1310		// RButton Down
#define	UDM_RBUTTON_UP		1311		// RButton Up


// ��������
#define	UDM_GET_ACCOUNT		2000		// ���¹�ȣ		// m_strAccount.operator LPCTSTR();
#define	UDM_GET_ACCOUNT_NAME	2001		// ���¸�
#define	UDM_GET_PASSWORD	2002		// �н�����

// �ܰ�����
#define	UDM_GET_BALANCE		2100		// �ܰ� ����		// m_ptw->GetBalance();

// �ֹ� ��������
#define	UDM_GET_ORDER_COUNT	2200		// ����		// m_ptcw->GetCount();

// �ֹ�ȯ�� ����
#define	UDM_GET_CONFIRM_ORDER	2300		// �ֹ�Ȯ�ο���
#define	UDM_GET_ENABLE_ORDER	2301		// �ֹ����ɿ���


// �ֹ�����
#define	UDM_SEND_NEW_ORDER	2400		// �ű��ֹ�
#define	UDM_SEND_CANCEL_ORDER	2401		// ����ֹ�
#define	UDM_SEND_FIX_ORDER	2402		// �����ֹ�
#define	UDM_SEND_OPPOSITE_ORDER	2403		// �ݴ��ֹ�
#define	UDM_SEND_ENABLE_COUNT_ORDER	2410	// �ֹ����� ������ŭ �ֹ�


// �ֹ� ���ɼ���
#define	UDM_SEND_ENABLE_ORDER_COUNT	2600		// �ֹ����ɼ��� ��ȸ



// Guide Message
#define	UDM_GUIDE_MESSAGE	4000		// Guide Message
#define	UDM_POST_GUIDE_MESSAGE	4001		// Post Guide Message




// Popup
#define	UDM_POPUP_SELL_ENABLE	5000		// �ŵ�����		// PopupSellEnable((DWORD)lParam);
#define	UDM_POPUP_BUY_ENABLE	5001		// �ż�����		// PopupBuyEnable((DWORD)lParam);




// �ٸ��������� pointer
#define	UDM_GET_MESSAGE_WND	9000		// MessageWnd Pointer Up
#define	UDM_GET_TICKCOUNT_WND	9001		// TickCountWnd Pointer Up


#endif