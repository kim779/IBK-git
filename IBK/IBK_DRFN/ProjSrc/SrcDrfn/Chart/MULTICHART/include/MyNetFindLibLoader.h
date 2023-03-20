// LoadMyNetFindLib.h: interface for the CLoadMyNetFindLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADMYNETFINDLIB_H__CC5B59D9_4FDE_4C23_850A_8F9E34BEBA85__INCLUDED_)
#define AFX_LOADMYNETFINDLIB_H__CC5B59D9_4FDE_4C23_850A_8F9E34BEBA85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//# HWND hSelfWnd => �ڽ��� ���� ���
//# HWND hWndParent => ���� View �Ǵ� ������ ���� ����Դϴ�.
typedef void (*_DLL_SetLibOpen)(HWND hSelfWnd, HWND hWndParent);
typedef void (*_DLL_SetLibClose)(HWND hSelfWnd);
//# [���] ������ ����, lpCode[8], lpName[21]
typedef int (*_DLL_GetPortList)(HWND hSelfWnd, char *pFullFilePath, int nSize, LPSTR &lpCode, LPSTR &lpName);
//# [���] lpData�� ������ ������
typedef int (*_DLL_GetFindDefine)(HWND hSelfWnd, char *pFullFilePath, int nSize, LPSTR &lpData);
//=>[�߰�] 2004.09.15
typedef int (*_DLL_GetFindDefine_TR)(HWND hSelfWnd, char *pFullFilePath, int nSize, int &nTrIndex, LPSTR &lpData);

//# [�Է�] int nType => 0:��õ, 1:�����, 2:Language
//#        BOOL bGetAlert => * Alert�� ��� : TRUE, * ����� ��ü : FALSE
//# [���] ������ ����, lpData[100]
//typedef int (*_DLL_GetConditionList)(int nType, BOOL bGetAlert, LPSTR &lpData);
typedef int (*_DLL_GetConditionList)(int nType, BOOL bGetAlert, LPSTR &lpName, LPSTR &lpPath);



//typedef int (*_DLL_GetFindDefine)(CString strFullFilePath, LPSTR &lpData);
//# _DLL_GetConditionList		nType => 0:��õ, 1:�����, 2:Language
//typedef int (*_DLL_GetConditionList)(int nType, LPSTR &lpData);

class CMyNetFindLibLoader
{
public:
	CMyNetFindLibLoader();
	virtual ~CMyNetFindLibLoader();
	void SetPath(CString strRootPath) {m_strRootPath=strRootPath;}
	void SetHandle(	HWND hSelfWnd,HWND hWndParent) {m_hSelfWnd = hSelfWnd; m_hWndParent = hWndParent;}
//	int GetFindDefine(char *pFullFilePath, int nSize, LPSTR &lpData);//(CString strFullFilePath, LPSTR &lpData);
	int GetFindDefine_TR(char *pFullFilePath, int nSize, int &nTrIndex, LPSTR &lpData);
//	int GetConditionList(int nType, BOOL bGetAlert, LPSTR &lpData);//(int nType, LPSTR &lpData);
	int GetConditionList(int nType, BOOL bGetAlert, LPSTR &lpName, LPSTR &lpPath);
private:
	BOOL LoadMyNetFindLib();
	BOOL UnloadMyNetFindLib();
	HINSTANCE m_hMyNetFindLib;
	CString m_szMyNetFindLib;
	CString m_strRootPath;
	HWND m_hSelfWnd;
	HWND m_hWndParent;
};

#endif // !defined(AFX_LOADMYNETFINDLIB_H__CC5B59D9_4FDE_4C23_850A_8F9E34BEBA85__INCLUDED_)
