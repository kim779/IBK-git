/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 926 $
 * 
 */

#ifndef AOSSDKERR_H
#define AOSSDKERR_H


//
//	����
//
#define AOSSDK_ERROR_SUCCESS											0x00000000L
																		
//
//	MessageText	: ��Ų ������ �޾ƿü� �����ϴ�.\n��� �Ŀ� �ٽ� �õ����ּ���.
//
#define AOSSDK_ERROR_DOWNLOAD_SKIN										0x32A10000L

//
//	MessageText	: ���������� �о������ ���߽��ϴ�.
//
#define AOSSDK_ERROR_OPEN_AUTHFILE										0x32A10001L

//
//	MessageText	: ��ǰ �¾� ahn.ui ������ �м��� �����߽��ϴ�.
//
#define AOSSDK_ERROR_VALID_AHNUI										0x32A10002L

//
//	MessageText	: ��ǰ �¾� ahn.ui ������ �޾ƿü� �����ϴ�.\n��� �Ŀ� �ٽ� �õ����ּ���.
//
#define AOSSDK_ERROR_DOWNLOAD_AHNUI										0x32A10003L

//
//	MessageText	: ��ǰ �¾��� ������ �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_CHECK_SETUP										0x32A10005L

//
//	MessageText	: AhnUpEx.dll ȣ���� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_LOAD_AHNUPEX										0x32A10006L

//
//	MessageText	: AspUp.dll ȣ���� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_LOAD_ASPUP											0x32A10007L

//
//	MessageText	: ��Ų ������ �޾ƿü� �����ϴ�.\n��� �Ŀ� �ٽ� �õ����ּ���.
//
#define AOSSDK_ERROR_DOWNLOAD_SKIN2										0x32A10008L

//
//	MessageText	: ASPSnc.dll ȣ���� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_LOAD_SUPDATEI										0x32A10009L

//
//	MessageText	: ���������� ������ ���� ���߽��ϴ�.
//
#define AOSSDK_ERROR_READ_AUTHFILE										0x32A1000aL

//
//	MessageText	: ���������� �޾ƿü� �����ϴ�. \n��� �Ŀ� �ٽ� �õ����ּ���.
//
#define AOSSDK_ERROR_DOWNLOAD_AUTHFILE									0x32A1000bL

//
//	MessageText	: ��ǰ���� ���������� �����Ҽ� �����ϴ�.
//
#define AOSSDK_ERROR_SAVE_AUTHFILE2										0x32A1000dL

//
//	MessageText	: �̹� ������Ʈ�� �������Դϴ�. (SUiPre)
//
#define AOSSDK_ERROR_EXIST_UPDATE										0x32A1000eL

//
//	MessageText	: �������� �ʴ� OS�̱� ������ ������ �Ұ����մϴ�.
//
#define AOSSDK_ERROR_ENABLE_OS											0x32A10012L

//
//	MessageText	: Administrator ������ ���� �������� �α����߽��ϴ�.\r\n
//					Administrator ������ ������ ���α׷��� ��ġ�ϰų� ����� �� �����ϴ�.
//					\r\nAdministrator ������ ���� ����� �������� �α����Ͻʽÿ�.
//
#define AOSSDK_ERROR_ENABLE_PRIVILEGE									0x32A10013L

//
//	MessageText	: ������ �޾ƿü� �����ϴ�.\n��� �Ŀ� �ٽ� �õ����ּ���.
//
#define AOSSDK_ERROR_DOWNLOAD_FILE										0x32A10014L

//
//	MessageText	: ������Ʈ ���α׷��� ahn.ui ������ �м��� �����߽��ϴ�.
//
#define AOSSDK_ERROR_VALID_AHNUI2										0x32A10015L

//
//	MessageText	: Helper.dll ȣ���� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_LOAD_HELPER										0x32A10017L

//
//	MessageText	: ������Ʈ ���α׷��� ����� �޾ƿü� �����ϴ�.
//
#define AOSSDK_ERROR_DOWNLOAD_SUPDATEI									0x32A10018L

//
//	MessageText	: ������Ʈ ���α׷� �¾��� �޾ƿü� �����ϴ�.
//
#define AOSSDK_ERROR_DOWNLOAD_SUPDATEISETUP								0x32A1001aL

//
//	MessageText	: ������Ʈ ���α׷� �¾��� ������ �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_CHECK_SUPDATEISETUP								0x32A1001bL

//
//	MessageText	: ������Ʈ ���α׷� ����� ������ �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_CHECK_SUPDATEIMODULE								0x32A1001dL

//
//	MessageText	: ��ǰ������ ������ �ֽ��ϴ�.
//
#define AOSSDK_ERROR_PRODUCT_INFO										0x32A1001eL

//
//	MessageText	: %1 ������ ������ ���� �����ϴ�.
//
#define AOSSDK_ERROR_EXECUTE											0x32A1001fL

//
//	MessageText	: ����Ǯ�⸦ �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_EXTRACT											0x32A10020L

//
//	MessageText	: ������Ʈ ���α׷��� ����̵��� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_MOVEFILE											0x32A10021L

//
//	MessageText	: �����带 �������� ���߽��ϴ�.
//
#define AOSSDK_ERROR_BEGINTHREAD2										0x32A10022L

//
//	MessageText	: ���������� ��ȿ�Ⱓ�� �������ϴ�.
//
#define AOSSDK_ERROR_EXPIRED_AUTHFILE									0x32A10023L

//
//	MessageText	: Smart Update i ��ġ ���α׷����� ������ �߻��߽��ϴ�.
//
#define AOSSDK_ERROR_EXECUTE_SUPDATEISETUP2								0x32A10024L

//
//	MessageText	: ���������� ������ ���� ���߽��ϴ�.
//
#define AOSSDK_ERROR_READ_AUTHFILE2										0x32A10025L

//
//	MessageText	: �������� �ʴ� �̿��� �����̱� ������ ������ �Ұ����մϴ�.
//
#define AOSSDK_ERROR_ENABLE_PRIVILEGE2									0x32A10026L

//
//	MessageText	: �������� �ʴ� ������ Microsoft Internet Explorer �Դϴ�.
//
#define AOSSDK_ERROR_ENABLE_IE											0x32A10027L

//
//	MessageText	: �������� �ʴ� CPU �Դϴ�.
//
#define AOSSDK_ERROR_ENABLE_CPU											0x32A10028L

//
//	MessageText	: ���� ���� ������ �����ϴ�.
//
#define AOSSDK_ERROR_ENABLE_WRITE										0x32A10029L

//
//	MessageText	: �����带 �������� ���߽��ϴ�.
//
#define AOSSDK_ERROR_BEGINTHREAD										0x32A10032L

//
//	MessageText	: suimain.exe �� �������� ���߽��ϴ�.
//
#define AOSSDK_ERROR_EXECUTE_SUIMAIN									0x32A10033L

//
//	MessageText	: Not Allowed URL
//
#define AOSSDK_ERROR_NOT_ALLOWED_URL									0x32A10035L

//
//	MessageText	: The operation was canceled by the user.
//
#define AOSSDK_ERROR_CANCELLED											0x32A10036L

//
//	MessageText	: Not Allowed Process
//
#define AOSSDK_ERROR_NOT_ALLOWED_PROCESS								0x32A10037L

//
//	MessageText	: ���� ��忡�� �������� �ʽ��ϴ�.
//
#define AOSSDK_ERROR_SAFE_BOOT										 	0x32A10038L

//
//	MessageText	: �� �� ���� ������ �߻�
//
#define AOSSDK_ERROR_UNSUPPORT											0x32A10039L

//
//  �ʱ�ȭ �������� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_UNINITIALIZE_FAILED								0x32A41001L

//
// 	SbSdk.dll, Mkd25.dll, Mkd25def.dll ����� �ε��ϴµ� �����Ͽ����ϴ�.
//				
#define AOSSDK_ERROR_LOADLIBRARY_FAILED									0x32A41002L

//
// 	�ʱ�ȭ���� �ʾҽ��ϴ�.
//
#define AOSSDK_ERROR_NOT_INITIALIZED									0x32A41007L
				
//
// 	�Ķ���Ͱ� �̻��մϴ�.
//
#define AOSSDK_ERROR_BAD_PARAMETER										0x32A41008L

//
// 	�ʱ�ȭ ���� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_INITIALIZE_FAILED									0x32A41009L

//
// 	AOS MKD ����� �ε���� �ʾҽ��ϴ�.
//
#define AOSSDK_ERROR_NOT_LOADLIBRARY									0x32A4100AL

//
// ������Ʈ�� �������� �ʽ��ϴ�.
//
#define AOSSDK_ERROR_UPDATE_NOT_SUPPORTED								0x32A4100BL

//
// ������Ʈ ���� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_UPDATE_FAILED										0x32A4100CL

//
// ������Ʈ �Ķ���Ͱ� �̻��մϴ�.
//
#define AOSSDK_ERROR_UPDATE_BAD_PARAMETER								0x32A4100DL

//
// ������Ʈ ����� �ε��ϴµ� �����Ͽ����ϴ�.
//
#define AOSSDK_ERROR_UPDATE_LOADLIBRARY_FAILED							0x32A4100EL

//
// AOS MKD�� ��ġ���� �ʾҽ��ϴ�.
//
#define AOSSDK_ERROR_NOT_INSTALLED										0x32A4100FL

//
// AOS MKDȣȯ�� ��ְ� �߻��߽��ϴ�.
// ��� MKD ������ǰ�� �����ϰ� �ٽ� �����Ͻʽÿ�.
//
#define AOSSDK_ERROR_DRIVER_RULE_NOT_VERIFYED							0x32A41010L

//
// ��ȣ���� �ʴ� Edit Control �Դϴ�.				
//
#define AOSSDK_ERROR_NOT_PROTECTED_HWND									0x32A41011L

//
// �Ӽ��� ��ġ���� �ʽ��ϴ�.
//
#define AOSSDK_ERROR_NOT_PROPERTY_HWND									0x32A41012L

//
// 	SbSdk.dll, Mkd25.dll, Mkd25def.dll ��⿡�� �������̽��� ã�� �� �����ϴ�.
//				
#define AOSSDK_ERROR_GETPROCADDRESS_FAILED								0x32A41003L

#endif