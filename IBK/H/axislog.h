#pragma once
#include <vadefs.h>
#include <stdarg.h>
#include <cstring>

static CString Int2CString(CString stmp, int ival)
{
	CString slog;
	slog.Format("[%s = %d]", stmp, ival);
	return slog;
}

static void LOG_OUTP(int scnt, ...)
{
	va_list argList;
	va_start(argList, scnt);

	CString sTmp, sResult;

	for (int i = 0; i < scnt; i++)
	{
		if (i == 0)
			sTmp.Format("<%s>", va_arg(argList, LPCTSTR));
		else
			sTmp.Format("[%s]", va_arg(argList, LPCTSTR));

		sResult += sTmp;
		if (i == 0)
			sResult += "   ";
		else
			sResult += " ";
	}
	va_end(argList);

	OutputDebugString("\r\n" + sResult);
}

void FileLog(LPCSTR log, ...)
{
#if 1
	TRY
	{
		char buf[256]{};
		GetCurrentDirectory(256, buf);
		CString spath;
		spath.Format("%s\\%s", buf, "axis.log");
		spath.TrimRight();

		FILE* fp;
		fopen_s(&fp, spath, "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}