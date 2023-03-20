#if !defined(AFX_TRMAINVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
#define AFX_TRMAINVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrMainView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrMainView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"
#include "../../../Controls/GDIMgr\ColorMap.h"
#include "..\..\TrDll\CtrlWrap\raprunform.h"
#include "../../Contents/inc/PcTrMngHelper.h"	//Pctr Manager

class CMainFrmWnd;
class CTrMainView : public CFormView
{
protected:
	CTrMainView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTrMainView)

public:
	CPCTrMngHelper pctrHelper;
	HWND			m_hFormMng;
	long			m_lBkColor;
	COLORREF	m_crBackColor;			// ȭ�� ��׶���

	int             m_nInitFlag;
	CMapStringToOb	*m_MapControlName;		// ȭ�鿡 ������ Control���� �����ϴ� Map

	IDispatch *m_pFormDisp;
	IRunFormCtl m_lRunForm;

public :
	CMainFrmWnd *m_pwndMainFrm;

	void OnCreate_MainWnd();
	void SetInit_MainWnd();

	void SetCloseScreen();
  

// Form Data
public:
	//{{AFX_DATA(CTrMainView)
	enum { IDD = IDD_STD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrMainView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTrMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTrMainView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	
	//}}AFX_MSG
	afx_msg COLORREF OnGetSkinColor(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnEditEnter();
	afx_msg void OnFormInitAfter();
	afx_msg void OnFormGetPageSave(LPCTSTR strFileName);	//���� ����ȭ��
	afx_msg void OnFormSetPageSave(LPCTSTR strFileName);	//���� ����ȭ��
	afx_msg void OnColorChange();	//������ ��Ų����� �Ҹ���.
	afx_msg long OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam);

	DECLARE_EVENTSINK_MAP()

	// ����¡ó������ �߰�
	CWnd* GetFrameWnd(CWnd* pWnd);
	void DoDllSizeEx(HWND _hChildFrame=NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR1111VIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
