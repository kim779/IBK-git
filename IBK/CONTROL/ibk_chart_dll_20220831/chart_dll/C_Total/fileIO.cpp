#include "stdafx.h"
#include "C_Total.h"
#include "fileIO.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFileIO::CFileIO()
{
}

CFileIO::~CFileIO()
{

}

bool CFileIO::ReadFile(CWnd *pView, CString path, char *pBytes, int nBytes)
{
	DWORD	dwAccess = GENERIC_READ;
	DWORD	dwShareMode = FILE_SHARE_READ;
	DWORD	dwDistribution = OPEN_EXISTING;

	HANDLE hFile = CreateFile(path.operator LPCTSTR(), dwAccess,
				dwShareMode, NULL, dwDistribution,
				FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("hFile Error [%d] on OpenHandle\n", GetLastError());
		AfxGetMainWnd()->MessageBox("������ ���� ������ �����߽��ϴ�.", "Ȯ��", MB_OK);

		return false;
	}

	char	*rWb = new char[nBytes + 1];
	DWORD	dwReadLen;

	BOOL bRc = ::ReadFile(hFile, rWb, nBytes, &dwReadLen, NULL);
	CloseHandle(hFile);
	if (dwReadLen != DWORD(nBytes))
	{
		BUF_DELETE(rWb);
		//delete rWb;
		return false;
	}
	CopyMemory(pBytes, rWb, nBytes);
	delete rWb;

	return true;
}

bool CFileIO::WriteFile(CWnd *pView, CString path, char *pBytes, int nBytes)
{
	DWORD	dwAccess = GENERIC_WRITE;
	DWORD	dwShareMode = FILE_SHARE_WRITE;
	DWORD	dwDistribution = CREATE_ALWAYS;

	HANDLE hFile = CreateFile(path.operator LPCTSTR(), dwAccess,
				dwShareMode, NULL, dwDistribution,
				FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("hFile Error [%d] on OpenHandle\n", GetLastError());
		AfxGetMainWnd()->MessageBox("������ ���� ������ �����߽��ϴ�.", "Ȯ��", MB_OK);

		return false;
	}

	DWORD	dwWriteLen;
	BOOL bRc = ::WriteFile(hFile, pBytes, nBytes, &dwWriteLen, NULL);
	CloseHandle(hFile);
	if (!bRc)
	{
		AfxGetMainWnd()->MessageBox("������ ���� ������ �����߽��ϴ�.", "Ȯ��", MB_OK);
		return false;
	}
	return true;
}

bool CFileIO::GetFilenameInPath(CString &strPath, CString name, CStringArray &aryFname)
{
	aryFname.RemoveAll();

	HANDLE	hFile;
	BOOL	bSuccess = FALSE;
	WIN32_FIND_DATA	findfiledata;

	hFile = ::FindFirstFile(strPath.operator LPCTSTR(), &findfiledata);
	if (hFile == INVALID_HANDLE_VALUE)	return FALSE;

	name = name + "_";
	UINT	cLen = name.GetLength();
	CString fname;

	if (!(findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (strlen(findfiledata.cFileName) > cLen)
		{
			fname.Format("%s", findfiledata.cFileName);
			if (fname.Left(cLen) == name)
				aryFname.Add(fname.Mid(cLen));
		}
	}
	while(1)
	{
		bSuccess = ::FindNextFile(hFile, &findfiledata);
		if (!bSuccess)
		{
			::FindClose(hFile);
			break;
		}
		if (!(findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (strlen(findfiledata.cFileName) <= cLen)
				continue;

			fname.Format("%s", findfiledata.cFileName);
			if (fname.Left(cLen) == name)
				aryFname.Add(fname.Mid(cLen));
		}
	}
	return TRUE;
}

BOOL CFileIO::ImageWindow(CWnd *pMain, CString& path)
{
	CBitmap 	bitmap;
	CWindowDC	dc(pMain);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc); 
	pMain->GetWindowRect(&rect);

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY); 

	// Create logical palette if device support a palette
	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = 
			GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);

		// Create the palette
		pal.CreatePalette(pLP);

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)
		return FALSE;

	CFileDialog	dlg(FALSE, "bmp", path, OFN_OVERWRITEPROMPT, 
		"BMP Files(*.bmp)|*.bmp||", NULL);
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
		if (dlg.GetFileName().IsEmpty())
		{
			GlobalFree(hDIB);
			return FALSE;	
		}
	}
	else
	{
		GlobalFree(hDIB);
		return FALSE;
	}

	//szFile = (LPSTR)(LPCTSTR)path;
	// Write it to file
	WriteDIB((LPSTR)(LPCTSTR)path, hDIB);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);
	return TRUE;
}

void CFileIO::PrintWindow(CWnd *pMain, bool bDlgPopup)
{
	HDC	hdcPrinterDC;

	CRect	WinRC;

	pMain->GetWindowRect(WinRC);
	CDC *pMainDC = pMain->GetDC();

	CDC	memDC;
	CBitmap	bitmap;
	// ȭ��ũ���� Bitmap����
	if (!bitmap.CreateCompatibleBitmap( pMainDC, WinRC.Width(), WinRC.Height()))
	{
		TRACE("failed to CreateCompatibleBitmap\n");
		return;
	}

	//�ʱ�ȭ
	pMain->ReleaseDC(&memDC);
	memDC.DeleteDC();

	memDC.CreateCompatibleDC(pMainDC);
	memDC.SelectObject(&bitmap);

	memDC.BitBlt(0, 0, WinRC.Width(), WinRC.Height(), 
		pMainDC, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	pMain->ReleaseDC(pMainDC);

	if (bDlgPopup)
	{
		PRINTDLG	pd;
		memset(&pd, 0, sizeof(PRINTDLG));
		pd.lStructSize = sizeof(PRINTDLG);
		pd.Flags = PD_RETURNDC;
		pd.hwndOwner = NULL;
		pd.nFromPage = 1;
		pd.nToPage = 1;
		pd.nMinPage = 1;
		pd.nMaxPage = 1;
		pd.nCopies = 1;
		PrintDlg(&pd);
		hdcPrinterDC = pd.hDC;        // ����Ʈ DC�ʱ�ȭ 
		//HDC hdcPrinter = printDlg.GetPrinterDC();    //Printer�� DC�� ���´�.
		if (hdcPrinterDC == NULL)	//�μ� ���̾�α׿��� ���������..
		    return;
	}
	else
	{
		char	szPrinter[80];
		char	*szDevice, *szDriver, *szOutput;

		GetProfileString("windows", "device", ",,,", szPrinter, 80);
		szDevice = strtok(szPrinter, ",");
		szDriver = strtok(NULL, ", ");
		szOutput = strtok(NULL, ", ");
		hdcPrinterDC = CreateDC(szDriver, szDevice, szOutput, NULL);

		if (!hdcPrinterDC) 
			return;
	}

	DOCINFO di;			// DOCINFO��Ʈ����.
	memset( &di, 0, sizeof(di));	// di�� �� ���� Ȯ��
	di.cbSize = sizeof(di);		// DOCINFO��Ʈ���� �ʱ�ȭ
	di.fwType = 0;			// ���� �߰����� [���]�� ����� ��� 
					// DI_APPBANDING�� ��� nt������ ����. 0���� ����
	di.lpszDatatype = NULL;		// print JOB �� ����ϴ� ������ Ÿ�� nt�ϰ�� ����. �Ű澲������
	di.lpszDocName = "TEST";        
	di.lpszOutput = NULL;		// ������� �̸� NULL�� ��� printDC�� ��µȴ�.


	if (StartDoc( hdcPrinterDC, &di) < 0)	// ������ ���� �˸�
	{
		pMain->MessageBox("����Ʈ ������ ������ �ְų� ������ �����ֽ��ϴ�.", COMPANYNAME);
	}
	else
	{
		if (StartPage( hdcPrinterDC) < 0)	// ����������..
		{
			pMain->MessageBox("Not start page", COMPANYNAME);
		}
		else	// ���� ��µǴ� �κ�.
		{
			// ����� ����ũ�⸦ ������ �´�
			int dpiX,dpiY;
			dpiX = GetDeviceCaps(hdcPrinterDC, VERTRES);
			dpiY = GetDeviceCaps(hdcPrinterDC, HORZRES);
			// �޸� DC�� �ִ� ��Ʈ��ũ�⸦ ����ũ�⿡ �°� ������
			::StretchBlt(hdcPrinterDC,
				0,
				200,
				dpiY - 100,
				dpiX*2/3,	// ���� �̻���
				//dpiX-400,	// ���� ����
				memDC.m_hDC,
				0,
				0,
				WinRC.Width(),
				WinRC.Height(),
				SRCCOPY);
			EndPage(hdcPrinterDC);	// ������ ��
		}
        
		EndDoc(hdcPrinterDC);		// ������ ����
		DeleteDC(hdcPrinterDC);		// ����Ʈ DC����
	}
}

HANDLE CFileIO::DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;

	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if (dwCompression == BI_BITFIELDS)	return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal == NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = int(pow(2, bi.biBitCount));
	if (nColors > 256)
		nColors = 0;

	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED, dwLen);
	if (!hDIB)
	{
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;
	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;
		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;

	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
	{
		hDIB = handle;
	}
	else
	{
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);	// Reselect the original palette
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if (!bGotBits)
	{
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	SelectPalette(hDC, hPal, FALSE);
	::ReleaseDC(NULL, hDC);

	return hDIB;
}

// WriteDIB		- Writes a DIB to file
// Returns		- TRUE on success
// szFile		- Name of file to write to
// hDIB			- Handle of the DIB
BOOL CFileIO::WriteDIB(LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if(!file.Open(szFile, CFile::modeWrite|CFile::modeCreate))
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = int(pow(2, lpbi->biBitCount));
	if (nColors > 256)
		nColors = 0;

	// Fill in the fields of the file header 
	hdr.bfType	= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize	= GlobalSize (hDIB) + sizeof(hdr);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits	= (DWORD) (sizeof(hdr) + lpbi->biSize +	nColors * sizeof(RGBQUAD));

	// Write the file header 
	file.Write(&hdr, sizeof(hdr));

	// Write the DIB header and the bits 
	file.Write(lpbi, GlobalSize(hDIB));

	return TRUE;
}
