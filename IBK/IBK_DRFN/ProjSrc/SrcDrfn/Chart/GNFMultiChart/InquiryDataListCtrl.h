#if !defined(AFX_INQUIRYDATALISTCTRL_H__4363764A_68ED_4311_8B84_D24D46C82843__INCLUDED_)
#define AFX_INQUIRYDATALISTCTRL_H__4363764A_68ED_4311_8B84_D24D46C82843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InquiryDataListCtrl.h : header file
// ��ġ��ȸâ���� ���Ǵ� ����Ʈ��Ʈ�ѷν� ��ġ��ȸâ(Window, Simple Window)Ÿ���̰�
// '�ڵ�'�̵����� �� ��쿡 ���콺�����Ͱ� ��ġ��ȸâ�� ��ġ�ϸ� WM_MOUSEMOVE�޼�����
// ��ġ��ȸâ�� ����Ʈ��Ʈ�ѿ��� �߻��Ѵ�. �̷��� �߻��� �޼����� �ٽ� 
// CToolBarManagerImplementationŬ�������� ó���ϵ��� �Ѵ�. 
// PS. ��~ ������. ������.

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataListCtrl window

class CInquiryDataListCtrl : public CListCtrl
{
// Construction
public:
	CInquiryDataListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInquiryDataListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInquiryDataListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInquiryDataListCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2006/2/15 - Seung-Won, Bae) Manage Origianl Point of Drag
protected:
	CPoint	m_ptStart;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INQUIRYDATALISTCTRL_H__4363764A_68ED_4311_8B84_D24D46C82843__INCLUDED_)
