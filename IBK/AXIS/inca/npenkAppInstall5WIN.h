// npenkAppInstall5WIN.h : ��� ����
//

#pragma once


class npenkAppInstall5WIN
{
public:
	// �����ڵ�
	static const DWORD eErr_True;	// ����, ����, TRUE
	static const DWORD eErr_False;	// ����, ����, FALSE
	static const DWORD eErr_NoFunc;	// DLL ���ο� �Լ��� ����
	static const DWORD eErr_FailLoadLibrary;	// DLL �ε� ����
	static const DWORD eErr_TimeOver;	// �ð� �ʰ�
	static const DWORD eErr_NoDoc;	// XML ���� �б� ����.
	static const DWORD eErr_NoNode;	// XML ���� ���� �� NODE �б� ����.
	static const DWORD eErr_FailAuthFileName;	// ���� ���� ����. ( ������ �ջ� �Ǿ��ų� �ʿ��� ������ ���� ���� �ʽ��ϴ�. )


private:
#ifdef _UNICODE 
	typedef DWORD (WINAPI *fnStart)( WCHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnStartXGuard)( WCHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnShutdownXGuard)( WCHAR *szCustomID );
#else
	typedef DWORD (WINAPI *fnStart)( CHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnStartXGuard)( CHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnShutdownXGuard)( CHAR *szCustomID );
#endif
	typedef DWORD (WINAPI *fnKillMain)();
	typedef DWORD (WINAPI *fnIsMainRunning)();
	typedef DWORD (WINAPI *fnIsXGuardRunning)();
	typedef DWORD (WINAPI *fnUninit)();

public:
	npenkAppInstall5WIN();
	~npenkAppInstall5WIN();

	/**
	* \brief		DLL ��� ����
	* \author		leesangwoo
	* \date			2011.02.23
	* \param		strDllPath	(AppInstall5.dll�� ��ġ)
	* \return		eErr_True(����), �� ��(����)
	*/
	DWORD			setDllPath( const TCHAR *szDllPath );

	/**
	* \brief		���� �����ϱ�
	* \author		kimdaehee
	* \date			2010.06.18
	* \param		strCustomID	��ü���̵�(��å���ϸ�)
	* \param		dwWaitTime	(�ִ�) ���� ��� �ð�
	*							-1		: ���� ���(sync)
	*							0		: ������� ����(async)
	*							1 �̻�	: �ִ� dwWaitTime millisecond ��ŭ ��ٸ�
	* \return		eErr_True(����), �� ��(����)
	*/
	DWORD			start( TCHAR *szCustomID, DWORD dwWaitTime );

	/**
	* \brief		���� �����ϱ�(���� ���ǿ��� caller �� ���ܽ�Ŵ)
	* \author		kimdaehee
	* \date			2010.06.18
	* \return		eErr_True(����), �� ��(����)
	*/
	DWORD			killMain();

	/**
	* \brief		������ �������ΰ�?
	* \author		kimdaehee
	* \date			2010.06.18
	* \return		eErr_True(������), �� ��(���������� ����)
	*/
	DWORD			isMainRunning();

	/**
	* \brief		XGuard ����
	* \author		leesangwoo
	* \date			2010.10.21
	* \param		strCustomID	��ü���̵�(��å���ϸ�)
	* \return		eErr_True(����), �� ��(����)
	*/
	DWORD			startXGuard( TCHAR *szCustomID, DWORD dwWaitTime );

	/**
	* \brief		XGuard ����
	* \author		leesangwoo
	* \date			2010.10.21
	* \return		eErr_True(����), �� ��(����)
	*/
	DWORD			shutdownXGuard( TCHAR *szCustomID );

	/**
	* \brief		XGuard �������ΰ�?
	* \author		leesangwoo
	* \date			2011.02.21
	* \return		eErr_True(����), �� ��(����)
	*/
	DWORD			isXGuardRunning();

	/**
	* \brief		AppInstaller ��� �ߴ�
	* \author		kiminsu
	* \date			2011.04.22
	* \return		eErr_True(����)
	*/
	DWORD			uninit();


	DWORD			uninitDll();
private:
	DWORD			initDll();
	

	HINSTANCE			hDll_;
	TCHAR				szDllPath_[255];
	fnStart				fnStart_;
	fnKillMain			fnKillMain_;
	fnIsMainRunning		fnIsMainRunning_;
	fnStartXGuard		fnStartXGuard_;
	fnShutdownXGuard	fnShutdownXGuard_;
	fnIsXGuardRunning	fnIsXGuardRunning_;
	fnUninit			fnUninit_;
};
