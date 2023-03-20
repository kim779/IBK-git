#ifndef __MESSAGEDEFINE_H__
#define __MESSAGEDEFINE_H__

//Main <-> Chart Control
const UINT RMS_ANALYSIS_FUNC = ::RegisterWindowMessage(_T("RMS_ANALYSIS_FUNC"));

//Scroll
#define	UM_SCROLL_STARTEND_MESSAGE			WM_USER + 506
#define UM_SCROLL_SAVE_INFO					WM_USER + 507
//Cursor exchange
#define UM_CURSOR_EXCHANGE					WM_USER + 521

//�ְ����� -> ��Ŷ�� ������ ����Ÿ�� �߰������Ƿ� ��ũ�� ���� �� ReDraw �ÿ� call
#define UM_STOCKPRICE_SCROLL_REDRAW			WM_USER + 561

//��Ÿ �޼���
#define UM_SET_ETC							WM_USER + 571

//margin
//#define UM_GRAPH_MARGIN						WM_USER + 572

//�׷�������
#define UM_GRAPH_SELECTEDDATAINDEX			WM_USER + 578

//MainBlock
#define UDM_GET_USERMAGIN                   WM_USER + 582

//ocx -> main
#define	UDM_CHART_RELEASE_ALL				WM_USER + 599 // ���ο� ���õ� ���� ����
#define	UDM_CHART_ADD						WM_USER + 600	// ���� íƮ ����Ʈ�� íƮ OCX �ڵ��� �߰�/�����Ѵ�. 
#define UDM_GET_USERID						WM_USER+605	 //id ������
#define UDM_GET_USETOOLBAR					WM_USER+606	 //�ܺ����� ��뿩��
#define	UDM_TOOL_SELECT     				WM_USER+608 // ���ο� ���õ� ���� ����

// AddIn Message
#define UDM_ADDIN_MSG						WM_USER + 614	// wParam - AddIn Key String Pointer, lParam - AddIn Custom Value.

// (2007/1/6 - Seung-Won, Bae) Add Platform Menu
#define UDM_ADD_PLATFORM_MENU				WM_USER + 615	// wParam - 0, lParam - Chart Menu Handle.
#define UDM_CHECK_PLATFORM_MENU				WM_USER + 616	// wParam - Menu ID, lParam - 0

#endif //__MESSAGEDEFINE_H__