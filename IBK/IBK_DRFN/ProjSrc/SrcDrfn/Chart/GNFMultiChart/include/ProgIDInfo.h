// ProgIDInfo.h: interface for the CProgIDInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGIDINFO_H__E53E14DB_B03D_4A22_A69C_6322EE4BCAC6__INCLUDED_)
#define AFX_PROGIDINFO_H__E53E14DB_B03D_4A22_A69C_6322EE4BCAC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <objbase.h>	// for CLSIDFromProgID()
#include <afxpriv.h>	// for USES_CONVERSION

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.05
//
//	����	:	ProgID�� ���� ������ ���ϴ� Object�̴�.
//
//	����	:	1. ũ�� ���� ������� Header Library�� �����Ѵ�.
//				2. Ư�� Object�� �����Ǵ� ���� �����Ƿ�, Static Member Routine�� �̿��ϴ� ������ �Ѵ�.
///////////////////////////////////////////////////////////////////////////////
class CProgIDInfo : public CObject  
{
public:
	CProgIDInfo();
	virtual ~CProgIDInfo();

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.05
//
//	����	:	ProgID�� ClassID�� ��ȯ�ϴ� Routine�� �����Ѵ�.
//
//	ERROR	:	[CLSIDFromProgID]	0,	Success, S_OK in ::CLSIDFromProgID()
//									-1,	CO_E_CLASSSTRING in ::CLSIDFromProgID()
//									-2, REGDB_E_WRITEREGDB in ::CLSIDFromProgID()
//									-3, Other Error in ::CLSIDFromProgID()
//				[ProgIDFromCLSID]	0,	Success, S_OK in ::ProgIDFromCLSID()
//									-1,	REGDB_E_CLASSNOTREG in ::ProgIDFromCLSID()
//									-2, REGDB_E_READREGDB in ::ProgIDFromCLSID()
//									-3, Other Error in ::ProgIDFromCLSID()
///////////////////////////////////////////////////////////////////////////////
public:
	static long CLSIDFromProgID( const char *p_szProgID, LPCLSID p_pClsID)
	{
		USES_CONVERSION;
		LPCOLESTR szOleProgID = T2COLE( p_szProgID);
		HRESULT hResult = ::CLSIDFromProgID( szOleProgID, p_pClsID);
		if( hResult == S_OK) return 0;
		else if( hResult == CO_E_CLASSSTRING) return -1;
		else if( hResult == REGDB_E_WRITEREGDB) return -2;
		return -3;
	}
	static long ProgIDFromCLSID( const CLSID p_clsID, CString &p_strProgID)
	{
		LPOLESTR oleProgID;
		HRESULT hResult = ::ProgIDFromCLSID( p_clsID, &oleProgID);
		if( hResult == REGDB_E_CLASSNOTREG) return -1;
		else if( hResult == REGDB_E_READREGDB) return -2;
		else if( hResult != S_OK) return -3;
		USES_CONVERSION;
		p_strProgID = OLE2T( oleProgID);
		CoTaskMemFree( oleProgID);
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.05
//
//	����	:	ClassID�� String���� ��ȣ ��ȯ�ϴ� Routine�� �����Ѵ�.
//
//	ERROR	:	[StringFromCLSID]	0,	Success, S_OK in ::StringFromCLSID()
//									-1,	Error in ::StringFromCLSID()
//				[CLSIDFromString]	0,	Success, NOERROR in ::CLSIDFromString()
//									-1,	CO_E_CLASSTRING in ::CLSIDFromString()
//									-2, REGDB_E_WRITEREGDB in ::CLSIDFromString()
//									-3, Other Error in ::CLSIDFromString()
///////////////////////////////////////////////////////////////////////////////
public:
	static long StringFromCLSID( const CLSID p_clsID, CString &p_strClsID)
	{
		LPOLESTR oleClsID;
		HRESULT hResult = ::StringFromCLSID( p_clsID, &oleClsID);
		if( hResult != S_OK) return -1;
		USES_CONVERSION;
		p_strClsID = OLE2T( oleClsID);
		CoTaskMemFree( oleClsID);
		return 0;
	}
	static long CLSIDFromString( const char *p_szClsID, LPCLSID p_pClsID)
	{
		USES_CONVERSION;
		LPOLESTR szOleClsID = T2OLE( p_szClsID);
		HRESULT hResult = ::CLSIDFromString( szOleClsID, p_pClsID);
		if( hResult == NOERROR) return 0;
		else if( hResult == CO_E_CLASSSTRING) return -1;
		else if( hResult == REGDB_E_WRITEREGDB) return -2;
		return -3;
	}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.05
//
//	����	:	ClassID String�� ProgID�� ��ȣ ��ȯ�ϴ� Routine�� �����Ѵ�.
//
//	ERROR	:	[StringCLSIDFromProgID]		0,		Success
//											-X1,	Error in CProgIDInfo::CLSIDFromProgID()
//											-X2,	Error in CProgIDInfo::StringFromCLSID()
//				[ProgIDFromStringCLSID]		0,		Success
//											-X1,	Error in CProgIDInfo::CLSIDFromString()
//											-X2,	Error in CProgIDInfo::ProgIDFromCLSID()
///////////////////////////////////////////////////////////////////////////////
public:
	static long StringCLSIDFromProgID( const char *p_szProgID, CString &p_strClsID)
	{
		// 1. ���� CLSID�� ���Ѵ�.
		CLSID clsID;
		LONG lResult = CLSIDFromProgID( p_szProgID, &clsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. String CLSID�� ���Ѵ�.
		lResult = StringFromCLSID( clsID, p_strClsID);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}
	static long ProgIDFromStringCLSID( const char *p_szClsID, CString &p_strProgID)
	{
		// 1. ���� CLSID�� ���Ѵ�.
		CLSID clsID;
		LONG lResult = CLSIDFromString( p_szClsID, &clsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. ProgID�� ���Ѵ�.
		lResult = ProgIDFromCLSID( clsID, p_strProgID);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.05
//
//	����	:	String ClassID�� CLSID�� �ش��ϴ� Module�� ���(COM Server Path)�� ���Ѵ�.
//
//	����	:	1. ProgID, CLSID�ε� ���� �� �ֵ��� �����Ѵ�.
//
//	ERROR	:	[ComServerFromStringCLSID]	0,		Success
//											-1,		Error in RegOpenKeyEx() (No Inproc/Local Server Key ?)
//											-2,		No ComServer Value
//											-3,		ComServer Path has No EXT.
//											-4,		There is no ComServer File.
//				[ComServerFromCLSID]		0,		Success
//											-X1,	Error in CProgIDInfo::StringFromCLSID()
//											-X2,	Error in CProgIDInfo::ComServerFromStringCLSID()
//				[ComServerFromProgID]		0,		Success
//											-XX1,	Error in CProgIDInfo::StringCLSIDFromProgID()
//											-X2,	Error in CProgIDInfo::ComServerFromStringCLSID()
///////////////////////////////////////////////////////////////////////////////
public:
	static long ComServerFromStringCLSID( const char *p_szClsID, CString &p_strComServer)
	{
		// 0. Key String�� ���Ѵ�.
		CString strSubKey;
		char *szSubKey = strSubKey.GetBufferSetLength( 6 + 38 + 15);
		strcpy( szSubKey, "CLSID\\");
		strcpy( szSubKey + 6, p_szClsID);
		strcpy( szSubKey + 6 + 38, "\\InprocServer32");
		// 1. Key�� Open�Ѵ�.
		HKEY hKey;
		if( RegOpenKeyEx( HKEY_CLASSES_ROOT, szSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		{
			// 1.1 InprocServer32�� �ƴ϶�� LocalServer32�� Ȯ���Ѵ�.
			strcpy( szSubKey + 6 + 38, "\\LocalServer32");
			if( RegOpenKeyEx( HKEY_CLASSES_ROOT, szSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
				return -1;
		}
		// 2. ComServer�� ��θ� ������ ���Ѵ�.(Default Value�̴�.)
		DWORD dwDataType, dwReceivedSize = _MAX_PATH;
		CString strShortServerPath;
		char *szShortServerPath = strShortServerPath.GetBufferSetLength( dwReceivedSize);
		RegQueryValueEx( hKey, NULL, 0, &dwDataType, ( unsigned char *)szShortServerPath, &dwReceivedSize);
		// 3. Key�� �ݴ´�.
		RegCloseKey( hKey);
		// 4. ������ Com Server Path�� Ȯ���Ѵ�.
		if( !*szShortServerPath) return -2;
		// 5. Module ���� Option�� �����Ѵ�.
		char *szExtension = strrchr( szShortServerPath, '.');
		if( !szExtension) return -3;
		szExtension[ 4] = '\0';
		// 6. Long File Path�� ���Ѵ�.
		char *szLongServerPath = p_strComServer.GetBuffer( _MAX_PATH);
		DWORD dwLongPath = GetLongPathName( szShortServerPath, szLongServerPath, _MAX_PATH);
		p_strComServer.ReleaseBuffer();
		if( !dwLongPath) return -4;
		// 7. Success�� Return�Ѵ�.
		return 0;
	}
	static long ComServerFromCLSID( const CLSID p_clsID, CString &p_strComServer)
	{
		// 1. ���� String CLSID�� ���Ѵ�.
		CString strClsID;
		long lResult = StringFromCLSID( p_clsID, strClsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. String CLSID�� Com Server�� ���Ѵ�.
		lResult = ComServerFromStringCLSID( strClsID, p_strComServer);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}
	static long ComServerFromProgID( const char *p_szProgID, CString &p_strComServer)
	{
		// 1. ���� String CLSID�� ���Ѵ�.
		CString strClsID;
		long lResult = StringCLSIDFromProgID( p_szProgID, strClsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. String CLSID�� Com Server�� ���Ѵ�.
		lResult = ComServerFromStringCLSID( strClsID, p_strComServer);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.08.05
//
//	����	:	ProgID�� Vender Part���� �̿��Ͽ� �ش��ϴ� Module�� ���(Com Server Path)�� ���Ѵ�.
//
//	����	:	1. ProgID�� <Vender>.<Component>.<Version>���� �����Ǿ� �ִ�.
//				2. ���� <Vender>���� <Component>���� �ϳ��� Module�� �����ȴٴ� �����Ͽ�
//					�־��� <Vender>�� ProgID�� �ϳ� �˻��Ͽ� (Registry�κ���) ComServer�� ���ϵ��� �Ѵ�.
//
//	ERROR	:	[ProgIDFromVender]			0,		Success
//											-1,		Error in RegQueryInfoKey() (No HKEY_CLASSS_ROOT Key ?)
//											-2,		ProgID of Vender Not Found in HKEY_CLASSS_ROOT
//				[ComServerFromVender]		0,		Success
//											-X1,	Error in CProgIDInfo::ProgIDFromVender()
//											-XXX2,	Error in CProgIDInfo::ComServerFromProgID()
//
//	����	:	(2003.08.06) HKEY_CLASSS_ROOT������ ProgID�� �˻��� Sequence Search���� Binary Search�� �����Ѵ�.
//					1. �⺻ Variable
//						1.1 B(Base)		: 2�� �˻��� �����ϱ� ���� �⺻ Index�̴�.
//											(ex, B=5�̸� 5������ Index���� �˻� ����� ���ٴ� ���̴�.)
//						1.2 N(Elements)	: 2�� �˻��� ������ ���� ��� Element�� �����̴�.
//											Ư�� B(Base) Index�� �̻�(B ����) N���� ���� ���Ѵ�.
//					2. �⺻ Algorithm
//						2.1 �⺻���� Algorithm�� �������� Binary Search�� �⺻���� �Ѵ�.
//						2.2 �׷��� �������� ��������� B(Base) Index�� 2���� ������ �̵��ϸ鼭 ���� ���� 2���� ���� ������ �������� �ʰ�,
//							���� 2���� ���� �����ϸ鼭 ������ �˻������ ���� ��쿡�� B(Base) Index�� ���� �̵��ϴ� ����� ���Ѵ�.
//						2.3 ���� ��� 0 ~ 19������ 20�� Index�� �ִ� ���, 12.5�� �˻��ϴ� ��츦 ���� ���� ���� ������ ����.
//										Classical Binary Search			Precheck Binary Search
//							Step 0		B=10, N=10						B=0, N=20
//							Step 1		Compare B (10), B=15, N=4		Compare B+N/2 (10), B=10, N=9
//							Step 2		Compare B (15), B=13, N=2		Compare B+N/2 (15), B=10, N=4
//							Step 3		Compare B (13), B=12, N=1		Compare B+N/2 (13), B=10, N=1
//							Step 4		Compare B (12), N=0, Not Found	Compare B+N/2 (12), B=13, N=0 Not Found
//						2.4 �������� ���� B���� ��ȭ�� ���� ���� �˻����� B�� Ȯ�ε� ��쿡�� ���������Ǵ� ������� �� �� �ִ�.
//					3. Algorithm
//						3.1 B=0, N=(Element ����)
//						3.2 N!=0�� ���� ������ �ݺ��Ѵ�.
//							3.2.0 Compare (B + N / 2)
//							3.2.1 =0														-> Found, Exit
//							3.2.2 <0, N=N/2
//								(N�� ���� �˻� Element�� ������, 2���� �Ѵ�.)
//							3.2.3 >0, B=N/2+1, N=N/2-1+(N%2)
//								(B�� ���� �˻� ����� ù Index�� ����Ű��,
//								 Ư�� N�� ����ġ���� ������ ���� Element 1���� �����ϰ�,
//								 ���� ���� N�� Ȧ���� ����� 2���� �������� �߰��Ѵ�.)
//						3.3 N!=0�̸� �˻� �����̴�.											-> Not Found, Exit
///////////////////////////////////////////////////////////////////////////////
public:
	static long ProgIDFromVender( const char *p_szVender, CString &p_strProgID)
	{
		// 1. Class Root Registry Key�� Open�Ѵ�.
		//	(HKEY_CLASSES_ROOT�� SubKey�� Handling�ϴ� ������ �̹� HKEY_CLASSES_ROOT�� Open�Ǿ� �ִ�.)
		// 2. Sub Key�� ���̸� Ȯ���Ѵ�.
		DWORD dwSubKeyCount, dwSubKeyMaxLen;
		if( RegQueryInfoKey( HKEY_CLASSES_ROOT, NULL, NULL, NULL, &dwSubKeyCount, &dwSubKeyMaxLen, NULL, 
			NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) return -1;
		// 2.1 NULL Character�� �����Ͽ� 1�� �߰��Ѵ�.
		dwSubKeyMaxLen++;
		// 3. Sequence Search�� �ش� Vender�� ProgID�� �˻��Ѵ�.
		// 3.0 SubKey Name�� ���� Buffer�� Ȯ���Ѵ�.
		char *szSubKeyName = p_strProgID.GetBuffer( dwSubKeyMaxLen);
		// 3.1 Vender Name�� ���Ѵ�.
		//	(Full Vender Name�� �ǵ��� '.'�� ���̰�, �񱳰� �����ϵ��� �빮�ڷ� ��ȯ�Ѵ�.)
		CString strVenderP( p_szVender);	// Vender Name with '.'
		strVenderP += ".";
		strVenderP.MakeUpper();
		// 3.2 �� Sub Key�� ���Ͽ� Ȯ���Ѵ�.
		LONG lResult;
		DWORD dwSubKeySize;
		FILETIME ftLastWriteTime;
// (2003.08.06) HKEY_CLASSS_ROOT������ ProgID�� �˻��� Sequence Search���� Binary Search�� �����Ѵ�.
//		// 3.2.0 Sequence Search�� �����Ѵ�.
//		for( DWORD i = 0, dwResult = ERROR_SUCCESS; dwResult == ERROR_SUCCESS; i++) 
//		{ 
//			// 3.2.1 dwSubKeySize�� I/O ����̹Ƿ� �Ź� �ʱ�ȭ�Ѵ�.
//			dwSubKeySize = dwSubKeyMaxLen;
//			// 3.2.2 Sub Key�� ���Ѵ�.
//			lResult = RegEnumKeyEx( HKEY_CLASSES_ROOT, i, szSubKeyName, &dwSubKeySize, NULL, NULL, NULL, &ftLastWriteTime); 
//			ASSERT( dwSubKeySize < dwSubKeyMaxLen);
//			if( lResult != ERROR_SUCCESS) break;
//			// 3.2.3 �̹� ������ Sub Key�� �˻� ���� ���Ѵ�.
//			// 3.2.3.1 �빮���� �񱳰� �ǵ���, Upper Case�� ��ȯ�Ѵ�.
//			strupr( szSubKeyName);
//			// 3.2.3.2 Key�� ���Ѵ�.
//			if( strcmp( strVenderP, szSubKeyName) <= 0) break;
//		}
		// 3.2.0 Binary Search�� �����Ѵ�.
		// 3.2.1 B=0, N=(Element ����)
		int nB = 0, nN = dwSubKeyCount, nCompare, nVenderNameLen = strVenderP.GetLength();
		// 3.2.2 N!=0�� ���� ������ �ݺ��Ѵ�.
		while( nN != 0)
		{
			// 3.2.2.0 Compare (B + N / 2)
			// 3.2.2.1 dwSubKeySize�� I/O ����̹Ƿ� �Ź� �ʱ�ȭ�Ѵ�.
			dwSubKeySize = dwSubKeyMaxLen;
			// 3.2.2.2 Sub Key�� ���Ѵ�.
			lResult = RegEnumKeyEx( HKEY_CLASSES_ROOT, nB + nN / 2, szSubKeyName, &dwSubKeySize, NULL, NULL, NULL, &ftLastWriteTime); 
			ASSERT( dwSubKeySize < dwSubKeyMaxLen);
			if( lResult != ERROR_SUCCESS) break;
			// 3.2.2.3 �̹� ������ Sub Key�� �˻� ���� ���Ѵ�.
			// 3.2.2.3.1 �빮���� �񱳰� �ǵ���, Upper Case�� ��ȯ�Ѵ�.
			strupr( szSubKeyName);
			// 3.2.2.3.2 Key�� ���Ѵ�.
			nCompare = strncmp( strVenderP, szSubKeyName, nVenderNameLen);
			// 3.2.2.1 =0	-> Found, Exit
			if( nCompare == 0) break;
			// 3.2.2.2 <0, N=N/2
			//	(N�� ���� �˻� Element�� ������, 2���� �Ѵ�.)
			if( nCompare < 0) nN = nN / 2;
			// 3.2.2.3 >0, B=B+N/2+1, N=N/2-1+(N%2)
			//	(B�� ���� �˻� ����� ù Index�� ����Ű��,
			//	 Ư�� N�� ����ġ���� ������ ���� Element 1���� �����ϰ�,
			//	 ���� ���� N�� Ȧ���� ����� 2���� �������� �߰��Ѵ�.)
			else
			{
				nB = nB + nN / 2 + 1;
				nN = nN / 2 - 1 + ( nN % 2);
			}
// [DEBUG] �˻��ǰ� �񱳵Ǵ� ������ Ȯ���� �� �ֵ��� MessageBox�� ����Ͽ� ����.
//			CString strTraceMsg;
//			strTraceMsg.Format( "ComparedProgID=%s, Base=%d, N=%d", szSubKeyName, nB, nN);
//			AfxMessageBox( strTraceMsg);
		}
		// 3.3 Class Root Registry Key�� �ݴ´�.
		//	(HKEY_CLASSES_ROOT�� SubKey�� Handling�ϴ� ������ �̹� HKEY_CLASSES_ROOT�� Open�Ǿ� �ִ�.)
		// 4. ProgID�� Return�Ѵ�.
		p_strProgID.ReleaseBuffer();
		// 4.1 �˻� ������ ��츦 ó���Ѵ�.
		if( lResult != ERROR_SUCCESS || strncmp( p_strProgID, strVenderP, strVenderP.GetLength()))
		{
			p_strProgID.Empty();
			return -2;
		}
		// 4.2 �˻� ������ ��츦 Return�Ѵ�.
		return 0;
	}
	static long ComServerFromVender( const char *p_szVender, CString &p_strComServer)
	{
		// 0. ���� ProgID�� ���Ѵ�.
		CString strProgID;
		long lResult = ProgIDFromVender( p_szVender, strProgID);
		if( lResult < 0) return lResult * 10 - 1;
		// 1. ProgID�� Com Server�� ���Ѵ�.
		lResult = ComServerFromProgID( strProgID, p_strComServer);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}
};

#endif // !defined(AFX_PROGIDINFO_H__E53E14DB_B03D_4A22_A69C_6322EE4BCAC6__INCLUDED_)
