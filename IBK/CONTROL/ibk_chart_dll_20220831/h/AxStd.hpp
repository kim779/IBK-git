// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information.
//  
//  �ۼ���        :  ������
//  ���          :  ǥ�� ���̺귯�� ���� �����Լ� ����
//  Components   :   AxStd.hpp	AXIS common 
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2019-01	Initial version
// *****************************************************************************


#pragma once

//----------------------------------------------------------
// FIXMEs / TODOs / NOTE macros
//----------------------------------------------------------
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE( x )   TRACE( x )
#define FILE_LINE   TRACE( __FILE__LINE__ "\n")
#define TODO( x )   TRACE( __FILE__LINE__"\n"                 \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   \
        "|  TODO :   " #x "\n"					\
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   )  
#define FIXME( x )  TRACE(  __FILE__LINE__"\n"                \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   \
        "|  FIXME :  " #x "\n"			                \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"  )
#define todo( x )   TRACE( __FILE__LINE__" TODO :   " #x "\n" ) 
#define fixme( x )  TRACE( __FILE__LINE__" FIXME:   " #x "\n" ) 
#define _LINE _T("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
#define XMSG( x )  TRACE(  __FILE__LINE__"\n"                 \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   \
	"|  " __FUNCTION__ "\n"\
	"|  XMSG :  " #x "\n"			                \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"  )

// ���ǹ� ��ũ��
#define IS_TRUE(c)              (c)
#define IS_FALSE(c)             (!(c))

#define IS_EXIST(c)             (c)
#define IS_NOT_EXIST(c)         (!(c))

#define IS_VALID(c)             (c)
#define IS_INVALID(c)           (!(c))

#define IS_SUCCEEDED(c)         (c)
#define IS_FAILED(c)            (!(c))

// ������ ��ũ��
#define IS_NULL(p)              (!(p))
#define IS_NOT_NULL(p)          (p)

#define IS_ZERO(n)              (!(n))
#define IS_NOT_ZERO(n)          (n)

// ���� �˻� ��ũ��
#define IS_WITHIN(min,max,expr)         (((min)<=(expr))&&((max)>=(expr)))
#define IS_WITHOUT(min,max,expr)        (((min)>(expr))||((max)<(expr)))
#define IS_WITHINx(min,max,expr)        (((min)<(expr))&&((max)>(expr)))
#define IS_WITHOUTx(min,max,expr)       (((min)>=(expr))||((max)<=(expr)))

#ifndef SAFE_WAIT
#define SAFE_WAIT(size, parr) while (size != parr->GetSize()){ size = parr->GetSize(); Sleep(1); }
#endif

#include <execution>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nb30.h>
#include <Iphlpapi.h>
#include <locale>
#include <codecvt>
#include <memory>
#include <string>
#include <functional>
#include <stack>
#include <gsl/gsl>
#include <mutex>
#include <thread>
#include <any>
#include <optional>
#include <variant>
#include "ppl.h"
#include <random>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <future>
#include <queue>
#include <thread>
#include <sal.h>
#include <VersionHelpers.h>
#include <axdll.hpp>


using namespace std;
using namespace concurrency;

#pragma warning(disable : 26814)	// gap�� ������ �ð� ���
#pragma warning(disable : 4003)		// ��ũ��ȣ�� �μ� ����
#pragma warning(disable : 26486)        // �Լ��� �߸��� ������ ����
#pragma warning(disable : 26489)        // �߸��� ������ ��ȯ
#pragma warning(disable : 4003)	        // ��ũ�� ȣ��� �μ� ����
#pragma warning(disable : 26434)	// ���߾� �Լ��� �θ��Լ��� ����
#pragma warning(disable : 26490)	// reinterpret_cast �� ������� ����
#pragma warning(disable : 26485)	// �迭���� �����ͷ� ���� (CString(char[15])) ����Ҷ� ��� �߻�
#pragma warning(disable : 26487)	// �߸��� �����͸� ��ȯ���� ������.
#pragma warning(disable : 26481)	// ������ ������  span�� ����ϼ���.
#pragma warning(disable : 26812)	// enum  ��� enum Ŭ������ ����ϼ���.
#pragma warning(disable : 26433)	// override ǥ��
#pragma warning(disable : 26493)	// c ��Ÿ�� ĳ��Ʈ�� ������� ������.
#pragma warning(disable : 6284)		// format ����...
#pragma warning(disable : 26444)	// �Ķ���ͷ� ������� ��ü ������
#pragma warning(disable : 6340)		// format ����...
#pragma warning(disable : 26414)	// �����Լ��� virtual �Լ��� �����ϴ�.
#pragma warning(disable : 6031)		// ��ȯ���� ���õǾ����ϴ�.
#pragma warning(disable : 26440)	// virtual funciton
#pragma warning(disable : 6258)		// TerminateThread�� ����ϸ� �����尡 ����� �������� �ʽ��ϴ�.
#pragma warning(disable : 28251)
#pragma warning(disable : 26426)
#pragma warning(disable : 26455)
#pragma warning(disable : 26451)
#pragma warning(disable : 26454)
#pragma warning(disable : 26482)
#pragma warning(disable : 26446)
#pragma warning(disable : 26461)
#pragma warning(disable : 26429)
#pragma warning(disable : 4996)

typedef BOOL(WINAPI* RtlGetVersion_FUNC) (OSVERSIONINFOEXW*);
static  ULONGLONG g_begin;

namespace AxStd
{
#pragma warning(disable : 26492)
#pragma warning(disable : 26477)
	inline void AFXAPI _Msg(TCHAR* fmt = _T(""), ...)
	{
//#ifdef _DEBUG
		CString	tmpstr, str;

		va_list	valist;
		va_start(valist, fmt);
		tmpstr.FormatV(fmt, valist);
		va_end(valist);
		const ULONGLONG end = GetTickCount64();
		if (tmpstr == _T(""))
		{
			str = _LINE;
			g_begin = end;
		}
		else
		{
			const CTime time = CTime::GetCurrentTime();
			CString sTime;

			sTime.Format(_T("[%ld ms]"), gsl::narrow_cast<int>(end - g_begin));
			str = _T("<Debug>") + sTime + tmpstr;
		}
		OutputDebugString(str + _T("\n"));
		

		/*
			ofstream dFile(_T("c:\\Debug.log"), std::ios::app);
			dFile << str << endl;
			dFile.close();
		*/
//#endif
	};

	inline void AFXAPI _Msg(CString sMsg)
	{
		_Msg("%s", sMsg);
	}

	inline CString AFXAPI FORMAT(const TCHAR* formatString, ...)
	{
		CString s;
		va_list	valist;

		va_start(valist, formatString);
		s.FormatV(formatString, valist);
		va_end(valist);
		return s;
	};

#pragma warning(default : 26492)
#pragma warning(default : 26477)

	inline CString Parser(CString &src, LPCTSTR sub = _T("\t"))
	{
		CString tmps;
		const int pos = src.Find(sub);
		
		if (pos == -1)
		{
			tmps = src;
			src.Empty();
			return tmps;
		}

		tmps = src.Left(pos);
		src = src.Mid(pos + _tcslen(sub));
		return tmps;
	}

	inline int SplitSub(vector<CString> &elems, CString& srcstr, CString substr)
	{
		while (!srcstr.IsEmpty())
			elems.push_back(Parser(srcstr, substr));

		return elems.size();
	}

	inline int Split(vector<CString> &elems, const wstring &s, WCHAR delim)
	{
		wstringstream ss(s);
		wstring item;
		while (getline(ss, item, delim))
			elems.push_back(CString(item.c_str()));

		return elems.size();
	}

	inline void xxCopy(gsl::span<char> org, CString src)
	{
		CopyMemory(org.data(), src, org.size());
	}

	inline BOOL GetVersion(OSVERSIONINFOEX* os) {
		HMODULE hMod{};
		RtlGetVersion_FUNC func{};

		OSVERSIONINFOEXW o{};
		OSVERSIONINFOEXW* osw = &o;

		hMod = LoadLibrary(TEXT("ntdll.dll"));
		if (hMod)
		{
			func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
			if (func == 0)
			{
				FreeLibrary(hMod);
				return FALSE;
			}
			ZeroMemory(osw, sizeof(*osw));
			osw->dwOSVersionInfoSize = sizeof(*osw);
			func(osw);

			os->dwBuildNumber = osw->dwBuildNumber;
			os->dwMajorVersion = osw->dwMajorVersion;
			os->dwMinorVersion = osw->dwMinorVersion;
			os->dwPlatformId = osw->dwPlatformId;
			os->dwOSVersionInfoSize = sizeof(*os);
		}
		else
			return FALSE;
		FreeLibrary(hMod);
		return  TRUE;
	}

	inline BOOL isDirExist(const CString& path)
	{
		return PathIsDirectory(path);
	}
	
	inline BOOL isFileExist(const CString& path)
	{
		return PathFileExists(path);
	}
	
	inline int AFXAPI SplitString(CStringArray& arr, LPCTSTR lpszFullString, LPCTSTR lpszSep)
	{
		if (lpszFullString == nullptr)     return FALSE;
		CString str;
		const int SepLen = lstrlen(lpszSep);
		LPCTSTR lpchEnd = lpszFullString;
		while (lpchEnd != nullptr)
		{
			lpchEnd = _tcsstr(lpszFullString, lpszSep);
			const int nLen = (lpchEnd == nullptr) ? lstrlen(lpszFullString) : (int)(lpchEnd - lpszFullString);
			ASSERT(nLen >= 0);
			str = CString(lpszFullString, nLen);
			lpszFullString += SepLen + nLen;       // point past the separator
			arr.Add(str);
		}
		return arr.GetSize();
	};

	
	/// Ư�� ������ ���� fullpath
	inline CString getWinPath(REFKNOWNFOLDERID rfid)	// Knownsfolders.h ����  FOLDERID_RoamingAppData
	{
		CString sPath;
		PWSTR path;
		const HRESULT hr = SHGetKnownFolderPath(rfid , 0, nullptr, &path);
		if (SUCCEEDED(hr)) {
			sPath = path;
			CoTaskMemFree(path);	
		}
		return sPath;
	}
	
	static CString getErrMsg()
	{	
		// OSVERSIONINFOEXA   VersionInformation{};
		// VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		// VersionInformation.dwMajorVersion = 6;
		// VersionInformation.dwMinorVersion = 2;
		// DWORD              dwTypeMask = VER_MAJORVERSION | VER_MINORVERSION;
		// DWORDLONG          dwlConditionMask = 0;
		
		// VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
		// VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
		
		// if(!VerifyVersionInfo(&VersionInformation, dwTypeMask, dwlConditionMask))
		// {
		// 	MessageBoxs(������8�� �������� �ʽ��ϴ�.);
		// }


		CString sMsg;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), sMsg.GetBuffer(256), 0, nullptr);
		sMsg.ReleaseBuffer();
		sMsg.TrimRight(_T(" \t\r\n"));
		return sMsg;
	}

	inline void addComma(CString &data)
	{
		CString tmps, sRight;
		int pos{}, length{};

		sRight = data;
		pos  = sRight.Find('.') + 1;
		tmps = Parser(sRight, _T("."));
		length = tmps.GetLength();
		if (length < 4)
			return;
			
		data.Empty();
		for (int ii = 0; ii < length; ii++)
		{
			if ((length - ii) % 3 == 0 && ii != 0)
				data += ',';
			data += tmps.GetAt(ii);
		}

		if (!sRight.IsEmpty())
			data += _T(".") + sRight;
	}
	
	// type : 0 �� �ڵ� ��������
	//      : 1 �� �̸� ��������
	inline void getArray(vector<std::pair<CString, CString>>& vArr, int type)
	{
		DllInvoker codedll("AxisCode.dll");	
		if (codedll.IsLoaded())
		{
			std::map<CString, std::pair<CString, int>>* map =
				(std::map<CString, std::pair<CString, int>>*) codedll.Function<char* WINAPI(int)>("getArray")(0);

			for_each(map->begin(), map->end(), [&vArr](const auto item) {
				vArr.emplace_back(std::make_pair(item.first, item.second.first));
			});

			if (type == 1)
			{
				std::sort(vArr.begin(), vArr.end(), [](const auto item1, const auto item2) {
					return (item1.second.Compare(item2.second) < 0);
				});
			}
				
		}
	}

	inline CString getName(CString sCode)
	{
		DllInvoker codedll("AxisCode.dll");
		if (codedll.IsLoaded())
		{
			CString sName = (char*)codedll.Function<const char* WINAPI(const char*)>("getName")(sCode.GetString());
			return sName;
		}
		return "";
	}

	inline CString getCode(CString sName)
	{
		DllInvoker codedll("AxisCode.dll");
		if (codedll.IsLoaded())
		{
			CString sCode = (char*)codedll.Function<const char* WINAPI(const char*)>("getCode")(sName.GetString());
			return sCode;
		}
		return "";
	}

	struct AxDeleter
	{
		template<typename T>
		void operator () (T* p) noexcept
		{
			delete[] p;
		}
	};
}

