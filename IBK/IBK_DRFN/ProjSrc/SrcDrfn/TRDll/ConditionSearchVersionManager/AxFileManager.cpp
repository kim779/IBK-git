#include "stdafx.h"
#include "AxFileManager.h"
//#include "tlhelp32.h"
#include <io.h>				// for _chmod() function supports
#include <sys/stat.h>		// for _chmod() function supports

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//==============================================================================
// �Լ���   : lzCopy
// �Լ���� : 
// �ĸ����� : 
// ���ϰ�   : int	0 : ����
//					1 : ���� ���� Open����
//					2 : ��� ���� Open���� / �ش� ���丮 ����
//					3 : ��� ���� Open���� / �ش� ���丮 ����
//					4 : 
//------------------------------------------------------------------------------
// HISTORY  : �� ��  : 98.12.14  �� �� ��
//==============================================================================
/*int CAxFileManager::lzCopy(LPSTR pszFromFile, LPSTR pszToFile)
{
	OFSTRUCT 	structTmp;
	OFSTRUCT    structOrg;
	HFILE 		hFromFile, hToFile;
	char        buff[MAX_PATH];
	long        res;

	hFromFile = LZOpenFile(pszFromFile, &structTmp, OF_READ);
	if(hFromFile == -1) {		
		m_szLastMsg = "lzCopy�Լ����� LZOpenFile() ����";
		return 1;
	}
//	FileRemove(pszToFile);
	hToFile = LZOpenFile(pszToFile, &structOrg, OF_DELETE);

	hToFile = LZOpenFile(pszToFile, &structOrg, OF_CREATE);
	if(hToFile == -1) {
		if(GetDirectory(pszToFile, buff) == FALSE) {
			LZClose(hFromFile);
			m_szLastMsg = "lzCopy�Լ����� GetDirectory() ����";
			return 2;
		}
		MakeDirectory(buff);
		hToFile = LZOpenFile(pszToFile, &structOrg, OF_CREATE);

		// 3�� ��õ�
		for(int i=0; i<3 && hToFile==-1; i++) {
			Sleep(1000);
			hToFile = LZOpenFile(pszToFile, &structOrg, OF_CREATE);

			if(hToFile==LZERROR_BADINHANDLE)
				m_szLastMsg = "������� ������ ����(LZERROR_BADINHANDLE) ";
			else if(hToFile==LZERROR_GLOBALLOC)
				m_szLastMsg = "������� ������ ����(LZERROR_GLOBALLOC) ";
			TRACE0(m_szLastMsg);
		}
		if(hToFile==-1) {
			LZClose(hFromFile);
			m_szLastMsg = "lzCopy�Լ����� LZOpenFile() ����";
			return 3;
		}
	}

	res = LZCopy(hFromFile, hToFile);

	LZClose(hFromFile);
	LZClose(hToFile);

	if(res < 0) {
		m_szLastMsg = "lzCopy�Լ����� LZCopy() ����";
		if(res==LZERROR_BADINHANDLE)
			m_szLastMsg += " ==> LZERROR_BADINHANDLE";
		else if(res==LZERROR_BADOUTHANDLE)
			 m_szLastMsg += " ==> LZERROR_BADOUTHANDLE";
		if(res==LZERROR_GLOBALLOC)
			m_szLastMsg += " ==> LZERROR_GLOBALLOC";
		else if(res==LZERROR_GLOBLOCK)
			 m_szLastMsg += " ==> LZERROR_GLOBLOCK";
		else if(res==LZERROR_READ)
			 m_szLastMsg += " ==> LZERROR_READ";
		TRACE0(m_szLastMsg);
// AfxMessageBox(m_szLastMsg);
		return 4;
	}

	return 0;
}


// �����̸� 0
// Ʋ���� ������  11:���ϻ�������
int	CAxFileManager::lzMove(LPSTR pszFromFile, LPSTR pszToFile)
{
	int lRet = lzCopy(pszFromFile, pszToFile);
	if(lRet!=0) return lRet;

	if(!FileRemove(pszFromFile)) return 11;
	return 0;
}
*/


// * lzCopy --> zipCopy (Zip �˰��� �̿�) 
// * void CAxFileManager::Open(LPCTSTR szFile)
// * void CAxFileManager::Extract(LPCTSTR szTargetDir)
// * void CAxFileManager::Close()
//int CAxFileManager::zipCopy(LPSTR pszFromFile, LPSTR pszToFile)
//{
//	BOOL bIsZipFile = TRUE;
//	CString szFromFile, szToFile, szToFileOrigin;
//	szToFileOrigin = (LPCSTR)pszToFile;
//	szFromFile = (LPCSTR)pszFromFile;
//
//	CString szTmp = szToFileOrigin;
//	szTmp.MakeLower();
//
//	if(strcmp(szTmp.Right(3), "zip")==0) {				// zip����� ������ ��� 
//		int nLength = szToFileOrigin.GetLength();
//		// '.zip'�� ���� 
//		szToFile = szToFileOrigin.Left(nLength - 4);
//	}
//	else {												// zip������ �ȵ� ��� 
//		szToFile = szToFileOrigin;
//		bIsZipFile = FALSE;
//	}
//
//	if(FileCheck(szToFile)) {						  // Dest File�� �̹� �����ϴ� ��쿡�� ����  	
//		int nRet;
//		for(int i=0; i<3; i++)						  // Dest File ����� (�����ϸ� 3ȸ���� �õ�)
//		{
//			nRet = FileRemove((LPSTR)(LPCSTR)szToFile);
//			if(nRet==1) {
//				break;
//			}
//			else if(i==2 && nRet==0) {
//				CString szMsg;
//				szMsg.Format("���� [%s] ������ �����Դϴ�.", szToFile);
//				AfxMessageBox(szMsg);
//				return 1;
//			}
//		}	
//	}
//
//	char buff[MAX_PATH];
//	if(GetDirectory((LPSTR)(LPCSTR)szToFile, buff)==FALSE) {    // Dest Directory ��� ��� 
//		m_szLastMsg = "zipCopy�Լ����� GetDirectory() ����";
//		return 2;
//	}
//
//	MakeDirectory(buff);			  		      // Dest Directory ����� 	
//
//	if(bIsZipFile) {
//		ExtractZipFile(szFromFile, (LPCSTR)buff);
//		FileRemove((LPSTR)(LPCSTR)szFromFile);
//	}
//	else {
//		MoveFile(szFromFile, szToFile);
//	}
//
//	return 0;
//}

// * lzCopy --> zipCopy (Zip �˰��� �̿�) 
// * void CAxFileManager::Open(LPCTSTR szFile)
// * void CAxFileManager::Extract(LPCTSTR szTargetDir)
// * void CAxFileManager::Close()
int CAxFileManager::zipCopy(LPSTR pszFromFile, LPSTR pszToFile)
{
	BOOL bIsZipFile = TRUE;
	CString szFromFile, szToFile;
	szFromFile = (LPCSTR)pszFromFile;
	szToFile = (LPCSTR)pszToFile;

	CString szTmp = ".zip";
	if(szTmp.CompareNoCase(szFromFile.Right(4))!=0)
	{
		bIsZipFile = FALSE;
	}

	if(FileCheck(szToFile))					  // Dest File�� �̹� �����ϴ� ��쿡�� ����
	{
		int nRet;
		nRet = FileRemove((LPSTR)(LPCSTR)szToFile);
		if(0 == nRet) {		// FAILED
			Sleep(50);
			nRet = FileRemove((LPSTR)(LPCSTR)szToFile, TRUE);
			if(0 == nRet) {		// FAILED
				Sleep(100);
				nRet = FileRemove((LPSTR)(LPCSTR)szToFile, TRUE);
				if( 0 == nRet) { // if(i==2 && nRet==0) {
//					CString szMsg;
//					szMsg.Format("���� [%s] ������ �����Դϴ�.", szToFile);
//					AfxMessageBox(szMsg);
					return 1;
				}
			}
		}
	}
	
	char buff[MAX_PATH];
	if(GetDirectory((LPSTR)(LPCSTR)szToFile, buff)==FALSE) {    // Dest Directory ��� ��� 
		m_szLastMsg = "zipCopy�Լ����� GetDirectory() ����";
		return 2;
	}

	MakeDirectory(buff);			  		      // Dest Directory ����� 	

	if(bIsZipFile) {
		ExtractZipFile(szFromFile, (LPCSTR)buff);
		FileRemove((LPSTR)(LPCSTR)szFromFile);
	}
	else {
		MoveFile(szFromFile, szToFile);
	}

	return 0;
}

int CAxFileManager::ExtractZipFile(LPCSTR szFromFile, LPCSTR szToDir)
{
	Open(szFromFile);
	Extract(szToDir);
	Close();

	return 1;
}

void CAxFileManager::Open(LPCTSTR szFile)
{
	/*
	m_zip.Open(szFile, CZipArchive::open, 0); 
	for (int i = 0; i < m_zip.GetNoEntries(); i++)
	{
		CFileHeader fh;
		m_zip.GetFileInfo(fh, (WORD)i);
	}
	*/
	
	//zip ���̺귯�� ������Ʈ From cyberahn. 2007.07.10.
	m_zip.Open(szFile, CZipArchive::zipOpen, 0); 
	for (int i = 0; i < m_zip.GetCount(); i++)
	{
		CZipFileHeader fh;
		m_zip.GetFileInfo( fh, (WORD)i);
	}
}

void CAxFileManager::Extract(LPCTSTR szTargetDir)
{
	//�������Ϲ� ���� �������� ����. From cyberahn. 2008.01.09.--START
	//m_zip.ExtractFile(0, szTargetDir);
	int nCount = m_zip.GetCount();
	for( int i=0; i < nCount; i++)
		m_zip.ExtractFile(i, szTargetDir);
	//�������Ϲ� ���� �������� ����. From cyberahn. 2008.01.09.--END
}

void CAxFileManager::Close()
{
	m_zip.Close();
}


BOOL CAxFileManager::GetDirectory(LPSTR pszSrc, LPSTR pszDes)
{
	int len = strlen(pszSrc);
	while (pszSrc[--len] != '\\') {
		if (len == 0)
			return FALSE;
	}
	strncpy(pszDes, pszSrc, len);
	pszDes[len] = 0x00;

	return TRUE;
}

BOOL CAxFileManager::GetFileName(LPSTR pszSrc, LPSTR pszDes)
{
	int len = strlen(pszSrc);
	int len2 = len;
	while (pszSrc[--len] != '/') ;	
	if(len==0) len--;

	strncpy(pszDes, &pszSrc[len+1], len2);
	pszDes[len2] = 0x00;

	return TRUE;
}


BOOL CAxFileManager::MakeDirectory(LPSTR pszSrc)
{
//	return(mkdir(pszSrc)==0);

	CString szSrc = pszSrc;
	CString baseDir;

	if( szSrc.Find(m_szDefaultPath) != -1)
	{
		baseDir = m_szDefaultPath;
		if( szSrc.GetLength() <=  baseDir.GetLength())
			return FALSE;
		szSrc = szSrc.Mid(baseDir.GetLength()+1);
	}
	else
	{
		baseDir = m_szCommonDir;
		if( szSrc.GetLength() <=  baseDir.GetLength())
			return FALSE;
		szSrc = szSrc.Mid(baseDir.GetLength()+1);
	}
	
	return CreateSubDirectory(baseDir, szSrc);
}


BOOL CAxFileManager::RemoveDirectory(LPSTR pszSrc)
{
	return(rmdir(pszSrc)==0);
}

BOOL CAxFileManager::CreateSubDirectory(const CString& baseDir, const CString& strPath)
{
	int nPosition = strPath.ReverseFind('/');
	if ( nPosition == -1 )
	{
		if ( ::CreateDirectory(baseDir + "/" + strPath, NULL) ) // ����
		{
			return TRUE;
		}
		else // ����
		{
			DWORD dwErrorCode = GetLastError();
			if ( dwErrorCode == 183 )		// �̹� ���丮�� �ִ� ���
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		CString strTempPath = strPath.Left(nPosition);
		if ( CreateSubDirectory(baseDir, strTempPath) )
		{
			if ( ::CreateDirectory(baseDir + "/" + strPath, NULL) ) // ����
			{
				return TRUE;
			}
			else // ����
			{
				DWORD dwErrorCode = GetLastError();
				if ( dwErrorCode == 183 )		// �̹� ���丮�� �ִ� ���
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
}


BOOL CAxFileManager::FreeAnyModule(LPSTR LibFile)
{
	char LibFileName[MAX_PATH];
	//GetFileName(LibFile, LibFileName);
	strcpy(LibFileName, LibFile);

	HINSTANCE hLib;
	hLib = GetModuleHandle(LibFileName);

	if(hLib == NULL){
		CString msg;
		msg.Format("GetLastError() NO = 0x%08lx\n", GetLastError());
		TRACE(msg);
		return FALSE;
	}
	FreeLibrary(hLib);

	return TRUE;
}



void CAxFileManager::Exec(LPCSTR pszExec, int showCmd) 
{
	UINT er = WinExec(pszExec, showCmd);

	if(er < 31)
	{
		CString ers;
		switch(er)
		{
			case 0: ers = "�޸𸮰� ���� �մϴ�.";
					break;
			case ERROR_BAD_FORMAT:
					ers = "���� ȭ�Ͽ� �̻��� �ֽ��ϴ�.";
					break;
			case ERROR_FILE_NOT_FOUND:
					ers = "������ �߰� �� �� �����ϴ�.";
					break;
			case ERROR_PATH_NOT_FOUND:
					ers = "��θ� �߰��� �� �����ϴ�.";
					break;
		}
		AfxMessageBox(ers);
	}
}



///////////////////////////////////////////////////////////////////////////////
// ������            : �ѵ���(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// ���� ��¥         : 2004/1/8
// �Լ� �̸�         : CAxFileManager::FileCheck
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : �����̸� TRUE
// �Ķ����
//		LPCSTR szPath  : ��ü ���
// �Լ� ����         : ������ �����ϴ��� ������ �˾Ƴ���.
///////////////////////////////////////////////////////////////////////////////
BOOL CAxFileManager::FileCheck(LPCSTR szPath)
{
	CFileFind fileFind;
	return fileFind.FindFile(szPath);
}

int CAxFileManager::DLLRegister(LPCSTR szFileName)
{
	int			iReturn=0;
	CString		szErrorMsg;

	// Initialize OLE.
	if (FAILED(OleInitialize(NULL))) {
		AfxMessageBox("DLLRegister OleInitialize ����");
		return 1;
	}

	SetErrorMode(SEM_FAILCRITICALERRORS);       // Make sure LoadLib fails.
	// Load the library.
	HINSTANCE hLib = LoadLibraryEx(szFileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hLib < (HINSTANCE)HINSTANCE_ERROR) {
		szErrorMsg.Format("File Name=%s, GetLastError() NO = 0x%08lx\n", szFileName, GetLastError());
		AfxMessageBox(szErrorMsg);
		iReturn = 2;
		goto CleanupOle;
	}

	HRESULT (STDAPICALLTYPE * lpDllEntryPoint)(void);
	// Find the entry point.
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");
	if (lpDllEntryPoint == NULL) {
		TCHAR szExt[_MAX_EXT];
		_tsplitpath(szFileName, NULL, NULL, NULL, szExt);
		if ((_stricmp(szExt, ".dll") != 0) && (_stricmp(szExt, ".ocx") != 0)) {
			szErrorMsg.Format("File Name=%s, GetProcAddress Fail\n", szFileName);
			AfxMessageBox(szErrorMsg);
		}

		iReturn = 3;
		goto CleanupLibrary;
	}

	// Call the entry point.
	if (FAILED((*lpDllEntryPoint)())) {
		szErrorMsg.Format("File Name=%s, lpDllEntryPoint Fail\n", szFileName);
		AfxMessageBox(szErrorMsg);
		iReturn = 4;
		goto CleanupLibrary;
	}

//    Info(IDS_CALLSUCCEEDED, pszDllEntryPoint, pszDllName);

CleanupLibrary:
	FreeLibrary(hLib);

CleanupOle:
	OleUninitialize();

	return iReturn;
}


BOOL CAxFileManager::FileCopy(LPSTR pszSrc, LPSTR pszDes)
{
	int nResult = zipCopy(pszSrc, pszDes); 

	if(nResult == 0) m_bFileMove = TRUE;
	else m_bFileMove = FALSE;

	return nResult;
}

BOOL CAxFileManager::FileMove(LPSTR pszSrc, LPSTR pszDes)
{
	if(FileCopy(pszSrc, pszDes) != 0) return FALSE;
	return FileRemove(pszSrc);
}

// * ������ ����� ���� �Ӽ��� �ľ��Ѵ�. 
// * ���� read �Ӽ��� �ִ� ��� write�� �Ӽ��� �����Ѵ�. 
BOOL CAxFileManager::FileRemove(LPSTR pszSrc, BOOL bChangeAttr)
{
	if(bChangeAttr)
	{
		if(!FileCheck(pszSrc))
			return TRUE;
		
		if(_chmod(pszSrc, _S_IWRITE)==-1) 
		{
			TRACE("[%s] ������ �Ӽ��� ������ �� �����ϴ�.\n", pszSrc);
		}
	}
/****
	CFileStatus status;
	CFile::GetStatus( pszSrc, status );
	BOOL bReadOnly = (BOOL) status.m_attribute & 0x01;
	if(bReadOnly)
	{
		if(_chmod(pszSrc, _S_IWRITE)==-1) {
			CString szMsg;
			szMsg.Format("[%s] ������ �Ӽ��� ������ �� �����ϴ�.", (LPCSTR)pszSrc);
			AfxMessageBox(szMsg);
		}
	}
****/
	BOOL bRtn = DeleteFile(pszSrc);
	//���ϻ��� ���н�. VIP From cyberahn. 2007.08.09.
	if( !bRtn)
	{
		CString strDescTemp;
		time_t ltime;
		char szTempPath[ MAX_PATH];

		time( &ltime );
		GetTempPath(MAX_PATH, szTempPath);
		strDescTemp.Format( "%s\\DeleteAfter_%ld", szTempPath, ltime);

		MoveFileEx(pszSrc, strDescTemp, MOVEFILE_REPLACE_EXISTING);
		MoveFileEx(strDescTemp, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	}
	return bRtn;
//	return (remove(pszSrc)==0);
}




///////////////////////////////////////////////////////////////////////////////
// ������            : �ѵ���(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// ���� ��¥         : 2004/1/8
// �Լ� �̸�         : int	CAxFileManager::ReadFile2Data
// ���� Ÿ��         : void
// �Ķ����
//		LPCSTR pszFileName : ���� ���̺� ���� �̸�
//		CStringList& szDataList  : ���� ���̺��� �о� ��Ʈ�� ����Ʈ�� ����
// �Լ� ����         : 
///////////////////////////////////////////////////////////////////////////////
int	CAxFileManager::ReadFile2Data(LPCSTR pszFileName, CStringList& szDataList)
{
	szDataList.RemoveAll();
	if(!FileCheck(pszFileName)) return 0;

	FILE* fp = fopen(pszFileName, "rt");
	if(!fp) return 0;

	//char buf[255];
	CString szData;
	szData.GetBuffer(255);
	szData.LockBuffer();
	while(fgets((LPSTR)(LPCSTR)szData, 255, fp)) 
	{
		szData.TrimLeft();
		szData.TrimRight();
		if(szData.GetLength()==0 || szData[0]=='#')
			continue;

		szDataList.AddTail(szData);
	}
	szData.ReleaseBuffer();

	fclose(fp);
	return szDataList.GetCount();
}

int	CAxFileManager::DirectoryMove(LPCSTR pszFromDir, LPCSTR pszToDir, LPCSTR pszExt, BOOL bSub)
{
	{
		CheckDirectory(pszToDir);
	}

	char aFile[MAX_PATH];
	sprintf(aFile, "%s/%s", pszFromDir, pszExt);

	char aCopyFileFrom[MAX_PATH], aCopyFileTo[MAX_PATH];

	CString strComponent;
	CFileFind finder;
	BOOL bWorking = finder.FindFile( aFile );
	while (bWorking){
		bWorking = finder.FindNextFile();
		strComponent = finder.GetFileName();

		if(!finder.IsDirectory())  {
			sprintf(aCopyFileFrom, "%s/%s", pszFromDir, strComponent);
			sprintf(aCopyFileTo, "%s/%s", pszToDir, strComponent);
			if(FileMove( aCopyFileFrom, aCopyFileTo)==FALSE)
				return -1;
			//TRACE("FileMove => %s To %s\n", aCopyFileFrom, aCopyFileTo);
		}
		else if(bSub && strComponent[0]!='.') {
			// ���丮�� ��� ���ȣ���� ���Ѵ�.
			char aSubFileFrom[MAX_PATH], aSubFileTo[MAX_PATH];
			sprintf(aSubFileFrom, "%s/%s", pszFromDir, strComponent);
			sprintf(aSubFileTo, "%s/%s", pszToDir, strComponent);
			return DirectoryMove(aSubFileFrom, aSubFileTo, pszExt, bSub);
		}
	}
	return 0;
}

int	CAxFileManager::DirFileRemove(LPCSTR pszDir, LPCSTR pszExt, BOOL bSub, BOOL _bDirRemove)
{
	char aFile[MAX_PATH];
	sprintf(aFile, "%s/%s", pszDir, pszExt);

	char aDelFile[MAX_PATH];
	CString strComponent;
	CFileFind finder;
	BOOL bWorking = finder.FindFile( aFile );
	while (bWorking){
		bWorking = finder.FindNextFile();
		strComponent = finder.GetFileName();

		if(!finder.IsDirectory())  {
			sprintf(aDelFile, "%s/%s", pszDir, strComponent);
			if(FileRemove( aDelFile)==FALSE)
				return -1;
			//TRACE("FileRemove =>  %s\n", aDelFile);
		}
		else if(bSub && strComponent[0]!='.') {
			// ���丮�� ��� ���ȣ���� ���Ѵ�.
			char aSubDir[MAX_PATH];
			sprintf(aSubDir, "%s/%s", pszDir, strComponent);
			return DirFileRemove(aSubDir, pszExt, bSub, TRUE);
		}
	}
//	if(_bDirRemove)
//		RemoveDirectory((LPSTR)pszDir);
	return 0;
}

int	CAxFileManager::DirectoryIfReMove(LPCSTR aTarDir ,LPCSTR pszExt, BOOL bSub)
{
	char aFile[MAX_PATH];
	sprintf(aFile, "%s/%s", aTarDir, pszExt);

	char aDelFile[MAX_PATH];
	CString strComponent;
	CFileFind finder;
	BOOL bWorking = finder.FindFile( aFile );
	while (bWorking){
		bWorking = finder.FindNextFile();
		strComponent = finder.GetFileName();

		if(!finder.IsDirectory())  {
			sprintf(aDelFile, "%s/%s", aTarDir, strComponent);

			CFile  fileChk;
			if(fileChk.Open(aDelFile, CFile::modeRead)) {
				if( fileChk.GetLength()==0) {
					fileChk.Close();
					FileRemove( aDelFile);
				}
			}
			else {
				fileChk.Remove(aDelFile);
			}
		}
		else if(bSub && strComponent[0]!='.') {
			char aSubDir[MAX_PATH];
			sprintf(aSubDir, "%s/%s", aTarDir, strComponent);
			DirectoryIfReMove(aSubDir, pszExt, bSub);
		}
	}
	return 0;
}

/*
	CFileFind fileFind;
	if(fileFind.FindFile(szPath))
		return TRUE;
*/


BOOL CAxFileManager::CheckDirectory(LPCSTR pszToDir)
{
	/*
	CString szDir = pszToDir;
	szDir = szDir.Mid(m_szDefaultPath.GetLength()+1);
		
	return CreateSubDirectory(szDir);
	*/


	CString szSrc = pszToDir;
	CString baseDir;

	if( szSrc.Find(m_szDefaultPath) != -1)
	{
		baseDir = m_szDefaultPath;
		if( szSrc.GetLength() <=  baseDir.GetLength())
			return FALSE;
		szSrc = szSrc.Mid(baseDir.GetLength()+1);
	}
	else
	{
		baseDir = m_szCommonDir;
		if( szSrc.GetLength() <=  baseDir.GetLength())
			return FALSE;
		szSrc = szSrc.Mid(baseDir.GetLength()+1);
	}
	
	return CreateSubDirectory(baseDir, szSrc);
}


