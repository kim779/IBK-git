// Palette.cpp: implementation of the CAxisPalette class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axislib.h"
#include "Palette.inc"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	nSYSCOLOR	64
#define	nRGB		3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxisPalette::CAxisPalette()
{
	m_rgbs.RemoveAll();
}

CAxisPalette::CAxisPalette(char *rgbN)
{
//	CString slog;
//	slog.Format("[axislib][CAxisPalette][GetPaletteRGB] CAxisPalette\n ");
//	OutputDebugString(slog);

	char	rgbV[128];
	CString	rgb, tmps;
	int	idx, val[nRGB];

	m_rgbs.RemoveAll();
	for (int ii = 0; ii < MAX_axisPALETTE-nSYSCOLOR; ii++)
	{
		tmps.Format("RGB%03d", ii);
		GetPrivateProfileString(_T("Palette"), tmps, "", rgbV, sizeof(rgbV), rgbN);

		rgb = rgbV;
		for (int kk = 0; kk < nRGB; kk++)
		{
			idx = rgb.Find(',');
			if (idx != -1)
			{
				tmps = rgb.Left(idx++);
				rgb  = rgb.Mid(idx);
			}
			else
			{
				tmps = rgb;
				rgb.Empty();
			}

			val[kk] = min(max(0, atoi(tmps)), MAX_axisPALETTE-1);
			if (rgb.IsEmpty())
				break;
		}
		axisRGB[ii] = PALETTERGB(val[0], val[1], val[2]);
	}

	SetPalette(rgbN);
}

CAxisPalette::~CAxisPalette()
{
}

COLORREF CAxisPalette::GetPaletteRGB(int index)
{
//	CString slog;
//	slog.Format("[axislib][CAxisPalette][GetPaletteRGB] index=[%d] val=[%d]\n ", min(max(0, index), MAX_axisPALETTE - 1));
//	OutputDebugString(slog);
	return axisRGB[min(max(0, index), MAX_axisPALETTE-1)];
}

int CAxisPalette::GetIndexRGB(COLORREF rgb)
{
	for (int ii = 0; ii < MAX_axisPALETTE; ii++)
	{
		if (axisRGB[ii] == rgb)
			return ii;
	}
	return -1;
}

void CAxisPalette::SetPalette(char *rgbN)
{
//	CString slog;
//	slog.Format("[axislib][CAxisPalette][SetPalette]  \n ");
//	OutputDebugString(slog);

	char	section[80], rgbV[128];
	CString	rgb, tmps;
	int	idx, val[nRGB];

	m_rgbs.RemoveAll();
	GetPrivateProfileString("General", "Palette", "", section, sizeof(section), rgbN);
	if (strlen(section))
	{
		for (int ii = 0; ii < MAX_axisPALETTE-nSYSCOLOR; ii++)
		{
			tmps.Format("RGB%03d", ii);
			GetPrivateProfileString(section, tmps, "", rgbV, sizeof(rgbV), rgbN);

			rgb = rgbV;
			if (rgb.IsEmpty())
				continue;
			for (int kk = 0; kk < nRGB; kk++)
			{
				idx = rgb.Find(',');
				if (idx != -1)
				{
					tmps = rgb.Left(idx++);
					rgb  = rgb.Mid(idx);
				}
				else
				{
					tmps = rgb;
					rgb.Empty();
				}

				val[kk] = min(max(0, atoi(tmps)), MAX_axisPALETTE-1);
				if (rgb.IsEmpty())
					break;
			}
			axisRGB[ii] = PALETTERGB(val[0], val[1], val[2]);
			m_rgbs.SetAt(ii, axisRGB[ii]);
		}
	}
}

BOOL CAxisPalette::Lookup(int index, COLORREF& rgb)
{
//	CString slog;
//	slog.Format("[axislib][CAxisPalette][GetPaletteRGB] Lookup index=[%d] \n ", index);
//	OutputDebugString(slog);
	return m_rgbs.Lookup(index, rgb);
}
