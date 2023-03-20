// MyBitmap.cpp: implementation of the CAxBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AxBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxBitmap::CAxBitmap()
{
	m_nWidth = 0;
	m_nHeight = 0;
}

CAxBitmap::~CAxBitmap()
{
	DeleteObject();
}

BOOL CAxBitmap::LoadBitmap(UINT nIDResource)
{
	BOOL iResult = CBitmap::LoadBitmap(nIDResource);
	
	BITMAP bmp;
	
	GetBitmap(&bmp);
	
	if(iResult)
	{
		m_nHeight = bmp.bmHeight;
		m_nWidth = bmp.bmWidth;
	}

	return iResult;
}

void CAxBitmap::LoadInit()
{
	BITMAP bmp;
	
	GetBitmap(&bmp);
	
	m_nHeight = bmp.bmHeight;
	m_nWidth = bmp.bmWidth;
}


BOOL CAxBitmap::LoadBitmap(CString strFilename)						// ���Ͽ��� �ҷ��� 
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strFilename, IMAGE_BITMAP,
					       0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	BITMAP bmp;
	if (!hBitmap) {
		CString ss;
		ss.Format("%s ������ �������� �ʽ��ϴ�.", strFilename);
		AfxMessageBox(ss);
		m_hObject = NULL;
		return FALSE;
	}
		if(GetSafeHandle())
		{
			DeleteObject();
			Detach();
		}

		Attach(hBitmap);

		this->GetBitmap(&bmp);

		m_nHeight = bmp.bmHeight;
		m_nWidth = bmp.bmWidth;

		return TRUE;
}

void CAxBitmap::DrawPosition(CDC *pDC, int x, int y, int width, int height)
{
	CDC MemDC;
	CBitmap	*poldbitmap;

	if (m_hObject == NULL)
		return;
		
		MemDC.CreateCompatibleDC(pDC);

		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->BitBlt(x, y, width, height, &MemDC, x, y, SRCCOPY);

		MemDC.SelectObject(poldbitmap);
		
		DeleteDC(MemDC);
}

void CAxBitmap::Draw(CDC *pDC, int x, int y)
{
	CDC MemDC;
	CBitmap	*poldbitmap;

	if (m_hObject == NULL)
		return;

		MemDC.CreateCompatibleDC(pDC);

		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->BitBlt(x, y, m_nWidth, m_nHeight, &MemDC, 0, 0, SRCCOPY);

		MemDC.SelectObject(poldbitmap);
		
		DeleteDC(MemDC);
}

void CAxBitmap::Draw(CDC *pDC, int x, int y, int w, int h, POINT p, DWORD dwRop)
{
	Draw(pDC, x, y, w, h, p.x, p.y, dwRop);
}	

void CAxBitmap::Draw(CDC *pDC, int x, int y, int w, int h, int srcX, int srcY, DWORD dwRop)
{
	CDC MemDC;
	CBitmap *poldbitmap;
	if (m_hObject == NULL)
		return;


		MemDC.CreateCompatibleDC(pDC);
		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->BitBlt(x, y, w, h, &MemDC, srcX, srcY, dwRop);
		
		MemDC.SelectObject(poldbitmap);
		DeleteDC(MemDC);
	
}

void CAxBitmap::Draw(CDC *pDC, int x, int y, int srcX, int srcY, int esrcX, int esrcY)
{
	CDC MemDC;
	CBitmap *poldbitmap;
	if (m_hObject == NULL)
		return;

		MemDC.CreateCompatibleDC(pDC);

		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->StretchBlt(x, y, 20, 20, &MemDC,
			srcX, srcY, esrcX, esrcY, SRCCOPY);
		
		MemDC.SelectObject(poldbitmap);
		DeleteDC(MemDC);		
}

void CAxBitmap::Draw(CDC *pDC, RECT dest)
{
	CDC MemDC;
	CBitmap *poldbitmap;
	if (m_hObject == NULL)
		return;

		
		MemDC.CreateCompatibleDC(pDC);

		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->StretchBlt(dest.left, dest.top, dest.right - dest.left, dest.bottom - dest.top, &MemDC,
			0, 0, m_nWidth, m_nHeight, SRCCOPY);
		
		MemDC.SelectObject(poldbitmap);
		MemDC.DeleteDC();
}


#define SRCMASK   0x00220326    // mask

// Function name	: CAxBitmap::DrawBitmapByMask
// Description		: ����ũ �̹��� - �������� ���� �̹��� ó�� 
// Return type		: void 
// Argument		: CDC *pDC
// Argument		: CPoint targetPos - �׷��� ��ġ 
// Argument		: CPoint srcPos - �̹������� ��ǥ 
// Argument		: COLORREF maskcolor
// Argument		: CRect bmpRc - �׸� ũ�� (NULL�̸� �ڵ� �Է�)
void CAxBitmap::DrawBitmapByMask(CDC *pDC, CPoint targetPos, CPoint srcPos, COLORREF maskcolor, CRect bmpRc)
{
//	 BITMAP  bm;
	 if (bmpRc.Width() == 0 && bmpRc.Height() == 0)
	 {
//		  pBitmap->GetBitmap(&bm);
		  bmpRc.left = bmpRc.top = 0;
		  bmpRc.right = m_nWidth;	//bm.bmWidth;
		  bmpRc.bottom = m_nHeight;	//bm.bmHeight;
	 }

	if (m_hObject == NULL)
		return;
 
	 CDC  buffDC, maskDC, memoryDC, copyDC;
	 CBitmap  buffBitmap, maskBitmap, copyBitmap;
	 CBitmap  *oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;

	 buffDC.CreateCompatibleDC(pDC);
	 buffBitmap.CreateCompatibleBitmap(pDC, bmpRc.Width(), bmpRc.Height());
	 oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	 maskDC.CreateCompatibleDC(pDC);
	 maskBitmap.CreateBitmap(bmpRc.Width(), bmpRc.Height(), 1, 1, NULL);
	 oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	 CRect  maskRc( 0, 0, bmpRc.Width(), bmpRc.Height());
	 buffDC.FillSolidRect(&maskRc, maskcolor);

	 memoryDC.CreateCompatibleDC(pDC);
	 oldmemoryBitmap = memoryDC.SelectObject(this);
	 buffDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &memoryDC, srcPos.x, srcPos.y, SRCCOPY);
	 maskDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &buffDC, 0, 0, SRCCOPY);

	 copyDC.CreateCompatibleDC(pDC);
	 copyBitmap.CreateCompatibleBitmap(pDC, bmpRc.Width(), bmpRc.Height());
	 oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	 copyDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), pDC,  targetPos.x, targetPos.y, SRCCOPY);
	 copyDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &maskDC, 0, 0, SRCAND);
	 buffDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &maskDC, 0, 0, SRCMASK);
	 copyDC.BitBlt( 0, 0, bmpRc.Width(), bmpRc.Height(), &buffDC, 0, 0, SRCPAINT);
	 pDC->BitBlt(targetPos.x, targetPos.y, bmpRc.Width(), bmpRc.Height(), &copyDC, 0, 0, SRCCOPY);

	 copyDC.SelectObject(oldcopyBitmap);
	 memoryDC.SelectObject(oldmemoryBitmap);
	 maskDC.SelectObject(oldmaskBitmap);
	 buffDC.SelectObject(oldbuffBitmap); 
	 buffDC.DeleteDC();
	 maskDC.DeleteDC();
	 memoryDC.DeleteDC();
	 copyDC.DeleteDC();
} 


// Function name	: CAxBitmap::DrawBitmapByMask
// Description		: ���� �̹��� ó��
// Return type		: void 
// Argument		: CDC *pDC	
// Argument		: CPoint targetPos - �׷��� ��ǥ 
// Argument		: COLORREF maskcolor 
// Argument		: CRect bmpRc      - �׸�ũ�� (Null �̸� ũ���ڵ��Է�)
void CAxBitmap::DrawBitmapByMask(CDC *pDC,  CPoint targetPos, COLORREF maskcolor, CRect bmpRc)
{
	DrawBitmapByMask(pDC, targetPos, CPoint(0, 0), maskcolor, bmpRc);
}










