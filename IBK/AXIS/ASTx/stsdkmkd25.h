#ifndef __STSDKMKD25_H_
#define __STSDKMKD25_H_


/*****************************************************************************
 *
 *	Edit Ÿ���� �����մϴ�.
 *
 *	AstxtypeNumber : �� ���� �����Ǹ� Number �� ��츸 ó���մϴ�.
 * 
 **************************************************************************/
enum AstxMkdEditType
{
	AstxtypeNumber				= 0x0001,
	AstxtypeChar				= 0x0002,
	AstxtypeSpecial				= 0x0004
};

/*****************************************************************************
 *
 *	Edit Control �� ���� ��带 �����մϴ�.
 *
 *	AstxmodeUserInsert : �� ���� �����Ǹ� Insert Key�� ����� �� �ֽ��ϴ�. 
 *
 *	AstxmodeUseEsc : �� ���� �����Ǹ� ESC Key�� ����� �� �ֽ��ϴ�. ESC Ű�� ������ 
 *		���� ���� �޸𸮸� Clear��ŵ�ϴ�. ���� ���� Clear �ǹǷ� 
 *		ǥ�õǴ� ���� Clear ���Ѿ� �մϴ�.
 *
 *	AstxmodeDeleteLimit : �� ���� �����Ǹ� Ŀ���� �ε����� 0 �� ������ ��� 
 *	    Backspace�� ���� ������� �� ��� ���˴ϴ�. ������ Ŀ����
 *		�� �տ� �ִ� ��쿡�� Backspace�� ���� ���� �� ������ ������� �� ���� 
 *		�� ���� �����Ͽ��� �մϴ�.
 *
 *	AstxmodeProtectLevelLow : �� ���� �����Ǹ� Text Ÿ�Կ� ���Ͽ�
 *  ���� ������ ��ȣ�� �ϰ� �˴ϴ�.
 *  ��ȣ������ �����Ƿ� �Է°��� ���� ������ �뵵�� ����ϱ⸦ ���մϴ�. 
 * 
 ***************************************************************************/

enum AstxMkdEditMode
{
	AstxmodeUseInsert			= 0x0001,
	AstxmodeUseEsc				= 0x0002,
	AstxmodeDeleteLimit			= 0x0004,
	AstxmodeProtectLevelLow		= 0x0008
};

/*****************************************************************************
 *
 *	AhnLab Safe Transaction MKD �� �ʱ� ��带 �����մϴ�.
 *
 *	AstxmodeProtect : �� ���� �����Ǹ� ��ȣ ���� �����ϰ� �˴ϴ�.
 *
 *	AstxmodeBypass  : �� ���� �����Ǹ� ��ȣ ���� �������� �ʽ��ϴ�.
 *
 *	AstxmodeProtectSafeNet  : �� ���� �����Ǹ� SafeNet ���� �����մϴ�. 
 *
 *	AstxmodeProtectInSafeNet  : �� ���� �����Ǹ� InSafeNet ���� �����մϴ�.
 * 
 *	AstxmodeProtectInAstx  	: �� ���� �����Ǹ� ASTx ���� ���� �����մϴ�. 
 * 
 ***************************************************************************/
 
enum AstxMkdInitMode
{
	AstxmodeProtect				= 0x0001,
	AstxmodeBypass				= 0x0002,
	AstxmodeProtectSafeNet		= 0x0004,
	AstxmodeProtectInSafeNet	= 0x0008,
	AstxmodeProtectInAstx		= 0x0010
};

/*****************************************************************************
 *
 *	ASTx MKD Interface
 *
 ***************************************************************************/

typedef int (__stdcall* PFN_AKSDK_Callback)(long lCode, LPARAM lParam);

class IAstxMkd8A
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, CHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST CHAR* lpszText) = 0;
	virtual DWORD _stdcall GetEncText(DWORD dwCustomCode, HWND hWnd, CHAR* szOutput, DWORD* pdwSize, BYTE bzRandom[]) = 0;
};

class IAstxMkd8W
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ProtectEditControl(HWND hWnd) = 0;
	virtual DWORD _stdcall ProtectEditControl2(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength) = 0;
	virtual DWORD _stdcall GetText(HWND hWnd, WCHAR* szOutput, DWORD* pdwSize) = 0;
	virtual DWORD _stdcall SetText(HWND hWnd, CONST WCHAR* lpszText) = 0;
	virtual DWORD _stdcall GetEncText(DWORD dwCustomCode, HWND hWnd, WCHAR* szOutput, DWORD* pdwSize, BYTE bzRandom[]) = 0;

};

class IAstxAkA
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall GetEncText(IN DWORD dwCustomCode, IN CHAR* szE2EWindowID, IN CHAR* szE2EFormID, IN CHAR* szE2EInputID, OUT CHAR* szOutput, OUT DWORD* pdwSize, OUT BYTE bzRandom[]) = 0;

};

class IAstxAkW
{
public:
	virtual DWORD _stdcall Initialize(int nInitMode, PFN_AKSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall GetEncText(IN DWORD dwCustomCode, IN WCHAR* szE2EWindowID, IN WCHAR* szE2EFormID, IN WCHAR* szE2EInputID, OUT WCHAR* szOutput, OUT DWORD* pdwSize, OUT BYTE bzRandom[]) = 0;

};

#endif // __STSDKMKD25_H_
