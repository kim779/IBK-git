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