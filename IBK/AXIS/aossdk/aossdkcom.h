/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 3845 $
 * 
 */

#ifndef AOSSDKCOM_H
#define AOSSDKCOM_H


/*****************************************************************************
 *
 *	IsRunningPd �Լ��� ���� ��ǰ ���� �ڵ� 
 *
 **************************************************************************/

#define AOSSDK_SERVICE_CODE_SB			0x00000004	// AOS Secure Browser
#define AOSSDK_SERVICE_CODE_FW			0x00000080	// AOS Firewall
#define AOSSDK_SERVICE_CODE_AK			0x00010000	// AOS Anti-Keylogger

/*****************************************************************************
 *
 *	Edit Ÿ���� �����մϴ�.
 *
 *	typeNumber : �� ���� �����Ǹ� Number �� ��츸 ó���մϴ�.
 * 
 **************************************************************************/
enum AosMkdEditType
{
	typeNumber	= 0x0001,
	typeChar	= 0x0002,
	typeSpecial = 0x0004
};

/*****************************************************************************
 *
 *	Edit Control �� ���� ��带 �����մϴ�.
 *
 *	modeUserInsert : �� ���� �����Ǹ� Insert Key�� ����� �� �ֽ��ϴ�. �� ���� 
 *		�����Ǹ� SetInsertMode �Լ��� ����Ͽ� Insert ���¸� ������ �� 
 *		�ֽ��ϴ�.
 *
 *	modeUseEsc : �� ���� �����Ǹ� ESC Key�� ����� �� �ֽ��ϴ�. ESC Ű�� ������ 
 *		���� ���� �޸𸮸� Clear��ŵ�ϴ�. ���� ���� Clear �ǹǷ� 
 *		ǥ�õǴ� ���� Clear ���Ѿ� �մϴ�.
 *
 *	modeDeleteLimit : �� ���� �����Ǹ� Ŀ���� �ε����� 0 �� ������ ��� 
 *	    Backspace�� ���� ������� �� ��� ���˴ϴ�. ������ Ŀ����
 *		�� �տ� �ִ� ��쿡�� Backspace�� ���� ���� �� ������ ������� �� ���� 
 *		�� ���� �����Ͽ��� �մϴ�.
 *
 *	modeProtectLevelLow : �� ���� �����Ǹ� Text Ÿ�Կ� ���Ͽ�
 *  ���� ������ ��ȣ�� �ϰ� �˴ϴ�.
 *  ��ȣ������ �����Ƿ� �Է°��� ���� ������ �뵵�� ����ϱ⸦ ���մϴ�. 
 * 
 ***************************************************************************/

enum AosMkdEditMode
{
	modeUseInsert		= 0x0001,
	modeUseEsc			= 0x0002,
	modeDeleteLimit		= 0x0004,
	modeProtectLevelLow	= 0x0008
};

/*****************************************************************************
 *
 *	AOS MKD �� �ʱ� ��带 �����մϴ�.
 *
 *	modeProtect : �� ���� �����Ǹ� ��ȣ ���� �����ϰ� �˴ϴ�.
 *
 *	modeBypass  : �� ���� �����Ǹ� ��ȣ ���� �������� �ʽ��ϴ�.
 * 
 ***************************************************************************/
 
enum AosMkdInitMode
{
	modeProtect		= 0x0001,
	modeBypass		= 0x0002
};

/*****************************************************************************
 *
 *	AOS MKD �� ��ȣ Ÿ���� �����մϴ�.

 *  typeAll : �� ���� �����Ǹ� �н�����, �ؽ�Ʈ�� ��� ��ȣ �մϴ�.
 *
 *	typePassword : �� ���� �����Ǹ� �н����常 ��ȣ �մϴ�.
 * 
 ***************************************************************************/

enum AosMkdEditCtrlProtectScope
{
	typeAll			= 0x0001,
	typePassword	= 0x0002
};

typedef struct stEditProtectSetting
{
	AosMkdEditCtrlProtectScope eProtectScope;
	int	nExceptionEditCtrl;
	HWND* pHWnd;
} EDITPROTECT_SET, *LPEDITPROTECT_SET;

/*****************************************************************************
 *
 *	AOS MKD Interface
 *
 ***************************************************************************/
 
class IAosMkdA
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, BOOL bIntegratedWithAos = FALSE) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, CHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST CHAR* lpszText) = 0;
	virtual DWORD _stdcall GetTextType(HWND hwnd, DWORD* pdwType) = 0;
	virtual DWORD _stdcall SetInsertMode(HWND hwnd, BOOL bInsert) = 0;
	virtual DWORD _stdcall Uninitialize2() = 0;
};

class IAosMkdW
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, BOOL bIntegratedWithAos = FALSE) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, WCHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST WCHAR* lpszText) = 0;
	virtual DWORD _stdcall GetTextType(HWND hwnd, DWORD* pdwType) = 0;
	virtual DWORD _stdcall SetInsertMode(HWND hwnd, BOOL bInsert) = 0;
	virtual DWORD _stdcall Uninitialize2() = 0;
};


class IAosMkdS4
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, HWND hMainWnd, BOOL bIntegratedWithAos) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall StartProtect() = 0;
	virtual DWORD _stdcall StopProtect() = 0;
	virtual DWORD _stdcall Uninitialize2() = 0;
};

class IAosMkd5
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, BOOL bIntegratedWithAos = FALSE) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectAllEditControl(HWND hWnd, LPEDITPROTECT_SET lpEdtiProtectSet=NULL) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall ProtectEditControl3(HWND hWnd) = 0;
};

class IAosMkd6
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, BOOL bIntegratedWithAos = FALSE) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall Uninitialize2() = 0;
	virtual DWORD _stdcall AddProtectWnd(HWND hWnd, BOOL bScanChild) = 0;
	virtual DWORD _stdcall RemoveProtectWnd(HWND hWnd, BOOL bScanChild) = 0;
};

/*****************************************************************************
 *
 *	AOS Verify Interface
 *
 ***************************************************************************/

class IAosVerify
{
public:
	virtual BOOL _stdcall IsRunningPd(DWORD dwServiceCode) = 0;
	virtual BOOL _stdcall VerifyTrustSelfA(LPCSTR szSignedName) = 0;
	virtual BOOL _stdcall VerifyTrustSelfW(LPCWSTR swSignedName) = 0;
	virtual BOOL _stdcall VerifyTrustSDK() = 0;
};

#endif // AOSSDKCOM_H
