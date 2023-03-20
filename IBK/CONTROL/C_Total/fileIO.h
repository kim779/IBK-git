#pragma once

class CFileIO  
{
public:
	CFileIO();
	virtual ~CFileIO();

public:
	static bool	ReadFile(CWnd *pView, CString path, char *pBytes, int nBytes);
	static bool	WriteFile(CWnd *pView, CString path, char *pBytes, int nBytes);
	static bool	GetFilenameInPath(CString &strPath, CString name, CStringArray &aryFname);
	static BOOL	ImageWindow(CWnd *pMain, CString& path);
	static void	PrintWindow(CWnd *pMain, bool bDlgPopup);
	static HANDLE	DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal);
	static BOOL	WriteDIB(LPTSTR szFile, HANDLE hDIB);
};