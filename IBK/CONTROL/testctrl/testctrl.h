// testctrl.h : main header file for the TESTCTRL DLL
//

#if !defined(AFX_TESTCTRL_H__B3F890C2_F3A6_4A75_AA92_8D73A3ED3D92__INCLUDED_)
#define AFX_TESTCTRL_H__B3F890C2_F3A6_4A75_AA92_8D73A3ED3D92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#define UNUSED_VALUE	-99999999.999
/////////////////////////////////////////////////////////////////////////////
// CTestctrlApp
// See testctrl.cpp for the implementation of this class
//

class CTestctrlApp : public CWinApp
{
public:
	CTestctrlApp();
	~CTestctrlApp();

	char* GetSampleData(int pInfo1, int pInfo2, int* pLen);
	void	SetView(CWnd* pView) { m_pView = pView; }
	bool	CreateImage();
	CImageList* GetCtrlImage() { return m_pImgCtrl.get(); }
	std::shared_ptr<CImageList> getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);
	CFont* GetFont(CWnd* pView, int point, CString name, int bold = 0, bool italic = false);
	CPen* GetPen(CWnd* pView, int style, int width, COLORREF clr);
	CBrush* GetBrush(CWnd* pView, COLORREF rColor);
	COLORREF GetColor(CWnd* pView, int index);

	HINSTANCE m_hDlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestctrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTestctrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pView;
	std::shared_ptr<CImageList>	m_pImgCtrl;
public:
	virtual int ExitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCTRL_H__B3F890C2_F3A6_4A75_AA92_8D73A3ED3D92__INCLUDED_)
