#ifndef TRS_5000_LIB_H
#define TRS_5000_LIB_H

#include <windows.h>


////////////////////////////////////////////////////////
//  TRS Library�� ���� ���ŵǴ� �޼��� ��鷯�� �����մϴ�.
//  �ڼ��� ������ DEMO���α׷��� �����Ͻʽÿ�.
#define WM_TRS5000MSG              WM_USER+656

#define MINORID_PASSWD                       1
#define MINORID_TRSENABLE                    2
#define MINORID_TRSDISABLE                   3
#define MINORID_SESSIONCLOSE                 4


#define RCPHONE_SUCCESS                      1
#define RCPHONE_FAIL                         0
////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////
//   ������� : ���� ����� ���� FALSE�� ���ϵǴ� ����
//              ��Ʈ�� Ŭ������ �ٽ� �����ֽʽÿ�

/////////////////////////////////////////////////////////////
// TRS Initialize (Reserved)
// ���� Ư���� ����� �����ϴ�.
extern "C" EXPORT int  PASCAL PHONEPAD_InitDevice(void);



/////////////////////////////////////////////////////////////
// TRS�� �Է� ��й�ȣ�� ���̸� ���մϴ�.
//       �Է� ���ð��� ���մϴ�. 
//parameter    : len     - �Է� ��й�ȣ�� ����(0~16)
//                       - 1~16�� �Է¹޴��ڸ��� fixed
//                       - 0�� �Է¹��� ��й�ȣ �ڸ��� ���� * or # (�ִ� 16��)
//               timeout - �Է� ���ð��� ���մϴ�.(�ִ� 60, ������ �ʴ���)
//return Value : 1  ����
//               0 ����
extern "C" EXPORT int  PASCAL PHONEPAD_SetConfig(int len,int timeout);



/////////////////////////////////////////////////////////////
// TRS�� �Է� ���� ��� ��ȣ�� �޾ƿɴϴ�. 
//parameter    : data : String Buffer
//
//return Value : 1  ����
//               0 ����
extern "C" EXPORT int  PASCAL PHONEPAD_GetPassWD(char * data);


/////////////////////////////////////////////////////////////
// TRS�� �Է� ���� ��� ��ȣ�� �޾ƿɴϴ�. 
//parameter    : 
//
//return Value : String Data  ����
//               NULL         ����
extern "C" EXPORT char *  PASCAL PHONEPAD_GetPassWDSTR(void);


/////////////////////////////////////////////////////////////
// TRS�� ���¸� Ȯ���մϴ�.
//parameter    : 
//
//return Value : 0         normal status
//               1         password
//               2         timeout or cancel
extern "C" EXPORT int  PASCAL PHONEPAD_Status(void);


/////////////////////////////////////////////////////////////
// TRS�� ��Ʈ�� �����մϴ�.
// parameter   : no - �Ű����� ������ 0���� �ϸ� �ڵ����� ��Ʈ�� ã���ϴ�.
//                    �̿� 1~255�� �ش���Ʈ�� �����մϴ�.
//return Value : 1  ����
//               0 ����
extern "C" EXPORT int  PASCAL PHONEPAD_PortOpen(int no);



/////////////////////////////////////////////////////////////
// TRS�� ��Ʈ�� Ŭ���� �մϴ�.
extern "C" EXPORT void  PASCAL PHONEPAD_PortClose(void);



///////////////// 1.8.0.1 just Koscom Surpported
// Koscom������ ����մϴ�.(���ο��� ������� ���ʽÿ�)
// �Ķ���ͷδ� ������ ��������� �Ѱ��ֽø�˴ϴ�.
// �޼������� WM_TRS5000MSG : �޼��� ������
//              �����׸� WPARAM : MINORID_TRSENABLE  : �Է¹�ư
//                              : MINORID_TRSDISABLE : ��ҹ�ư
extern "C" EXPORT int  PASCAL PHONEPAD_GetPWSignalWindow(HWND hWnd);
///////////////// 1.8.0.1 just Koscom Surpported

#endif
