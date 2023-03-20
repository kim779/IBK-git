#include "stdafx.h"
#include "PcTrMng.h"
//@Solomon090819#include "../../../../../CommonSrc/DirMgr.h"

#include "../../../SrcSite/include/axisfire.h"	//@Solomon

/////////////////////////////////////////////////////////////////////////////
// CMainInfoManager

CMainInfoManager::CMainInfoManager()
{
	m_MapData.SetSize(MAX_IMainInfoManager);
}

CMainInfoManager::~CMainInfoManager()
{
	m_MapData.RemoveAll();
	m_MapData.SetSize(0);
}

LPCSTR CMainInfoManager::GetDataString(int nKey)
{
	if(nKey<0 || nKey>=MAX_IMainInfoManager) return NULL;

	// ������ �°� �Ϻμ���, ID���ϱ�
	//if(nKey==USERINFOMNG_USERID || nKey==USERINFOMNG_USERENCID)
	//	return (LPCSTR)::GetUserID(); // ������ SharedInfo�� �ִ� ������ �Ѱ��ش�.

	return (LPCSTR)m_MapData.GetAt(nKey); 
}

BOOL CMainInfoManager::SetDataString(int nKey, LPCSTR szData)
{
	if(nKey<0 || nKey>=MAX_IMainInfoManager) return FALSE;

	m_MapData.SetAt(nKey, szData);
	
/*	
	if(nKey==28) { // ATM �ϰ�쿣 code.ocx�� ATM �������� �Ѱ��ش�.
		theApp.m_MasterMng.m_CodeOcx.SetAtmData(szData);
		theApp.m_pDataMonMng->NotifyData("ATM", 0, (long)szData);
	}
*/
	return TRUE;
}

UINT AFXAPI CMainInfoManager::HashDataAXIS(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;

	return nHash;
}

LPCSTR CMainInfoManager::GetUserID()
{
	theApp.m_szTemp = "winix";
	HWND hWinixWnd = theApp.m_MasterMng.GetHWND();
	if (!hWinixWnd)
	{
		AfxMessageBox("GetUserID�Լ� hWinixWnd is Null!!");
		return theApp.m_szTemp;
	}
	
	LPCSTR strTemp;
	static CString strUserName;//20220117 �̹��� >>
	strTemp = theApp._WinixVariant(hWinixWnd, nameCC);
	//strUserName.Format("%08u", HashKey(strTemp));
	strUserName.Format("%08u", HashDataAXIS(strTemp));
	
	return strUserName;
}

LPCSTR CMainInfoManager::GetEncUserID()
{
	return GetUserID();
}

LPCSTR CMainInfoManager::GetRootDir()
{
	return GetDataString(USERINFOMNG_ROOTDIR);
}

LPCSTR CMainInfoManager::GetDataDir()
{
	return GetDataString(USERINFOMNG_DATADIR);
}


LPCSTR CMainInfoManager::GetEncUserDir(LPCSTR _szKey)
{
//@Solomon090819	theApp.m_szTemp = CDirMgr::GetUserDir();

	//CString szRoot = GetRootDir();
	CString szUser = GetEncUserID();
	theApp.m_szTemp.Format("%s\\user\\%s", GetRootDir(), szUser);

	if(_szKey!=NULL)
	{
		CString szKey(_szKey); szKey.MakeLower();
		if(szKey.Compare("chart")==0)
			theApp.m_szTemp += "\\Chart";
	}

	return theApp.m_szTemp;
}


//*****************************************************************************
// ��  ��  ��  : CString CMainInfoManager::GetLoginInfo(LPCSTR szKey)
// ��      ��  : 
// ��      ��  : szKey="ISHTS" : ����ڰ� ��("1")���� ����("0")����
//				 szKey="USER_LEVEL" : ������� ��� "A"��
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2007-01-13 ���� 4:21:50
//*****************************************************************************
CString CMainInfoManager::GetLoginInfo(LPCSTR szKey)
{
	int nResult = 0;
	CString strResult = "";

//@Solomon090819	if(strcmp(szKey, "ISHTS") == 0)
	{
//@Solomon090819		BOOL bTrue = m_linkDataMng.IsHTS();
//@Solomon090819		if(bTrue)
			nResult = 1;

		strResult.Format("%d", nResult);
	}

	return strResult;
}

//-->@.ȯ�溯�� ó�� ����
BOOL CMainInfoManager::SetEnvironmentVariable(
				LPCTSTR lpName,  // environment variable name
				LPCTSTR lpValue  // new value for variable
				)
{
	CString szKey(lpName);
	CString szValue(lpValue);
	szKey.TrimLeft();	szKey.TrimRight();
	szValue.TrimLeft(); szValue.TrimRight();
	m_mapEnvData.SetAt(szKey, szValue);
	return TRUE;
}

DWORD CMainInfoManager::GetEnvironmentVariable(
				LPCTSTR lpName,		// environment variable name
				CString &rValue		// CString for variable value
			)
{
	rValue.Empty();

	m_mapEnvData.Lookup(lpName, rValue);
	return rValue.GetLength();
}

BOOL CMainInfoManager::FreeEnvironmentStrings(
				LPTSTR lpName  // environment variable name
			)
{
	m_mapEnvData.RemoveKey(lpName);
	return TRUE;
}

DWORD CMainInfoManager::GetEnvironmentStrings(CStringList& szBufferList)
{
	szBufferList.RemoveAll();

	CString rKey, rValue;
	for(POSITION pos=m_mapEnvData.GetStartPosition(); pos;)
	{
		m_mapEnvData.GetNextAssoc(pos, rKey, rValue);
		szBufferList.AddTail(rKey);
	}

	return szBufferList.GetCount();
}

int	CMainInfoManager::IniToEnvironmentTable(LPCSTR szIniPath, LPCSTR szSect)
{
	int nAddCnt=0;

	char *pReturnedStrng=(char*)malloc(9600);	//Section MaxSize 9600.
	DWORD nSize = 9600;

	DWORD lReads = ::GetPrivateProfileSection(szSect, pReturnedStrng, 9600, szIniPath);
	if(lReads<0)
	{
		// �����Ͱ� ������ 64K���� �����Ѵ�.
		free(pReturnedStrng);
		pReturnedStrng = (char*)malloc(64000);	//64K.
		nSize = 64000;

		lReads = ::GetPrivateProfileSection(szSect, pReturnedStrng, 9600, szIniPath);
		if(lReads<0)
		{
			free(pReturnedStrng);
			return nAddCnt;
		}
	}

	int nStart=0, nEnd=0;
	int nStrSize=0;
	char aLine[MAX_PATH], aKey[100];//, aValue[MAX_PATH];
	for(int i=0; i<(int)lReads; i++)
	{
		if(pReturnedStrng[i] == 0x00)
		{
			memcpy(aLine, &pReturnedStrng[nStart], i-nStart+1);	//Null����.
			nStart = i+1;

			theApp.m_DataUtilMng.GetWord(aKey, aLine, '=');

			// SetEnvironmentVariable�ȿ��� Trimó���� �ڵ����� �Ѵ�.
			SetEnvironmentVariable(aKey, aLine);
			nAddCnt++;
		}
	}

	free(pReturnedStrng);
	return nAddCnt;
}

int	CMainInfoManager::GetCountOfAccount()
{
//@Solomon090819	return m_linkDataMng.GetAccountCount();
	return 0; //@Solomon090819
}

BOOL CMainInfoManager::GetUserAccount(CStringArray& saAccount)
{
	HWND hWinixWnd = theApp.m_MasterMng.GetHWND();
	CString strRet = theApp._WinixVariant(hWinixWnd, accountCC);

	CString strAccount, strTemp;
	int nPos=0, nOffset=0;
	int nDataLen = strRet.GetLength();
	while (nPos>=0 && nOffset<=nDataLen)
	{
		nPos = strRet.Find('\n');
		if (nPos<0)			break;

		strTemp = strRet.Left(nPos);
		strRet = strRet.Mid(nPos+1);

		nPos = strTemp.Find('\t');
		if (nPos<0)			break;
		strAccount = strTemp.Left(nPos);
		strTemp = strTemp.Mid(nPos+1);

		saAccount.Add(strAccount);
	}
	
/*
	int nCount = GetCountOfAccount();
	if(nCount == 0) return FALSE;

	char szBuffer[512];		memset(szBuffer, 0x00, 512);
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{	
//@Solomon090819		m_linkDataMng.GetAccountByIndex(nIndex, szBuffer, 512);
//@Solomon090819		CString strData = szBuffer;
//@Solomon090819		strData.Replace(" ", "");
//@Solomon090819		saAccount.Add(strData);
	}
*/
	return TRUE;	
}

CString CMainInfoManager::GetAccountName(LPTSTR lpszAccount)
{
//@Solomon090819	m_linkDataMng.GetAccountName(lpszAccount, szBuffer, 512);
	HWND hWinixWnd = theApp.m_MasterMng.GetHWND();
	CString strName = theApp._WinixVariant(hWinixWnd, acnameCC, lpszAccount);

	return strName;
}

CString CMainInfoManager::GetAccountPass(LPTSTR lpszAccount)
{
//@Solomon090819	m_linkDataMng.GetAccountPassword( lpszAccount, szBuffer, 512);
	HWND hWinixWnd = theApp.m_MasterMng.GetHWND();
	CString strPass = theApp._WinixVariant(hWinixWnd, passCC, lpszAccount);

	return strPass;
}


CString CMainInfoManager::GetLoginPass()
{
	//@Solomon090819	m_linkDataMng.GetAccountPassword( lpszAccount, szBuffer, 512);
	HWND hWinixWnd = theApp.m_MasterMng.GetHWND();
	CString strPass = theApp._WinixVariant(hWinixWnd, passCCx, "");
	
	return strPass;
}

char gHtsMode[2]={0,};
int CMainInfoManager::GetHtsMode()
{
	if(gHtsMode[0]==NULL)
	{
		//systemCC
		HWND hWinixWnd = theApp.m_MasterMng.GetHWND();
		CString strHts = theApp._WinixVariant(hWinixWnd, systemCC, "");
		gHtsMode[0] = strHts[0];
	}

	if (gHtsMode[0]=='F')
		return HTSMODE_FOPRO;
	else if (gHtsMode[0]=='G')
		return HTSMODE_FX;
	else
		return HTSMODE_HTS;

// 	if (strHts.CompareNoCase(_T("H")) == 0)
// 		return HTSMODE_HTS;
// 	else if (strHts.CompareNoCase(_T("F")) == 0)
// 		return HTSMODE_FOPRO;

	return 0;
}

