#include "stdafx.h"
#include <assert.h>
#include <Winsock2.h>
#include "resource.h"	
#include "winprint.h"
#include "imageload.h"			

//20080711 �̹��� >>
#define COMPILE_MULTIMON_STUBS
#include "multimon.h"
//20080711 �̹��� <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IMAGETYPE_NONE	0
#define IMAGETYPE_BMP	1
#define IMAGETYPE_GIF	2
#define IMAGETYPE_PCX	3
#define IMAGETYPE_TGA	4
#define IMAGETYPE_JPG	5
#define IMAGETYPE_TIF	6

#define IMAGETYPE_FIRSTTYPE IMAGETYPE_BMP
#define IMAGETYPE_LASTTYPE	IMAGETYPE_TIF


char * szExtensions[] = { ".BMP", ".GIF", ".PCX", ".TGA", ".JPG", ".TIF", "" };


//20080711 �̹��� >>
typedef CArray < CRect, CRect & > CRectArray;

BOOL CALLBACK MonitorEnumProc( HMONITOR hMonitor,  // handle to display monitor
							  HDC hdcMonitor,     // handle to monitor-appropriate device context
							  LPRECT lprcMonitor, // pointer to monitor intersection rectangle
							  LPARAM dwData       // data passed from EnumDisplayMonitors
							  )
{
	CRect rect = *lprcMonitor;
	
	if(dwData)
	{
		CRectArray * pRectArray = (CRectArray *)dwData;
		pRectArray->Add(rect);
	}
	return TRUE;
}
//20080711 �̹��� <<

char 		szPrintDlg[] = "PRINTING";    // Name of Print dialog from .RC

CWinPrint::CWinPrint()
{
	bPrintContinue	= FALSE;
	nPrintPage		= 0;

	//20080711 �̹��� >>
	m_hMainWnd		= NULL; 
	m_hMDIClient	= NULL;
	//20080711 �̹��� <<

}
CWinPrint::~CWinPrint()
{
}

//===========================================================================//
// �Լ���  : fnPrintWindow                                                   //
// �Լ����: ĸ�ĸ� ���� ���                                                //
// �Ķ��Ÿ: HWND hFrameWnd      : ���� ������ �ڵ�                          //
//           HWND hPrintWnd      : ALP+P�� ����� ����Ʈ ȭ��                //
//           HINSTANCE hInstance : DLL�� �θ��� �������� �ν��Ͻ�            //
// ���ϰ�  : BOOL                                                            //
//===========================================================================//
BOOL CWinPrint::fnPrintWindow(	int niGubun,
								HWND hFrameWnd, 
								HDC hPrintDC, 
								CString strFileName,
								BOOL bPortrait ,
								HDIB _hDIB,
								CString strTitle,
								CString strMsg )
{
    HWND            hWndSelect =  hFrameWnd;    // ����Ʈ ȭ�� �ڵ� 
    HDIB            hDIB = _hDIB;				// handle to DIB
    BOOL            nErrorCheck;
  
    ghWndFrame = hFrameWnd;
	m_Portrait = bPortrait;

	memset(lpWindowTitle,0x00,50);
	::GetWindowText(hWndSelect,lpWindowTitle,50);
    // ���õ� ������ ��Ƽ�� 
    SetActiveWindow(hWndSelect);
    // ���õ� �����츦 ĸ���ϱ����� ���� 
    SetWindowPos(hWndSelect, NULL, 0, 0, 0, 0,
                 SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE);
    UpdateWindow(hWndSelect);

    // ���õ� �������� Ÿ��Ʋ�� ��´�.
    gnWindowsTextSize = strlen(lpWindowTitle);
    
    //lpPrintBuff = new char[101];
    // ���õ� �����쿡�� DIB�� ��´�.
	
    if(!hDIB)
		hDIB = CopyWindowToDIB(hWndSelect);

	if( niGubun == 500 || niGubun == 600 )
        SaveDIB((HANDLE) hDIB, (LPSTR)(LPCTSTR)strFileName);
	else 
		  nErrorCheck = PrintWindowDIB(hDIB,hPrintDC);     

    return nErrorCheck;
}
//===========================================================================//
// �Լ���  : SelectWindow                                                    //
// �Լ����: ���콺�� ������ �������� �ڵ��� ����                            //
// �Ķ��Ÿ: NULL                                                            // 
// ���ϰ�  : HWND : ������ ������ �ڵ�                                       //
//===========================================================================//
HWND CWinPrint::SelectWindow()
{
    HCURSOR hOldCursor = NULL;     // Handle to old cursor
    POINT pt;               // Stores mouse position on a mouse click
    HWND hWndClicked;       // Window we clicked on
    MSG msg;

    // ���콺 �޼����� �����찡 ���õ� ���� ��� �޴´�. 
    SetCapture(ghWndFrame);
    
    // ���콺 ����� �ٲ۴�.
  //  HCURSOR hCurSelect = AfxGetApp()->LoadCursor(IDR_MAINFRAME);
  //  hOldCursor = SetCursor(hCurSelect);

    // Eat mouse messages until a WM_LBUTTONUP is encountered.
    // WM_LBUTTONUP�޼����� �߻��ҵ��� ����  
    for (;;){
    WaitMessage();
    if (PeekMessage(&msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)) {
        if (msg.message == WM_LBUTTONUP) {
            // ���콺 �������� ��´�.
            pt.x = LOWORD(msg.lParam);
            pt.y = HIWORD(msg.lParam);

            // ȭ�� ������ �ٲ۴�.
            ClientToScreen(ghWndFrame, &pt);
            // Ŭ���� �쵵�� �ڵ��� ��´�. 
            hWndClicked = WindowFromPoint(pt);
            // ������ �ڵ��� �˻� 
            if (!hWndClicked) {
                ReleaseCapture();
                SetCursor(hOldCursor);
                return NULL;
            }
        break;
        }           

    }
    else
        continue;
    }

    ReleaseCapture();
    SetCursor(hOldCursor);
    return (hWndClicked);
}

//===========================================================================//
// �Լ���  : PrintWindowDIB                                                  //
// �Լ����: hDIB�� ����Ʈ                                                   //
// �Ķ��Ÿ: HDIB hDIB : ����Ʈ�� hDIB                                       //
// ���ϰ�  : BOOL      : ����Ʈ�� ��������                                   //
//===========================================================================//
BOOL CWinPrint::PrintWindowDIB(HDIB hDIB ,HDC hPrintDC) 
{
    static OPTIONSTRUCT opts;
    int     iReturn = TRUE;
    BOOL    wReturn=TRUE;

/*    // Hot Ket�� ���� ����Ʈ ���� �˻� 
    if( nPrintHotKey ) {
        opts.iOption = IDC_BESTFIT;
        bPrintContinue = FALSE;
        iReturn = TRUE;

    } else {
        // first page print
        if( nPrintPage == 0 ) {
            CPrintDlg z_SabPrintDlg(NULL, &opts);
            iReturn = z_SabPrintDlg.DoModal();
        }
        else 
            iReturn = TRUE;
    }          
  */      
    if (iReturn) {
        if (hDIB) {
            if (iReturn == 2)       // ���� check
                bPrintContinue = TRUE;

            wReturn = PrintDIB(hDIB,hPrintDC ,PW_BESTFIT, 0, 0);
            // ����Ʈ�� ���� hDIB�� �޸𸮸� ���� 
            DestroyDIB(hDIB);
        }
    }
    return wReturn;
}

//===========================================================================//
// �Լ���  : CopyWindowToDIB                                                 //
// �Լ����: ���õ� �����츦 DIB�� �����.                                   //
// �Ķ��Ÿ: HWND   hWnd : ���õ� ������                                     //
// ���ϰ�  : HDIB        : �������� DIB                                      //
//===========================================================================//
HDIB CWinPrint::CopyWindowToDIB(HWND hWnd)
{
    HDIB                hDIB;  // handle to DIB
    RECT                rectClient;

    // ������ �ڵ��� �˻� 
    if (!hWnd)
        return NULL;

	GetWindowRect(hWnd, &rectClient);

    // ������ ȭ���� DIB�� �����Ѵ�.
	//20080711 �̹��� >>
    //hDIB = CopyScreenToDIB(&rectClient);
	hDIB = CopyScreenToDIB(hWnd, &rectClient);
	//20080711 �̹��� <<

    return hDIB;
}

//===========================================================================//
// �Լ���  : CopyScreenToDIB                                                 //
// �Լ����: ������ ������ HDIB�� ��´�.                                    //
// �Ķ��Ÿ: LPRECT lpRect : ������ �������� ����                            //
// ���ϰ�  : HDIB        : �������� DIB                                      //
//===========================================================================//
//20080711 �̹��� >>
//HDIB CWinPrint::CopyScreenToDIB(HWND hWnd, LPRECT lpRect)
HDIB CWinPrint::CopyScreenToDIB(HWND hWnd, LPRECT lpRect)
//20080711 �̹��� <<
{
    HBITMAP hBitmap;    // handle to device-dependent bitmap
    HPALETTE hPalette;  // handle to palette
    HDIB hDIB=NULL;     // handle to DIB

    // ȭ���� ��Ʈ���� ���� 
	//20080711 �̹��� >>
	//hBitmap = CopyScreenToBitmap(lpRect);
    hBitmap = CopyScreenToBitmap(hWnd, lpRect);
	//20080711 �̹��� <<

    if (!hBitmap)
        return NULL;

    // ������ �ķ�Ʈ�� ��´�.
    hPalette = GetSystemPalette();

    // ��Ʈ���� DIB�� ��ȯ 
    hDIB = BitmapToDIB(hBitmap, hPalette);

    DeleteObject(hBitmap);
    DeleteObject(hPalette);

    return hDIB;
}


HDIB CWinPrint::CopyBmpToDIB(HBITMAP hBitmap)
{
	HPALETTE hPalette;  // handle to palette
    HDIB hDIB=NULL;     // handle to DIB

    if (!hBitmap)
        return NULL;

    // ������ �ķ�Ʈ�� ��´�.
    hPalette = GetSystemPalette();

    // ��Ʈ���� DIB�� ��ȯ 
    hDIB = BitmapToDIB(hBitmap, hPalette);
    
    DeleteObject(hPalette);

    return hDIB;
}

//===========================================================================//
// �Լ���  : CopyScreenToBitmap                                              //
// �Լ����: ������ ������ HBITMAP�� ��´�.                                 //
// �Ķ��Ÿ: LPRECT lpRect : ������ �������� ����                            //
// ���ϰ�  : HBITMAP       : �������� HDIB                                   //
//===========================================================================//
HBITMAP CWinPrint::CopyScreenToBitmap(HWND hWnd, LPRECT lpRect)
{
    HDC     hScrDC, hMemDC;             // screen DC and memory DC
    HBITMAP hBitmap;
    HGDIOBJ hOldBitmap;        // handles to deice-dependent bitmaps
    int     nX, nY, nX2, nY2;           // coordinates of rectangle to grab
    int     nWidth, nHeight;            // DIB width and height
    


    // ȭ���� ��� �ִ����� �˻� 
    if (IsRectEmpty(lpRect))
        return NULL;



	hScrDC = ::GetWindowDC(hWnd);//20080711 �̹��� >>	



    // ȭ�� DC�� �޸� DC�� �����Ѵ�.
    //hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 20080711 �̹��� 
    hMemDC = CreateCompatibleDC(hScrDC);


    nX	= lpRect->left;
    nY	= lpRect->top;
    nX2 = lpRect->right;
    nY2 = lpRect->bottom;


    nWidth = nX2 - nX;
    nHeight = nY2 - nY;

    // ȭ���� ��Ʈ������ ��´�.
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

    // ��Ʈ���� �޸� DC�� �ű��.
    hOldBitmap = SelectObject(hMemDC, hBitmap);


	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX - lpRect->left, nY - lpRect->top, SRCCOPY);
	

    // ��Ʈ�ʿ� �簢�� �׵θ� �׸��� 
    SelectObject(hMemDC, GetStockObject(BLACK_PEN));
    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
    Rectangle(hMemDC, 0, 0, nWidth, nHeight);
    
    
    // ������ DC�� ���� ���´�.
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    
	
    DeleteDC(hMemDC);
	ReleaseDC(hWnd, hScrDC);

	return hBitmap;
}

//===========================================================================//
// �Լ���  : GetSystemPalette                                                //
// �Լ����: ������ ������ HBITMAP�� ��´�.                                 //
// �Ķ��Ÿ: VOID       :                                                    //
// ���ϰ�  : HPALETTE   : �ķ�Ʈ                                             //
//===========================================================================//
HPALETTE CWinPrint::GetSystemPalette ()
{
    HDC           hDC;
    HPALETTE      hPal = NULL;
    HANDLE        hLogPal;
    LPLOGPALETTE  lpLogPal;
    int           i, nColors;


    // �ķ�Ʈ�� ã�´�.
    hDC = GetDC (NULL);
    if (!hDC)
        return NULL;

    // device context�� �ķ�Ʈ ��� ���� ã�Ƴ���.
    nColors = PalEntriesOnDevice (hDC);
    ReleaseDC (NULL, hDC);

    // �ķ�Ʈ ��� �� ��ŭ �޸� �Ҵ�
    hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
                           nColors * sizeof (PALETTEENTRY));
    if (!hLogPal)
        return NULL;

    lpLogPal = (LPLOGPALETTE) GlobalLock (hLogPal);

    lpLogPal->palVersion    = PALVERSION;
    lpLogPal->palNumEntries = nColors;

    for (i = 0;  i < nColors;  i++) {
        lpLogPal->palPalEntry[i].peBlue  = 0;
        *((LPWORD) (&lpLogPal->palPalEntry[i].peRed)) = i;
        lpLogPal->palPalEntry[i].peFlags = PC_EXPLICIT;
    }

    // ���� �ķ�Ʈ�� ���� 
    hPal = CreatePalette (lpLogPal);

    GlobalUnlock (hLogPal);
    GlobalFree (hLogPal);

    return hPal;
}         

//===========================================================================//
// �Լ���  : PalEntriesOnDevice                                                //
// �Լ����: ������ ����̽��� �ķ�Ʈ ��� ��                                //
// �Ķ��Ÿ: HDC    hDC : device context                                     //
// ���ϰ�  : int        : device �ķ�Ʈ�� ��� ��                            //
//===========================================================================//
int CWinPrint::PalEntriesOnDevice (HDC hDC)
{
    int nColors;

    // ����̽��� �ķ�Ʈ ��� ���� ��´�.
    nColors = GetDeviceCaps (hDC, SIZEPALETTE);

    // �ķ�Ʈ�� ��� ���� 0�̸� �ý��� �ķ�Ʈ���� ã�Ƴ���.
    if (!nColors)
        nColors = GetDeviceCaps (hDC, NUMCOLORS);

    assert (nColors);

    return nColors;
}

//===========================================================================//
// �Լ���  : BitmapToDIB                                                     //
// �Լ����: ��Ʈ���� DIB�� ��ȯ                                             //
// �Ķ��Ÿ: HBITMAP hBitmap : ��ȯ�� ��Ʈ��                                 //
//           HPALETTE hPal   : ������ �ķ�Ʈ                                 //
// ���ϰ�  : HDIB            : ��ȯ�� DIB                                    //
//===========================================================================//
HDIB CWinPrint::BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal)
{
    BITMAP             Bitmap;
    BITMAPINFOHEADER   bmInfoHdr;
    LPBITMAPINFOHEADER lpbmInfoHdr;
    LPSTR              lpBits;
    HDC                hMemDC;
    HANDLE             hDIB;
    HPALETTE           hOldPal = NULL;

    if (!hBitmap)
        return NULL;
    
    // ��Ʈ���� ������ ��´�.
    if (!GetObject (hBitmap, sizeof (Bitmap), (LPSTR) &Bitmap))
        return NULL;

    // ��Ʈ���� ��� ������ ��´�.
    InitBitmapInfoHeader (&bmInfoHdr,
                         Bitmap.bmWidth,
                         Bitmap.bmHeight,
                         Bitmap.bmPlanes * Bitmap.bmBitsPixel);


    // DIB �޸𸮸� �Ҵ��Ѵ�.
    hDIB = GlobalAlloc (GHND, sizeof (BITMAPINFOHEADER) +
                        PaletteSize ((LPSTR) &bmInfoHdr) + bmInfoHdr.biSizeImage);

    if (!hDIB)
        return NULL;

    lpbmInfoHdr  = (LPBITMAPINFOHEADER) GlobalLock (hDIB);
    *lpbmInfoHdr = bmInfoHdr;
    lpBits       = FindDIBBits((LPSTR) lpbmInfoHdr);

    // ��Ʈ���� DIB�� ��ȯ�ϱ����� DC�� ��´�.
    hMemDC       = GetDC (NULL);

    // �ķ�Ʈ�� �˻��Ͽ� �ķ�Ʈ�� ���� 
    if(hPal) 
	{
        hOldPal = SelectPalette (hMemDC, hPal, FALSE);
        RealizePalette (hMemDC);
    }	

    // �۷ι� �޸𸮿� DIB�� �����Ѵ�.
    if (!GetDIBits(hMemDC, hBitmap, 0, Bitmap.bmHeight, lpBits,
                   (LPBITMAPINFO) lpbmInfoHdr, DIB_RGB_COLORS)) {
        GlobalUnlock (hDIB);
        GlobalFree (hDIB);
        hDIB = NULL;
    }
    else 
        GlobalUnlock (hDIB);
   
    if (hOldPal)
        SelectPalette (hMemDC, hOldPal, FALSE);

    ReleaseDC (NULL, hMemDC);

    return hDIB;
}

//===========================================================================//
// �Լ���  : InitBitmapInfoHeader                                            //
// �Լ����: BITMAPINFOHEADER�� �ʱ�ȭ, ����,����, �ȼ��� ����Ʈ             //
// �Ķ��Ÿ: LPBITMAPINFOHEADER lpBmInfoHdr : ��Ʈ�� �������                //
//           DWORD dwWidth                  : ��Ʈ�� ����                    //
//           DWORD dwHeight                 : ��Ʈ�� ����                    //
// ���ϰ�  : void                                                            //
//===========================================================================//
void CWinPrint::InitBitmapInfoHeader (LPBITMAPINFOHEADER lpBmInfoHdr, DWORD dwWidth,
                           DWORD dwHeight, int nBPP)
{
   memset (lpBmInfoHdr, 0, sizeof (BITMAPINFOHEADER));

   lpBmInfoHdr->biSize      = sizeof (BITMAPINFOHEADER);
   lpBmInfoHdr->biWidth     = dwWidth;
   lpBmInfoHdr->biHeight    = dwHeight;
   lpBmInfoHdr->biPlanes    = 1;

   if (nBPP <= 1)
      nBPP = 1;
   else if (nBPP <= 4)
      nBPP = 4;
   else if (nBPP <= 8)
      nBPP = 8;
   else
      nBPP = 24;

   lpBmInfoHdr->biBitCount  = nBPP;
   lpBmInfoHdr->biSizeImage = WIDTHBYTES (dwWidth * nBPP) * dwHeight;
}

//===========================================================================//
// �Լ���  : FindDIBBits                                                     //
// �Լ����: DIB�� ��Ʈ �ּ�                                                 //
// �Ķ��Ÿ: LPSTR lpbi : DIB�� �޸� �ּ�                                  //
// ���ϰ�  : LPSTR      : DIB bit�� �ּ�                                     //
//===========================================================================//
LPSTR CWinPrint::FindDIBBits (LPSTR lpbi)
{
   return (lpbi + *(LPDWORD)lpbi + PaletteSize (lpbi));
}

//===========================================================================//
// �Լ���  : PrintDIB                                                        //
// �Լ����: hDIB print band or non-banding(����Ʈ�� band�� ���� ���ο� ���� //
// �Ķ��Ÿ: HDIB hDIB      : ����Ʈ�� hDIB                                  //
//           WORD fPrintOpt : ����Ʈ �ɼ�                                    //
//           WORD wXScale   : ����Ʈ ���� X ����                             //
//           WORD wYScale   : ����Ʈ ���� Y ����                             //
// ���ϰ�  : BOOL      : ����Ʈ�� ��������                                   //
//===========================================================================//
BOOL CWinPrint::PrintDIB(HDIB hDib, HDC hPrintDC,WORD fPrintOpt, WORD wXScale, WORD wYScale)  
{
    RECT rect;                   // ����� ���� Rect ����ü 
    LPSTR lpBits;                // pointer to the DIB bits
    LPBITMAPINFOHEADER lpDIBHdr; // Pointer to DIB header
    int nBandCount = 0;          // used for print dialog box to count bands
    BOOL wErrorCode=TRUE;         // Error code to return
    RECT rPrintRect;             
    char szJobNameTrunc[35];   
    char szBuffer[100];        
    DOCINFO stDocInfo;

    if (!hDib)
        return FALSE;

    // DIB ����Ʈ ��带 ��´�.
    lpDIBHdr = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (!lpDIBHdr) 
        return FALSE;
    // DIB bits�� ����Ʈ�� ã�´�. 
    lpBits = FindDIBBits((LPSTR)lpDIBHdr); 

    // ù��° ����Ʈ�̸� ����Ʈ DC�� ��´�. 
    if( nPrintPage == 0 )
	{
		if(hPrintDC == NULL)
		{
			if( !(hPrnDC = GetPrinterDC()) )
				return FALSE;
		}
		else hPrnDC = hPrintDC;
	}
            
    SetStretchBltMode(hPrnDC, COLORONCOLOR);
    // ����Ʈ�� ���� ��� 
    CalculatePrintRect(hPrnDC, &rPrintRect, fPrintOpt, lpDIBHdr->biWidth,
                       lpDIBHdr->biHeight);


//	rPrintRect = CRect (0,0,500,500);
    rPrintRect.top = 20;

    // ������ ���̾˷α� ���� 
   // hDlgAbort = CreateDialog(AfxGetInstanceHandle(), szPrintDlg, GetFocus(), PrintAbortDlg);

    // ������ ���¸� ��Ÿ����.
    memcpy(szJobNameTrunc, lpWindowTitle, gnWindowsTextSize+1);
    wsprintf(szBuffer, "�μ��� '%s'", (LPSTR)szJobNameTrunc);
//    SetDlgItemText(hDlgAbort, IDC_PRINTTEXT1, (LPSTR)szBuffer);
    bAbort = FALSE;

    // ABOUT PROC�� ���� 
 //   if (SetAbortProc(hPrnDC, PrintAbortProc) < 0)
  //      return FALSE;

    // ù��° ����Ʈ
	CString strMsg = "����Ʈ �۾��� ���� �� �� �����ϴ�.\n�����ͱ� ��ġ�� Ȯ�� �ٶ��ϴ�";
    if( nPrintPage == 0 ) {
        // Escape()�� STARTDOC�� ������ ���� 
        memset(&stDocInfo, 0x00, sizeof(stDocInfo));
        stDocInfo.cbSize = sizeof(stDocInfo);
        stDocInfo.lpszDocName = szJobNameTrunc;
        if (StartDoc(hPrnDC, &stDocInfo) < 0) {
            DestroyWindow(hDlgAbort); 
            DeleteDC(hPrnDC);
            GlobalUnlock(hDib);
			::MessageBox(ghWndFrame,strMsg ,"DRSCPRINT",MB_SYSTEMMODAL);
            return FALSE;
        }
        if (StartPage(hPrnDC) < 0) {
            DestroyWindow(hDlgAbort); 
            DeleteDC(hPrnDC);
            GlobalUnlock(hDib);
			::MessageBox(ghWndFrame,strMsg ,"DRSCPRINT",MB_SYSTEMMODAL);
            return FALSE;
        }
    }   

    // ����� �ʵ�  
    rect = rPrintRect;
 //   SetDlgItemText(hDlgAbort, IDC_PERCENTAGE, (LPSTR)
//                   "ȭ�� �μ� ��...");
    wErrorCode = PrintBand(hPrnDC, &rPrintRect, &rect, TRUE, TRUE,
                           lpDIBHdr, lpBits);
	
    // Abort procudure ���� 
    if (!bAbort) {
        // ù��° ����Ʈ���� ���� 
        if( (nPrintPage == 1) || (bPrintContinue == FALSE) ) {
            // Non-banding devices need a NEWFRAME 
            if (EndPage(hPrnDC) < 0)
                return FALSE;
            // ENDDOC�� �������� ������.
            if (EndDoc(hPrnDC) < 0)
                wErrorCode = FALSE;
            nPrintPage = 0;
            DeleteDC(hPrnDC);
        }
        else 
            nPrintPage++;
        DestroyWindow(hDlgAbort);
    }
    else {
        DestroyWindow(hDlgAbort);
        DeleteDC(hPrnDC);
        nPrintPage = 0;
    }
      
    GlobalUnlock(hDib);
    return wErrorCode;
}

//===========================================================================//
// �Լ���  : CalculatePrintRect                                              //
// �Լ����: fPrintOpt�� ���� ����Ʈ ������ ���                           //
// �Ķ��Ÿ: HDC hDC            : ����Ʈ DC                                  //
//           LPRECT lpPrintRect : ����Ʈ �ɼ�                                //
//           WORD cxDIB         : ����Ʈ�� DIB ����                          //
//           WORD cxDIB         : ����Ʈ�� DIB ����                          //
// ���ϰ�  : void                                                            //
//===========================================================================//
void CWinPrint::CalculatePrintRect(HDC hDC, LPRECT lpPrintRect, WORD fPrintOpt, 
                        DWORD cxDIB, DWORD cyDIB)  
{
    int cxPage, cyPage, cxInch, cyInch;

    if (!hDC)
        return;

    // ����Ʈ ����̺� ������ ��´�.
    cxPage = GetDeviceCaps(hDC, HORZRES);     // Width of printr page - pixels
    cyPage = GetDeviceCaps(hDC, VERTRES);     // Height of printr page - pixels
    cxInch = GetDeviceCaps(hDC, LOGPIXELSX);  // Printer pixels per inch - X
    cyInch = GetDeviceCaps(hDC, LOGPIXELSY);  // Printer pixels per inch - Y

    lpPrintRect->top = 0;
    lpPrintRect->left = 0;
	if(m_Portrait) //����
		lpPrintRect->bottom = cyPage/2;
	else
		lpPrintRect->bottom = cyPage - 100;

    lpPrintRect->right = cxPage;
}

//===========================================================================//
// �Լ���  : PrintBand                                                       //
// �Լ����: �־��� �������� ����Ʈ                                        //
// �Ķ��Ÿ: HDC hDC                     : ����Ʈ DC                         //
//           LPRECT lpRectOut            : ����Ʈ�� ����                     //
//           LPRECT lpRectClip           : ����Ʈ�� DIB ����                 //
//           BOOL fDoText                : ����Ʈ�� �ڷᰡ �ؽ�Ʈ����        //
//           BOOL fDoGraphics            : ����Ʈ�� �ڷᰡ �׷�������        //
//           LPBITMAPINFOHEADER lpDIBHdr : ����Ʈ�� DIB ���                 //
//           LPSTR lpDIBBits             : ����Ʈ�� DIB�� ���� �ּ�          //
// ���ϰ�  : BOOL : ����Ʈ ���� ����                                         //
//===========================================================================//
BOOL CWinPrint::PrintBand(HDC hDC, LPRECT lpRectOut, LPRECT lpRectClip, BOOL fDoText, 
               BOOL fDoGraphics, LPBITMAPINFOHEADER lpDIBHdr, LPSTR lpDIBBits)
{                                                
    RECT        rect;                            
    double      dblXScaling, dblYScaling;
//    HFONT		hFont, hFontOld;

    if (fDoGraphics) {
        dblXScaling = ((double)lpRectOut->right - lpRectOut->left) / 
                       (double)lpDIBHdr->biWidth;
        dblYScaling = ((double)lpRectOut->bottom - lpRectOut->top) / 
                       (double)lpDIBHdr->biHeight;
        // RectOut, RectClip�� ������ 
        IntersectRect(&rect, lpRectOut, lpRectClip);
        if (!IsRectEmpty(&rect)) {
            RECT rectIn;
            rectIn.left = (int)((rect.left - lpRectOut->left) / dblXScaling + 0.5);
            rectIn.top = (int)((rect.top - lpRectOut->top) / dblYScaling + 0.5);
            rectIn.right = (int)(rectIn.left + (rect.right - rect.left) /
                                 dblXScaling + 0.5);
            rectIn.bottom = (int)(rectIn.top + (rect.bottom - rect.top) /
                                  dblYScaling + 0.5);
            // ù��° ����Ʈ
            if( nPrintPage != 0 ) {
                rect.top = GetDeviceCaps(hPrnDC, VERTRES)/2;
                // ���� ����� ���� ũ�Ⱑ �۴�. 
                if( rect.bottom >= rect.top )
                    return FALSE;
                rect.bottom = rect.bottom*2 + 400;
                if( GetDeviceCaps(hPrnDC, VERTRES) <= rect.bottom )
                    rect.bottom = GetDeviceCaps(hPrnDC, VERTRES) - 400;
            }
            
            if( bPrintContinue ) {
                if( (GetDeviceCaps(hPrnDC, VERTRES)/2) <= rect.bottom )
                    bPrintContinue = FALSE;
            }
//            hFont = fnNewFont(40, 28, FW_BOLD, "����ü");
//            hFontOld = (HFONT)SelectObject(hDC, (HGDIOBJ)hFont);
//            TextOut(hDC, 10, rect.top+10, lpPrintBuff, strlen(lpPrintBuff)); 
//            SelectObject(hDC, hFontOld);
            // �־��� ����Ʈ ������ DIB�� ����Ʈ �Ѵ�. 
            if (!StretchDIBits(hDC, rect.left, rect.top+60, rect.right - rect.left, 
                               rect.bottom - rect.top, rectIn.left, 
                               (int)(lpDIBHdr->biHeight) - rectIn.top - (rectIn.bottom - rectIn.top),
                                rectIn.right - rectIn.left, rectIn.bottom - rectIn.top,  
                                lpDIBBits,  (LPBITMAPINFO)lpDIBHdr, DIB_RGB_COLORS, SRCCOPY))
                                
                return FALSE;
        }

		// ��½ð� ǥ��
		CTime currentTime = CTime::GetCurrentTime();
		
		int nYYYY = currentTime.GetYear();
		int nMM = currentTime.GetMonth();
		int nDD = currentTime.GetDay();
		int nm1 = currentTime.GetHour();
		int nm2 = currentTime.GetMinute();
		int nm3 = currentTime.GetSecond();
		CString szCurrentTime;
		szCurrentTime.Format("[��½ð�] %04d.%02d.%02d %02d:%02d:%02d",nYYYY, nMM, nDD, nm1,nm2,nm3);
		CSize size;
		::GetTextExtentPoint32(hDC, szCurrentTime, szCurrentTime.GetLength(), &size );
		::TextOut(hDC,rect.left,rect.bottom+50+size.cy,(LPCTSTR)szCurrentTime, szCurrentTime.GetLength());

		CString szPublisher;
		szPublisher.Format("[������] %s (%s)",m_strName,fnGetIpAddress());
		::GetTextExtentPoint32(hDC, szPublisher, szPublisher.GetLength(), &size );
		::TextOut(hDC,rect.right-size.cx,rect.bottom+50+size.cy,(LPCTSTR)szPublisher, szPublisher.GetLength());
		


    }
    return TRUE;
}

//===========================================================================//
// �Լ���  : PaletteSize                                                     //
// �Լ����: DIB�� Į����� RGBQUAD(for a Windows 3.0-style DIB) �Ǵ�        //
//           RGBTRIPLE(for an OS/2-style DIB)                                //                                                          //
// �Ķ��Ÿ: LPSTR lpbi : DIB �޸� ����Ʈ                                  //
// ���ϰ�  : WORD :DIB �ķ�Ʈ Į�� ũ��                                      //
//===========================================================================//
WORD CWinPrint::PaletteSize (LPSTR lpbi)
{
    if (IS_WIN30_DIB (lpbi))
        return (DIBNumColors (lpbi) * sizeof (RGBQUAD));
    else
        return (DIBNumColors (lpbi) * sizeof (RGBTRIPLE));
}

//===========================================================================//
// �Լ���  : DestroyDIB                                                      //
// �Լ����: hDIB �޸𸮸� �����.                                           //
// �Ķ��Ÿ: HDIB hDib : ���� hDIB                                           //
// ���ϰ�  : void                                                            //
//===========================================================================//
void CWinPrint::DestroyDIB(HDIB hDib)
{
    GlobalFree(hDib);
}

//===========================================================================//
// �Լ���  : GetPrinterDC                                                    //
// �Լ����: ����Ʈ DC�� ��´�.                                             //
// �Ķ��Ÿ: void                                                            //
// ���ϰ�  : HDC : ����Ʈ DC                                                 //
//===========================================================================//
HDC CWinPrint::GetPrinterDC()
{
    HDC         hPrinterDC;
    LPDEVMODE   lpDevMode = NULL;
    LPDEVNAMES  lpDevNames;

    static char szPrinter[64];
    char *szDevice, *szDriver, *szOutput;

    // Print Dialog box�� ������ �־����� Ȯ��  
    if (!pd.hDC) 
	{
        GetProfileString("windows", "device", "", szPrinter, 64);
        if ((szDevice = strtok(szPrinter, ",")) && (szDriver = strtok(NULL, ","))
            && (szOutput = strtok(NULL, ", "))) 
		{
            lstrcpy((LPSTR)gszDevice, (LPSTR)szDevice);    // Copy to global variables
            lstrcpy((LPSTR)gszOutput, (LPSTR)szOutput);
            return CreateDC(szDriver, szDevice, szOutput, NULL);
        }   
        return NULL;
    }

    // Print Dialog box���� ����Ʈ DC�� ã�´�.
    lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
    lstrcpy((LPSTR)gszDriver, (LPSTR)lpDevNames + lpDevNames->wDriverOffset);
    lstrcpy((LPSTR)gszDevice, (LPSTR)lpDevNames + lpDevNames->wDeviceOffset);
    lstrcpy((LPSTR)gszOutput, (LPSTR)lpDevNames + lpDevNames->wOutputOffset);


    if(pd.hDC) {
        hPrinterDC = pd.hDC;
        GlobalUnlock(pd.hDevNames);
        return hPrinterDC;
    }

    if (pd.hDevMode)
        lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);

    hPrinterDC = CreateDC(gszDriver, gszDevice, gszOutput, lpDevMode);
    GlobalUnlock(pd.hDevNames);
    GlobalFree(pd.hDevNames);
    return hPrinterDC;
}

//===========================================================================//
// �Լ���  : DIBNumColors                                                    //
// �Լ����: DIB�� ���� ���� ����                                            //
// �Ķ��Ÿ: LPSTR lpbi : DIB �޸��� �ּ�                                  //
// ���ϰ�  : WORD       : ���� ��                                            //
//===========================================================================//
WORD CWinPrint::DIBNumColors (LPSTR lpbi)
{
    WORD wBitCount;

    if (IS_WIN30_DIB (lpbi)) {
        DWORD dwClrUsed;

        dwClrUsed = ((LPBITMAPINFOHEADER) lpbi)->biClrUsed;

        if (dwClrUsed)
            return (WORD) dwClrUsed;
    }


    // DIB�� �ȼ��� ��Ʈ������ �÷� ���� ��� 
    if (IS_WIN30_DIB (lpbi))
        wBitCount = ((LPBITMAPINFOHEADER) lpbi)->biBitCount;
    else
        wBitCount = ((LPBITMAPCOREHEADER) lpbi)->bcBitCount;

    switch (wBitCount) {
        case 1:
            return 2;

        case 4:
            return 16;

        case 8:
            return 256;

        default:
            return 0;
    }
}

//===========================================================================//
// �Լ���  : PrintAbortDlg                                                   //
// �Լ����: ����Ʈ��  ����Ѵ�.                                             //
// �Ķ��Ÿ: HWND hWnd :                                                     //
// ���ϰ�  : HDC : ����Ʈ DC                                                 //
//===========================================================================//
BOOL CWinPrint::PrintAbortDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            char szBuffer[100];
            // ��Ʈ���� ��� ���¸� ��Ÿ����.
            // (EX "on HP LaserJet on LPT1")
            wsprintf(szBuffer, "on %s on %s", (LPSTR)(LPCTSTR)gszDevice, (LPSTR)(LPCTSTR)gszOutput);
            SetDlgItemText(hWnd, IDC_PRINTTEXT2, (LPSTR)szBuffer);
            SetFocus(GetDlgItem(hWnd, IDCANCEL));
//            fnMoveCenter(ghWndFrame, hWnd, 0);
        }
        return TRUE;     // Return TRUE is SetFocus() ��»��¿� ���� ��ȯ 

        case WM_COMMAND:
            bAbort = TRUE;
            DestroyWindow(hWnd);
            return TRUE;
        break;
        }
    return FALSE;
}

//===========================================================================//
// �Լ���  : PrintAbortProc                                                  //
// �Լ����: ������ ���¿��� �޼��� ������ ����.                             //
// �Ķ��Ÿ: HDC hDC : Dialog DC                                             //
// ���ϰ�  : BOOL ������ ����� ���                                         //
//===========================================================================//
BOOL CWinPrint::PrintAbortProc(HDC hDC, int code)
{
    MSG msg;

    while (!bAbort && PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        if (!IsDialogMessage(hDlgAbort, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    return (!bAbort);
}

//===========================================================================//
// �Լ���  : PrintDlgProc                                                    //
// �Լ����: ����Ʈ �ɼ� ����                                                //
// �Ķ��Ÿ: HWND hWndDlg : ����Ʈ ���̾˷α� �ɼ�                           //
// ���ϰ�  : BOOL :                                                          //
//===========================================================================//
BOOL CWinPrint::PrintDlgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam) 
{
    static LPOPTIONSTRUCT lpOS;                   
    DWORD FlagSave;

    switch (Message) {
        case WM_INITDIALOG: {
            // DialogBoxParam()���� dialog box�� ����Ͽ� 
            // lParam�� OPTIONSTRUCT�� �����Ѵ�.
            lpOS = (LPOPTIONSTRUCT)lParam;

            // ���� �˻� 
            //CheckDlgButton(hWndDlg, IDC_CHECK_CONTINUE, TRUE);
            bPrintContinue = FALSE;
            
            // fill in non-variant fields of PRINTDLG struct.
            pd.lStructSize          = sizeof (PRINTDLG) ;
            pd.hwndOwner            = ghWndFrame;
            pd.hDevMode             = NULL ;
            pd.hDevNames            = NULL ;
            pd.hDC                  = NULL ;
            pd.Flags                = PD_RETURNDC | PD_NOSELECTION | PD_NOPAGENUMS;
            pd.nFromPage            = 0 ;
            pd.nToPage              = 0 ;
            pd.nMinPage             = 0 ;
            pd.nMaxPage             = 0 ;
            pd.nCopies              = 1 ;
            pd.hInstance            = NULL ;
            pd.lCustData            = 0L ;
            pd.lpfnPrintHook        = NULL ;
            pd.lpfnSetupHook        = NULL ;
            pd.lpPrintTemplateName  = NULL ;
            pd.lpSetupTemplateName  = NULL ;
            pd.hPrintTemplate       = NULL ;
            pd.hSetupTemplate       = NULL ;
        }
//        fnMoveCenter(ghWndFrame, hWndDlg, 0);
        break; // End of WM_INITDIALOG                                 

    case WM_CLOSE:
        PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
    break;

    case WM_COMMAND:
        switch (wParam) {
                                             
        case IDOPTION: {
            FlagSave = pd.Flags;
            pd.Flags |= PD_PRINTSETUP;    // Print Setup option 
            PrintDlg((LPPRINTDLG)&pd);
            pd.Flags = FlagSave;          // Remove option 
            
        }           
        break;

        case IDOK: {
            // OPTIONSTRUCT�� ������ ���� ���� 
            if (!lpOS) {
                EndDialog(hWndDlg, FALSE);
                break;
            }

            lpOS->iOption = IDC_BESTFIT;
            // ���� ����Ʈ �˻� 
            if( IsDlgButtonChecked(hWndDlg, IDC_CHECK_CONTINUE) )
                bPrintContinue = TRUE;
            else 
                bPrintContinue = FALSE;
            
            EndDialog(hWndDlg, TRUE);
        }
        break;

        case IDCANCEL:
            EndDialog(hWndDlg, FALSE);
        break;

        }
    break;    // End of WM_COMMAND                                 

    default:
        return FALSE;
    }
    return TRUE;
}
/*
//===========================================================================//
// �Լ���  : PrintBOGOSE                                                     //
// �Լ����: ĸ�ĸ� ���� ���                                                //
// �Ķ��Ÿ: HWND hFrameWnd      : ���� ������ �ڵ�                          //
//           HWND hWnd           : DLL�� �θ��� �������� �ڵ�                //
// ���ϰ�  : BOOL                                                            //
//===========================================================================//
BOOL FAR PASCAL _export fnPrintBOGOSE(HWND hFrameWnd, HWND hWnd)
{
    HGLOBAL     hChild;
    LPCHILD     lpChild;
    HDC         hPrnDC;                  // ����Ʈ DC
    HFONT       hPrintFont;
    static      OPTIONSTRUCT opts;
    FARPROC     lpMsgProc;      
    char        szJobNameTrunc[35];   
    char        szBuffer[50];           // Caption title
    int         xTemp, yTemp, xCharSpace, yCharSpace;
    int         nReturn;
    
    ghWndFrame = hFrameWnd;
    // ���õ� �������� Ÿ��Ʋ�� ��´�.
    gnWindowsTextSize = GetWindowText(hWnd, lpWindowTitle, 100);

    // ����Ʈ �ɼ� ���� dialog box(ex, bestfit, stretchtopage, scale)
    opts.iOption = 1;
    lpMsgProc = MakeProcInstance((FARPROC)PrintDlgProc, ghDLLInst);
    nReturn = DialogBoxParam(ghDLLInst, (LPSTR)"Print",
                            ghWndFrame, lpMsgProc, (LONG)(LPSTR)&opts);
    FreeProcInstance(lpMsgProc);
    if(!nReturn)
        return FALSE;
    // Print DC get
    hPrnDC = GetPrinterDC();
    if(!hPrnDC)
        return FALSE;               
    
    // ������ ���̾˷α� ���� 
    hDlgAbort = CreateDialog(ghDLLInst, szPrintDlg, GetFocus(), PrintAbortDlg);
    // ������ ���¸� ��Ÿ����.
    lstrcpyn(szJobNameTrunc, lpWindowTitle, 31);
    szJobNameTrunc[31] = '\0';           // Truncate string to 31 chars
    wsprintf(szBuffer, "�μ��� '%s'", (LPSTR)szJobNameTrunc);
    SetDlgItemText(hDlgAbort, IDC_PRINTTEXT1, (LPSTR)szBuffer);
    SetDlgItemText(hDlgAbort, IDC_PERCENTAGE, (LPSTR)"���� �μ� ��...");
    bAbort = FALSE;
*/
/*
    // ����Ʈ ����̺� ������ ��´�.
    cxPage = GetDeviceCaps(hPrnDC, HORZRES);     // Width of printr page - pixels
    cyPage = GetDeviceCaps(hPrnDC, VERTRES);     // Height of printr page - pixels
    
    if(cxPage < cyPage) {
        xTemp = cxPage/332;
        yTemp = xTemp;
    }
    else {
        yTemp = cyPage/240;
        xTemp = yTemp;
    }
        
    SetMapMode(hPrnDC, MM_ISOTROPIC);
    SetWindowExt(hPrnDC, ((DEFAULT_CHAR_COUNT+1)*xTemp), 
                          ((DEFAULT_CHAR_LINE+1)*(yTemp-2+3)));
    SetViewportExt(hPrnDC, cxPage, cyPage);
*/
/*      
    // ABOUT PROC�� ���� 
    if (Escape(hPrnDC, SETABORTPROC, NULL, (LPSTR)PrintAbortProc, NULL) < 0)
        return FALSE;
    // Escape()�� STARTDOC�� ������ ���� 
    if (Escape(hPrnDC, STARTDOC, lstrlen((LPSTR)szJobNameTrunc), (LPSTR)
             szJobNameTrunc, NULL) < 0) {
        DestroyWindow(hDlgAbort); 
        DeleteDC(hPrnDC);
        return FALSE;
    }
    // Drowing Map 
    hChild = GetWindowWord(hWnd, WW_OFFSET_CHILD);
    lpChild = (LPCHILD)GlobalLock(hChild);
    xCharSpace = lpChild->xChar;
    yCharSpace = lpChild->yChar;
    lpChild->xChar = xTemp;
    lpChild->yChar = yTemp*2;
    
    // ��Ʈ ũ�� ������ 
    hPrintFont = fnNewFont((int)(xTemp*1.5), xTemp,FW_NORMAL, lpChild->szFontName);
    SelectObject(hPrnDC, hPrintFont);

    // map drowing
    if(lpChild->hMapData)
        fnDrawMap(hWnd, hPrnDC, lpChild, 0);
        
    lpChild->xChar = xCharSpace;
    lpChild->yChar = yCharSpace;
    GlobalUnlock(hChild);
    
    // Non-banding devices need a NEWFRAME 
    if (Escape(hPrnDC, NEWFRAME, NULL, NULL, NULL) < 0) {
        DeleteDC(hPrnDC);
        return FALSE;
    }
    if (Escape(hPrnDC, ENDDOC, NULL, NULL, NULL) < 0) {
        DeleteDC(hPrnDC);
        return FALSE;
    }
        
    if (!bAbort)
        DestroyWindow(hDlgAbort);
    DeleteDC(hPrnDC);

    return TRUE;
}
*/

//===========================================================================//
// �Լ���  : fnPrintDelete()                                                 //
// �Լ����: ���� ����� ����Ʈ DC�� �������� ���                           //
// �Ķ��Ÿ: NULL                                                            //
// ���ϰ�  : VOID                                                            //
//===========================================================================//
void CWinPrint::fnPrintDelete()
{
    // ���� ����� ����Ʈ DC�� �������� ��� 
    if(hPrnDC)
        DeleteDC(hPrnDC);
}
//===========================================================================//
// WINPRINT.C(����Ʈ DLL)   END                                              //
//===========================================================================//

//�̹����� ȭ�Ϸ� �����ϴ� �κ� 
BOOL CWinPrint::SaveDIB(HANDLE hDib, LPSTR lpFileName)
{
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure
    HANDLE              fh;         // file handle for opened file
    DWORD               dwDIBSize;
    DWORD               dwWritten;

    if (!hDib)
        return FALSE;

    fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    // Get a pointer to the DIB memory, the first of which contains
    // a BITMAPINFO structure

    lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (!lpBI)
    {
        CloseHandle(fh);
        return FALSE;
    }

    // Check to see if we're dealing with an OS/2 DIB.  If so, don't
    // save it because our functions aren't written to deal with these
    // DIBs.

    if (lpBI->biSize != sizeof(BITMAPINFOHEADER))
    {
        GlobalUnlock(hDib);
        CloseHandle(fh);
        return FALSE;
    }

    // Fill in the fields of the file header

    // Fill in file type (first 2 bytes must be "BM" for a bitmap)

    bmfHdr.bfType = 0x4d42;  // "BM"

    // Calculating the size of the DIB is a bit tricky (if we want to
    // do it right).  The easiest way to do this is to call GlobalSize()
    // on our global handle, but since the size of our global memory may have
    // been padded a few bytes, we may end up writing out a few too
    // many bytes to the file (which may cause problems with some apps,
    // like HC 3.0).
    //
    // So, instead let's calculate the size manually.
    //
    // To do this, find size of header plus size of color table.  Since the
    // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
    // the size of the structure, let's use this.

    // Partial Calculation

    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  

    // Now calculate the size of the image

    // It's an RLE bitmap, we can't calculate size, so trust the biSizeImage
    // field

    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
        dwDIBSize += lpBI->biSizeImage;
    else
    {
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only

        // It's not RLE, so size is Width (DWORD aligned) * Height

        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) *
                lpBI->biHeight;

        dwDIBSize += dwBmBitsSize;

        // Now, since we have calculated the correct size, why don't we
        // fill in the biSizeImage field (this will fix any .BMP files which 
        // have this field incorrect).

        lpBI->biSizeImage = dwBmBitsSize;
    }


    // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
                   
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;

    // Now, calculate the offset the actual bitmap bits will be in
    // the file -- It's the Bitmap file header plus the DIB header,
    // plus the size of the color table.
    
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize +
            PaletteSize((LPSTR)lpBI);

    // Write the file header

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    // Write the DIB header and the bits -- use local version of
    // MyWrite, so we can write more than 32767 bytes of data
    
    WriteFile(fh, (LPSTR)lpBI, dwDIBSize, &dwWritten, NULL);

    GlobalUnlock(hDib);
    CloseHandle(fh);

    if (dwWritten == 0)
        return FALSE; // oops, something happened in the write
    else
        return 0; // Success code

	return true;
}

CString CWinPrint::fnGetIpAddress()
{
	WORD wVersionRequested;
	char  dst[32];
	char  host[128];
	struct hostent *pent;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD( 2, 0 );

	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{

		gethostname(host, 127);
		pent = gethostbyname(host);
		if( pent == NULL ) {
			memcpy(dst, "000.000.000.000", 15);
		} else {
			int  i;
			unsigned char ip[4];
			memcpy(dst, "000.000.000.000", 15);
			for(i=0 ; ; i++) {
				if( pent->h_addr_list[i] == NULL )
					break;
				memcpy(ip, pent->h_addr_list[i], 4);
				sprintf(dst, "%u.%u.%u.%u",
						ip[0], ip[1], ip[2], ip[3]
						);
				if( memcmp(dst, "127.000.000.001", 15) != 0 )
					break;
			}
		}
		WSACleanup( );
	}
	return CString(dst);
}

BOOL CWinPrint::SaveImg( const char *pszFilename, HDIB hDIB)
{
	int nType = ExtensionIndex( pszFilename );

	if( nType < IMAGETYPE_FIRSTTYPE || nType > IMAGETYPE_LASTTYPE ) return( FALSE );
	

	switch( nType )
	{
		case IMAGETYPE_BMP:
			return( ::SaveBMP( pszFilename, hDIB ) );
			break;
		case IMAGETYPE_JPG:
			{
				int nQ = 120;
				return( ::SaveJPG( pszFilename, hDIB, nQ ) );
			}			
			break;
		case IMAGETYPE_PCX:
			return( ::SavePCX( pszFilename, hDIB ) );
			break;
	}

	return( TRUE );

}

int CWinPrint::ExtensionIndex( const char *pszFilename )
{
	int Index = 0;
	char *pszExtension;

	pszExtension = (char *) &pszFilename[strlen(pszFilename)-4];

	while( szExtensions[Index][0] )
	{
		if( !stricmp( pszExtension, szExtensions[Index] ) ) return( Index + 1 );
		Index++;
	}

	return -1;
}

HDIB CWinPrint::AddBMPFileHeader(HDIB hDIB)
{
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure
    DWORD               dwDIBSize;
	HDIB				hNewDIB;

    if (!hDIB)
        return NULL;

	lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    if (!lpBI)
        return NULL;
    
	if (lpBI->biSize != sizeof(BITMAPINFOHEADER))
	{
		GlobalUnlock(hDIB);
		return NULL;
	}

	// Partial Calculation
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  

    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
        dwDIBSize += lpBI->biSizeImage;
    else
    {
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only

        // It's not RLE, so size is Width (DWORD aligned) * Height
        dwBmBitsSize= WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
        dwDIBSize	+= dwBmBitsSize;

        // Now, since we have calculated the correct size, why don't we
        // fill in the biSizeImage field (this will fix any .BMP files which 
        // have this field incorrect).
        lpBI->biSizeImage = dwBmBitsSize;
    }


	bmfHdr.bfType		= 0x4d42;  // "BM"
    bmfHdr.bfSize		= dwDIBSize + sizeof(BITMAPFILEHEADER);
    bmfHdr.bfReserved1	= 0;
    bmfHdr.bfReserved2	= 0;
    bmfHdr.bfOffBits	= (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize +
            PaletteSize((LPSTR)lpBI);	


    hNewDIB				= GlobalAlloc (GHND, bmfHdr.bfSize);
	if(hNewDIB)
	{
		BYTE *	pNewDIB	= (BYTE *)GlobalLock(hNewDIB);
		if(pNewDIB)
		{
			BITMAPFILEHEADER *pBmpFile	= (BITMAPFILEHEADER *)pNewDIB;
			*pBmpFile					= bmfHdr;
			LPBITMAPINFOHEADER pBmpInfo = (LPBITMAPINFOHEADER)(pNewDIB + sizeof(BITMAPFILEHEADER));
			memcpy(pBmpInfo, lpBI, dwDIBSize);

			GlobalUnlock(hNewDIB);
		}
		else
		{
			GlobalFree(hNewDIB);
			hNewDIB = NULL;
		}
	}

	GlobalUnlock(hDIB);
	return hNewDIB;
}