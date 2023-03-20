#include "stdafx.h"
#include "libctr.h"
#include "CX_FDS.h"
#include "../../h/axisfire.h"
#include "../../h/ledger.h"

#define	homeCC			0x0a

void WriteLog( LPCSTR home,LPCSTR log, ... )
{
#if 1
	FILE *fp = fopen( (CString)home + "\\exe\\axis.log", "a+");
	if (!fp)
		return;

	const	CTime time = CTime::GetCurrentTime();
	fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] ").GetString());
	
	va_list argptr;
	va_start(argptr, log);
	vfprintf(fp, log, argptr);
	va_end(argptr);
	fprintf(fp, "\n");
	
	fclose(fp);
#endif
}

__declspec(dllexport) int APIENTRY axFDSValue(char* trN, char* srcB, int srcL, char* desB)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sPath;
	CString strFilePath;
	CFile	file;
	CString sBuf;
	TCHAR	chFileName[512]{};

	GetModuleFileName(NULL, chFileName, MAX_PATH);

	strFilePath.Format(_T("%s"), chFileName);

	strFilePath = strFilePath.Left(strFilePath.ReverseFind('\\'));
	strFilePath.Replace("\\exe", "");

	WriteLog(strFilePath,"FDS START");

	sPath.Format("%s\\tab\\FDSSERVICE.DAT", strFilePath);
	if (file.Open(sPath, CFile::modeRead | CFile::shareDenyNone))
	{
		const	long	len = (long)file.GetLength();
		std::unique_ptr<char[]> buf = std::make_unique<char[]>(len + 1);
		file.Read(buf.get(), len);
		sBuf = CString(buf.get(), len);
		sBuf.MakeUpper();
		buf.reset();
		file.Close();
	}

	CString strTRN = trN;
	strTRN.MakeUpper();

	if (sBuf.Find(strTRN, 0) < 0)
	{
		if (srcL < sizeof(_ledgerH))
		{
			WriteLog(strFilePath,"FDS SIZE ERROR\n");
			return 0;
		}

		const	struct	_ledgerH *ledger = (struct _ledgerH *)srcB;

		strTRN = CString(ledger->svcd, sizeof(ledger->svcd));
		strTRN.MakeUpper();
		if (sBuf.Find(strTRN, 0) < 0)
		{
			WriteLog(strFilePath,"FDS NOT SERVICE\n");
			return 0;
		}
	}

	sPath.Format("%s\\dev\\KTBSOL.dll", strFilePath);

	HMODULE	hInfoDll = LoadLibrary(sPath);
	if (hInfoDll == nullptr)
	{
//		AfxMessageBox("DLL 파일 존재 에러....");
		WriteLog(strFilePath,"FDS KTBSOL DLL ERROR\n");
		return 0;
	}

	int	nRet = 0;
	std::unique_ptr<char[]> pValue = nullptr;
	typedef int (typeInitialize_DLL_Api)();
	typedef int (typeGetScanResultApiLen)();
	typedef int (typeGetScanResultApi)(char* pszVal);

	typeInitialize_DLL_Api *Initialize_DLL_Api = (typeInitialize_DLL_Api*)GetProcAddress(hInfoDll, _T("Initialize_DLL_Api"));
	if (Initialize_DLL_Api)
		Initialize_DLL_Api();
	
	typeGetScanResultApiLen *GetScanResultApiLen = (typeGetScanResultApiLen*)GetProcAddress(hInfoDll, _T("GetScanResultApiLen"));
	if (GetScanResultApiLen)
	{
		nRet = GetScanResultApiLen();
		pValue = std::make_unique<char[]>(nRet);
		FillMemory(desB, nRet, ' ');
		desB[nRet] = 0x00;
	}

	typeGetScanResultApi *GetScanResultApi = (typeGetScanResultApi*)GetProcAddress(hInfoDll, _T("GetScanResultApi"));
	if (GetScanResultApi && pValue)
	{
		nRet = GetScanResultApi(pValue.get());
		CopyMemory(desB, pValue.get(), nRet);
	}
	if (pValue)
		pValue.reset();

	FreeLibrary(hInfoDll);
	WriteLog(strFilePath,"FDS END");
	
	return nRet;
}
