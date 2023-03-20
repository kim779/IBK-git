// DumpLogger.h: interface for the CDumpLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUMPLOGGER_H__F88F14FB_C314_4FA9_8479_D0240992B75D__INCLUDED_)
#define AFX_DUMPLOGGER_H__F88F14FB_C314_4FA9_8479_D0240992B75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.07
//
//	����	:	������ Memory Dump Logging Library�� Class Header Library�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
class CDumpLogger : public CObject  
{
public:
	CDumpLogger( const char *p_szLogFileKey = "MDump", BOOL p_bMSecLogFile = FALSE, const char *p_szLogFolderPath = "C:\\Logs")
	{
		// 0. Parameter ���� �����ϱ� ���Ͽ�, ���� Variable�� �̿��Ѵ�.
		const char *szLogFileKey	= p_szLogFileKey;
		const char *szLogFolderPath	= p_szLogFolderPath;
		// 1. ���� Logging�ϴ� �ð��� Ȯ���Ѵ�.
		SYSTEMTIME sTime;
		GetLocalTime(&sTime);
		// 2. Log File Name�� �����Ѵ�.
		// 2.1 Log Folder Path�� Ȯ���Ѵ�.
		if( szLogFolderPath == NULL || *szLogFolderPath == '\0') szLogFolderPath = "C:\\Logs";
		// 2.2 Log File Key�� Ȯ���Ѵ�.
		if( szLogFileKey == NULL || *szLogFileKey == '\0') szLogFileKey = "MDump";
		// 2.3 Log File Name�� �����Ѵ�.
		CString strFileName;
		if( p_bMSecLogFile) strFileName.Format( "%s\\%s_%04d%02d%02d_%02d%02d%02d_%03d.log", szLogFolderPath, szLogFileKey, sTime.wYear, 
			sTime.wMonth, sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond, sTime.wMilliseconds);
		else strFileName.Format( "%s\\%s_%04d%02d%02d.log", szLogFolderPath, szLogFileKey, sTime.wYear, 
			sTime.wMonth, sTime.wDay);
		// 3. szLogFolderPath�� ������ �ʿ䰡 ������ �̸� ó���Ѵ�.
		::CreateDirectory( szLogFolderPath, NULL);

		// 3. Log File�� Open�Ѵ�.
		m_hLogFile = _lopen( ( LPCTSTR)strFileName, OF_READWRITE);
		if( m_hLogFile != HFILE_ERROR)
		{
			// 3.1 ���� File�� ������ File ������ �̵��Ѵ�.
			if( _llseek( m_hLogFile, 0L, FILE_END) == HFILE_ERROR) return;
		}
		// 3.2 ���� File�� ������ ������ �����Ѵ�.
		else m_hLogFile = _lcreat( ( LPCTSTR)strFileName, 0);
	}
	virtual ~CDumpLogger()
	{
		// 6. Dump File�� �ݴ´�.
		if( m_hLogFile != HFILE_ERROR) _lclose( m_hLogFile);
	}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.07
//
//	����	:	�⺻���� Log File Key�� LogFolderPath�� �����޾� LogFileName�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
protected:
	HFILE		m_hLogFile;			// Log File Handle

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.07
//
//	����	:	������ Logging File�� Dump Log�� ����ϴ� Interface�� �����Ѵ�.
//
//	����	:	1. �ٸ� Logger�� File�� ������ ��츦 �����Ͽ�, Constructor���� Open�Ͽ� �� ���¸� �������� �ʰ�,
//					Log Write�ø��� ��� ��� ó���Ѵ�.
//
//	����	:	(2003.07.08) 16 Byte Line Dump ó����, ������ Character�� Multi-Byte Character�� ���,
//					���� Character���� ������ �����Ͽ� ' '(Space)�� �ϳ� �� �߰���ĭ��.
///////////////////////////////////////////////////////////////////////////////
public:
	void WriteToDumpLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
	{
		if( m_hLogFile == HFILE_ERROR) return;

		// 1. ���� Logging�ϴ� �ð��� Ȯ���Ѵ�.
		SYSTEMTIME sTime;
		GetLocalTime(&sTime);

		// 4. �־��� Section Value�� Ȯ���Ͽ� Section String�� �����Ѵ�.
		CString strBuffer;
		if( p_szSection && *p_szSection != '\0')
		{
			CString strCurTime;
			strCurTime.Format("\r\n\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
				sTime.wMilliseconds);
			strBuffer = strCurTime + p_szSection + "]\r\n";
			_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
		}

		int i = 0;
		// 5. Memory�� Dump�Ѵ�.
		for(  i = 0; i < p_nDataLen; i++)
		{
			// 5.1 �⺻ Byte�� Dump�Ѵ�.
			strBuffer.Format( "%02x ", ( unsigned char)p_szData[ i]);
			_hwrite( m_hLogFile, ( LPCSTR)strBuffer, 3);

			// 5.2 16�ڸ��� Dump�� ��, �� Data�� ����Ѵ�.
			if( i % 16 == 15) 
			{
				_hwrite( m_hLogFile, "\t\t", 2);
				_hwrite( m_hLogFile, p_szData + i - 15, 16);
			
				// (2003.07.08) 16 Byte Line Dump ó����, ������ Character�� Multi-Byte Character�� ���,
				//		���� Character���� ������ �����Ͽ� ' '(Space)�� �ϳ� �� �߰���ĭ��.
				if( p_szData[ i] & 0x80) _hwrite( m_hLogFile, " ", 1);

				_hwrite( m_hLogFile, "\r\n", 2);
			}
		}
		// 5.3 ������ Byte Dump�� ���� �� Data�� ����Ѵ�.
		if( i % 16 != 0) 
		{
			_hwrite( m_hLogFile, "\t\t", 2);
			_hwrite( m_hLogFile, p_szData + i - i % 16, p_nDataLen - ( i - i % 16));

			// (2003.07.08) 16 Byte Line Dump ó����, ������ Character�� Multi-Byte Character�� ���,
			//		���� Character���� ������ �����Ͽ� ' '(Space)�� �ϳ� �� �߰���ĭ��.
			if( p_szData[ p_nDataLen - 1] & 0x80) _hwrite( m_hLogFile, " ", 1);

			_hwrite( m_hLogFile, "\r\n", 2);
		}
	}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.04.06
//
//	����	:	Dump�� �ƴ϶� �Ϲ� String Logging ����� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
public:
	void WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
	{
		if( m_hLogFile == HFILE_ERROR) return;

		// 1. ���� Logging�ϴ� �ð��� Ȯ���Ѵ�.
		SYSTEMTIME sTime;
		GetLocalTime(&sTime);

		// 4. �־��� Section Value�� Ȯ���Ͽ� Section String�� �����Ѵ�.
		CString strBuffer;
		if( p_szSection && *p_szSection != '\0')
		{
			CString strCurTime;
			// (2005/9/1 - Seung-Won, Bae) Line Skip for Only Section.
			strCurTime.Format("\r\n\r\n\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
				sTime.wMilliseconds);
			strBuffer = strCurTime + p_szSection + "]\r\n";
			_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
		}
		// (2005/9/2 - Seung-Won, Bae) in No Section Log, Insert Time in front of Line
		else
		{
			CString strCurTime;
			strCurTime.Format("%02d:%02d:%02d:%03d\t", sTime.wHour, sTime.wMinute, sTime.wSecond, 
				sTime.wMilliseconds);
			_hwrite( m_hLogFile, strCurTime, strCurTime.GetLength());
		}

		// 5. Memory�� Dump�Ѵ�.
		_hwrite( m_hLogFile, p_szData, p_nDataLen);
	}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.07
//
//	����	:	������ Memory Dump Logging Routine�� Static Member�� �����Ѵ�.
//
//	����	:	1. ���������� CDumpLogger Object�� �����Ͽ� ó������Ѵ�.
//
//	����	:	(2004.04.06, ��¿�) Dump�� �ƴ϶� �Ϲ� String Logging ��ɵ� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
public:
	static void DumpMemoryToFile( const char *p_szSection, const char *p_szData, int p_nDataLen, const char *p_szLogFileKey = "MDump", const char *p_szLogFolderPath = "C:\\Logs")
	{
		CDumpLogger dLogger( p_szLogFileKey, FALSE, p_szLogFolderPath);
		dLogger.WriteToDumpLog( p_szSection, p_szData, p_nDataLen);
	}
	static void LogStringToFile( const char *p_szSection, const char *p_szData, int p_nDataLen, const char *p_szLogFileKey = "MDump", const char *p_szLogFolderPath = "C:\\Logs")
	{
		CDumpLogger dLogger( p_szLogFileKey, FALSE, p_szLogFolderPath);
		dLogger.WriteToStringLog( p_szSection, p_szData, p_nDataLen);
	}
	static void LogStringToFile( const char *p_szSection, const char *p_szData, const char *p_szLogFileKey = "MDump", const char *p_szLogFolderPath = "C:\\Logs")
	{
		if( !p_szData) return;
		CDumpLogger dLogger( p_szLogFileKey, FALSE, p_szLogFolderPath);
		dLogger.WriteToStringLog( p_szSection, p_szData, strlen( p_szData));
	}
	static void LogTraceLineToFile( const char *p_szCodeFileName, int p_nCodeLine, const char *p_szLogFolderPath = "C:\\Logs")
	{
		if( !p_szCodeFileName) return;
		CString strLog;
		strLog.Format( "TRACE - %s[%d]\r\n", p_szCodeFileName, p_nCodeLine);
		CDumpLogger dLogger( "CodeTrace", FALSE, p_szLogFolderPath);
		dLogger.WriteToStringLog( NULL, strLog, strLog.GetLength());
	}
};

// (2007/3/19 - Seung-Won, Bae) Exception Log Macro
#define _DUMPLOGGER_EXCEPTION_LOG_DECLARATION	extern CString			g_strFunctionName;												\
												extern CString			g_strFileName;													\
												extern int				g_nLineNumber;													\
												extern CString			g_strLog;														\
												extern CDumpLogger		g_dLogger;														\
												extern CDumpLogger *	g_pLogger
#define _DUMPLOGGER_EXCEPTION_LOG_DEFINE		CString					g_strFunctionName;												\
												CString					g_strFileName;													\
												int						g_nLineNumber;													\
												CString					g_strLog;														\
												CDumpLogger g_dLogger( "CodeTrace", FALSE, "C:\\Logs");									\
												CDumpLogger *g_pLogger = NULL
#define _DUMPLOGGER_EXCEPTION_LOG_START( szFunctionName)																				\
												g_strFunctionName = szFunctionName;														\
												__try																					\
												{
#define _DUMPLOGGER_EXCEPTION_LOG_CHECK( szFileName, nLineNumber)																		\
													g_strFileName = szFileName;															\
													g_nLineNumber = nLineNumber;
#define _DUMPLOGGER_EXCEPTION_LOG_END			}																						\
												__except( EXCEPTION_EXECUTE_HANDLER)													\
												{																						\
													g_strLog.Format( "FileName : %s\r\nFunction Name: %s\r\nLine Number : %d\r\n",		\
														g_strFileName, g_strFunctionName, g_nLineNumber);								\
													g_dLogger.WriteToStringLog( "Chart Exception Log", g_strLog, g_strLog.GetLength());	\
													AfxMessageBox( "Unexpected error!\r\nSee code-trace log, please.");					\
													g_pLogger->WriteToStringLog( NULL, NULL, 0);										\
												}


#endif // !defined(AFX_DUMPLOGGER_H__F88F14FB_C314_4FA9_8479_D0240992B75D__INCLUDED_)
