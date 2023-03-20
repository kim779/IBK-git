/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision 926 $
 * 
 */

#ifndef __STSDKEXERR_H_
#define __STSDKEXERR_H_


//	MessageText : ����
#define STSDKEX_ERROR_SUCCESS											0x00000000L

//	MessageText : UNINITIALIZE�� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_UNINITIALIZE_FAILED								0x32A41001L

//	MessageText : SDK ����� �ε��ϴµ� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_LOADLIBRARY_FAILED								0x32A41002L

//	MessageText : SDK ��⿡�� �������̽��� ã�� �� �����ϴ�.
#define STSDKEX_ERROR_GETPROCADDRESS_FAILED								0x32A41003L

//	MessageText : ����� ���۽�Ű�µ� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_FUNCTION_START_FAILED								0x32A41004L

//	MessageText : ����� �����Ű�µ� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_FUNCTION_STOP_FAILED								0x32A41005L

//	MessageText : ��ǰ�� �̹� ��ġ�Ǿ� �ֽ��ϴ�.
#define STSDKEX_ERROR_ALREADY_INSTALLED									0x32A41006L

//	MessageText : �ʱ�ȭ ���� �ʾҽ��ϴ�.
#define STSDKEX_ERROR_NOT_INITIALIZED									0x32A41007L

//	MessageText : �Ķ���Ͱ� �̻��մϴ�.
#define STSDKEX_ERROR_BAD_PARAMETER										0x32A41008L

//	MessageText : �ʱ�ȭ�� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_INITIALIZE_FAILED									0x32A41009L

//	MessageText : SDK ����� �ε�Ǿ� ���� �ʽ��ϴ�.
#define STSDKEX_ERROR_NOT_LOADLIBRARY									0x32A4100AL

//	MessageText : ����� �ٸ� ������ ������̹Ƿ� ������ ���� �����ϴ�.
#define STSDKEX_ERROR_REMAIN_SHAREDCOUNT								0x32A4100BL

//	MessageText : ����� �̹� ����� �����Դϴ�.
#define STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED							0x32A4100CL

//	MessageText : ���� ID�� �߸� �Ǿ����ϴ�.
#define STSDKEX_ERROR_INVALID_CUSTOMER_ID								0x32A4100DL

//	MessageText : ��� �ڵ尡 �߸� �Ǿ����ϴ�.
#define STSDKEX_ERROR_INVALID_FUNCTION_CODE								0x32A4100EL

//	MessageText : ��ǰ�� ��ġ���� �ʾҽ��ϴ�.
#define STSDKEX_ERROR_PRODUCT_NOT_INSTALLED								0x32A4100FL

//	MessageText : ����̹� ���� ��ȿ���� �ʽ��ϴ�.
#define STSDKEX_ERROR_DRIVER_RULE_NOT_VERIFYED							0x32A41010L

//	MessageText : ��ȣ���� �ʴ� Edit Control�Դϴ�.
#define STSDKEX_ERROR_NOT_PROTECTED_HWND								0x32A41011L

//	MessageText : �Ӽ��� ��ȿ���� �ʽ��ϴ�.
#define STSDKEX_ERROR_NOT_PROPERTY_HWND									0x32A41012L

//	MessageText : ���μ��� ID�� �߸� �����Ǿ����ϴ�.
#define STSDKEX_ERROR_INVALID_PROCESS_ID								0x32A41013L

//	MessageText : �ٿ�ε��� ���ϼ��� �����ϴµ� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_DOWNLOAD_SETTING_FAILED							0x32A41014L

//	MessageText : ������Ʈ ���α׷� �¾��� �޾ƿü� �����ϴ�.
#define STSDKEX_ERROR_DOWNLOAD_UPDATESETUP								0x32A41015L

//	MessageText : ������Ʈ ���α׷� ��� ������ �����Ͽ����ϴ�.
#define STSDKEX_ERROR_CHECK_UPDATEMODULE								0x32A41016L

//	MessageText : ������ �ùٸ��� �Ǿ� ���� �ʽ��ϴ�.
#define STSDKEX_ERROR_SIGN_NOT_VERIFYED									0x32A41017L

//	MessageText : �Լ��� �����ϴµ� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_FUNCTION_FAILED									0x32A41018L

//	MessageText : ��ŷ��ȣ ����� ���α�� �ڵ尡 �߸� �Ǿ����ϴ�.
#define STSDKEX_ERROR_INVALID_SUB_FUNCTION_CODE							0x32A41019L

//	MessageText : ����� ���������� �ʽ��ϴ�.
#define STSDKEX_ERROR_FUNCTION_NOT_RUNNING								0x32A4101AL

//	MessageText : ����� �̹� �������� �����Դϴ�.
#define STSDKEX_ERROR_FUNCTION_ALREADY_STARTED							0x32A4101BL

//	MessageText : ���� ��å����(���)�� �ٿ�ε� �ϴµ� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_CUSTOMER_POLICY_DOWNLOAD_FAILED					0x32A4101CL

//	MessageText : ���� ��å����(�ɼ�)�� �ٿ�ε� �ϴµ� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_OPTION_POLICY_DOWNLOAD_FAILED						0x32A4101DL

//	MessageText : �������μ��� ����Ʈ�� �߸� �����Ǿ����ϴ�.
#define STSDKEX_ERROR_INVALID_EXCEPTION_PROCESS_LIST					0x32A4101EL

//	MessageText : �켱����(�浹 ���α׷� ����, Super��)�� ���� ���� ���� �����Ͽ� ��û�� ������ �� �����ϴ�.
#define STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE					0x32A4101FL

//	MessageText : ��� ���ῡ �����Ͽ����ϴ�.
#define STSDKEX_ERROR_CONNECT_FAILED									0x32A41020L

//	MessageText : �ý��� �Լ� ȣ�⿡ �����Ͽ����ϴ�.
#define STSDKEX_ERROR_FUNCTION_CALL_FAILED								0x32A41021L

//	MessageText : STSESS ���࿡ �����Ͽ����ϴ�.
#define STSDKEX_ERROR_STSESS_EXEC_FAILED								0x32A41022L

//	MessageText : STSESS ������ Timeout �Ǿ����ϴ�.
#define STSDKEX_ERROR_STSESS_TIMEOUT_FAILED								0x32A41023L

//	MessageText : AsdSvc �� ���������� �ʽ��ϴ�.
#define STSDKEX_ERROR_ASDSVC_NOT_RUNNING								0x32A41024L

//	MessageText : ASTx ������Ʈ ���Դϴ�.
#define STSDKEX_ERROR_ASTX_UPDATING										0x32A41025L

//	MessageText : CUSTOMER ID�� NULL�Դϴ�.
#define STSDKEX_ERROR_NULL_CUSTOMER_ID									0x32A41026L	

//	MessageText : ActiveConsoleSessionId�� ���� ���߽��ϴ�.
#define STSDKEX_ERROR_GET_ACTIVE_CONSOLE_SESSION_ID						0x32A41027L	

//  MessageText : STSDKEX_Initialize()���� ���� ȣ��Ǿ�� �մϴ�.
#define STSDKEX_ERROR_CALL_AFTER_INIT									0x32A41028L	

//  MessageText : �������� �ʴ� �Լ��Դϴ�.
#define STSDKEX_ERROR_CALL_NOT_IMPLEMENTED								0x32A41029L	

//  MessageText : �ݹ��Լ��� NULL�Դϴ�.
#define STSDKEX_ERROR_PB_NULL_CALLBACK									0x32A41030L	

//  MessageText : HSBCTL�� NULL�Դϴ�.
#define STSDKEX_ERROR_PB_NULL_HSBCTL									0x32A41031L	

//  MessageText : ȣȯ�� ��尡 �����Ǿ� �ֽ��ϴ�.
#define STSDKEX_ERROR_PB_COMPATIBLITY_MODE								0x32A41032L	

//  MessageText : CreateOptions�� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_PB_CREATEOPTIONS_FAILED							0x32A41033L	

//  MessageText : ����̹� ���翡 �����Ͽ����ϴ�.
#define STSDKEX_ERROR_PB_COPYDRIVER_FAILED								0x32A41034L	

//  MessageText : ����̹� �ʱ�ȭ�� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_PB_HSBDRV_INITIALIZE_FAILED						0x32A41035L	

//  MessageText : ����̹� ������ �����Ͽ����ϴ�.
#define STSDKEX_ERROR_PB_HSBDRV_START_FAILED							0x32A41036L	

//  MessageText : ����̹� �������μ��� ������ �����Ͽ����ϴ�.
#define STSDKEX_ERROR_PB_HSBDRV_INSERTEXCEPTIONPROCESS_FAILED			0x32A41037L	

//  MessageText : ����̹� ����Ȯ�ο� �����Ͽ����ϴ�.
#define STSDKEX_ERROR_PB_HSBDRV_GETDRIVERVERSION_FAILED					0x32A41038L	

//  MessageText : ����̹� �ε忡 �����Ͽ����ϴ�.
#define STSDKEX_ERROR_PB_HSBDRV_LOADDRIVER_FAILED						0x32A41039L	

//	MessageText : ������Ʈ ���α׷� ������ Timeout �Ǿ����ϴ�
#define STSDKEX_ERROR_UPDATE_TIMEOUT									0x32A41040L


#endif	// __STSDKEXERR_H_
