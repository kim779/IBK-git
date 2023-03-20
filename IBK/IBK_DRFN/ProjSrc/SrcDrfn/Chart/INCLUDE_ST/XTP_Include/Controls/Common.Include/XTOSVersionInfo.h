// XTOSVersionInfo.h: interface for the CXTOSVersionInfo class.
//
// This file is a part of the XTREME TOOLKIT MFC class library.
// �2004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTOSVERSIONINFO_H__)
#define __XTOSVERSIONINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// Summary: CXTOSVersionInfo is a OSVERSIONINFO derived class. This class wraps
//			the Win32 API GetVersionEx(...), used to get the current Windows OS
//			version. CXTOSVersionInfo is a single instance, or "singleton" object,
//			that is accessed with the Get() method.
class _XT_EXT_CLASS CXTOSVersionInfo : public OSVERSIONINFO  
{
public:

	// Example: <pre>bool bIsWinNT = CXTOSVersionInfo::Get().IsWinNT4();</pre>
	// Summary:	Call this member function to access the class members. Since this
	//			class is designed as a single instance object you can only access version
	//			info thru this static method. You <b>cannot</b> directly instantiate an object
	//			of type CXTOSVersionInfo.
	static CXTOSVersionInfo &Get();

	// Returns:	true if the OS is Windows 3.1, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			Windows 3.1.  
	bool IsWin31() const;

	// Returns: true if the OS is Windows 95, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			Windows 95.  
	bool IsWin95() const;

	// Returns: true if the OS is Windows 98, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			Windows 98.  
	bool IsWin98() const;

	// Returns: true if the OS is Windows ME, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			Windows ME.  
	bool IsWinME() const;

	// Returns: true if the OS is Windows NT 4, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			Windows NT 4.  
	bool IsWinNT4() const;

	// Returns: true if the OS is Windows 2000, otherwise returns false.
	// Summary:	Call this member function to check to see if the operating system is
	//			Windows 2000.  
	bool IsWin2K() const;

	// Returns: true if the OS is Windows XP, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			Windows XP.  
	bool IsWinXP() const;

	// Returns: true if the OS is greater than or equal to Windows 3.1, otherwise returns 
	//			false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			greater than or equal to Windows 3.1. 
	bool IsWin31OrGreater() const;

	// Returns: true if the OS is of the Windows 9x family, and is Windows 95 or
	//			a later version, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			of the Windows 9x family, and if it is Windows 95 or a later version. 
	bool IsWin95OrGreater() const;

	// Returns: true if the OS is of the Windows 9x family, and is Windows 98 or
	//			a later version, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			of the Windows 9x family, and if it is Windows 98 or a later version. 
	bool IsWin98OrGreater() const;

	// Returns: true if the OS is of the Windows 9x family, and is Windows ME or
	//			a later version, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			of the Windows 9x family, and if it is Windows ME or a later version. 
	bool IsWinMEOrGreater() const;

	// Returns: true if the OS is of the Windows NT family, and is Windows NT 4 or
	//			a later version, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			of the Windows NT family, and if it is Windows NT 4 or a later version. 
	bool IsWinNT4OrGreater() const;

	// Returns: true if the OS is of the Windows NT family, and is Windows 2000 or
	//			a later version, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			of the Windows NT family, and if it is Windows 2000 or a later version. 
	bool IsWin2KOrGreater() const;

	// Returns: true if the OS is of the Windows NT family, and is Windows XP or
	//			a later version, otherwise returns false.
	// Summary:	Call this member function to check whether or not the operating system is
	//			of the Windows NT family, and if it is Windows XP or a later version. 
	bool IsWinXPOrGreater() const;

private:

	// greater or equal a version number

	inline bool gte_ver(const DWORD maj, const DWORD min) const;

	inline bool eq_ver(const DWORD maj, const DWORD min) const;

    // Constructs a CXTOSVersionInfo object.
	CXTOSVersionInfo();
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTOSVersionInfo::IsWin31() const {
	return (dwPlatformId == VER_PLATFORM_WIN32s);
}
AFX_INLINE bool CXTOSVersionInfo::IsWin95() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4) && (dwMinorVersion < 10);
}
AFX_INLINE bool CXTOSVersionInfo::IsWin98() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && eq_ver(4, 10);
}
AFX_INLINE bool CXTOSVersionInfo::IsWinME() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && eq_ver(4, 90);
}
AFX_INLINE bool CXTOSVersionInfo::IsWinNT4() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_NT) && eq_ver(4, 0);
}
AFX_INLINE bool CXTOSVersionInfo::IsWin2K() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_NT) && eq_ver(5, 0);
}
AFX_INLINE bool CXTOSVersionInfo::IsWinXP() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_NT) && eq_ver(5, 1);
}
AFX_INLINE bool CXTOSVersionInfo::IsWin95OrGreater() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion >= 4);
}
AFX_INLINE bool CXTOSVersionInfo::IsWin98OrGreater() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && gte_ver(4, 10);
}
AFX_INLINE bool CXTOSVersionInfo::IsWinMEOrGreater() const {
	return (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && gte_ver(4, 90);
}
AFX_INLINE bool CXTOSVersionInfo::IsWinNT4OrGreater() const {
	return (dwPlatformId >= VER_PLATFORM_WIN32_NT) && (dwMajorVersion >= 4);
}
AFX_INLINE bool CXTOSVersionInfo::IsWin2KOrGreater() const {
	return (dwPlatformId >= VER_PLATFORM_WIN32_NT) && (dwMajorVersion >= 5);
}
AFX_INLINE bool CXTOSVersionInfo::IsWinXPOrGreater() const {
	return (dwPlatformId >= VER_PLATFORM_WIN32_NT) && gte_ver(5, 1);
}
AFX_INLINE bool CXTOSVersionInfo::gte_ver(const DWORD maj, const DWORD min) const {
	return (dwMajorVersion > maj)  || (dwMajorVersion == maj  &&  dwMinorVersion >= min);
}
AFX_INLINE bool CXTOSVersionInfo::eq_ver(const DWORD maj, const DWORD min) const {
	return (dwMajorVersion == maj)  &&  (dwMinorVersion == min);
}

//:Ignore
//////////////////////////////////////////////////////////////////////

#endif // !defined(__XTOSVERSIONINFO_H__)
//:End Ignore