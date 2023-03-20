// CfxTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CfxTab.h"
#include "../../h/memdc.h"
#include "misctype.h"
#include "../../h/Axisvar.h"
#include "../../h/AxisFire.h"
#include <windows.h>

// CfxTab


#define IDC_TAB_SETUP	8000
#define IDC_TAB_OVER	8001
#define TAB_GAP		3
#define	AL_TOP		1
#define	AL_LEFT		2
#define	AL_RIGHT	3
#define	AL_BOTTOM	4
#define	clBOTTOM	RGB(100, 100, 100)
#define	clWHITE		RGB(255, 255, 255)
#define PALETTEFILE	"PALETTE.INI"
#define IMGSEL_L	"L_S"
#define	IMGSEL_M	"S"
#define	IMGSEL_R	"R_S"
#define IMG_L		"L_"
#define	IMG_M		""
#define	IMG_R		"R_"
#define	IMGSPACE_L	"L_B"
#define	IMGSPACE_M	"B"
#define	IMGSPACE_R	"R_B"
#define IMG_FILE	"TAB.BMP"
#define COLOR_SPIN	7
#define MASK_COLOR	RGB(255, 0, 255)

const int bmp_selL = 0, bmp_selM = 1, bmp_selR = 2, bmp_norL = 3, bmp_norM = 4, bmp_norR = 5, bmp_spL = 6, bmp_spM = 7, bmp_spR = 8;

#define SEL_SEP		"|"
#define SEL_TAP		"\t"
#define MAX(a,b)	(a>=b ? a : b)

const int nBase = 10000;
const int nMoveX = 3;
const int nMoveY = 2;
#define X_SIZE		8

CfxTab::CfxTab()
{

}

CfxTab::~CfxTab()
{
}


BEGIN_MESSAGE_MAP(CfxTab, CTabCtrl)
	ON_WM_DRAWITEM()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnSelchange)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_TAB_SETUP, OnSetup)
END_MESSAGE_MAP()

void CfxTab::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	return;
}

// CfxTab 메시지 처리기입니다.




int CfxTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CfxTab::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CTabCtrl::OnPaint()을(를) 호출하지 마십시오.
}


BOOL CfxTab::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CTabCtrl::OnEraseBkgnd(pDC);
}


void CfxTab::OnDestroy()
{
	CTabCtrl::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CfxTab::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CTabCtrl::OnLButtonDown(nFlags, point);
}


void CfxTab::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CfxTab::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CTabCtrl::OnMouseMove(nFlags, point);
}

BOOL CfxTab::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{

	return FALSE;
}

void CfxTab::OnSetup()
{

}