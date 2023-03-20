#if !defined(AFX_CERTIFYPPG_H__D6D0A896_DDF8_4EE6_BDA0_F5649960E55D__INCLUDED_)
#define AFX_CERTIFYPPG_H__D6D0A896_DDF8_4EE6_BDA0_F5649960E55D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CertifyPpg.h : Declaration of the CCertifyPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCertifyPropPage : See CertifyPpg.cpp.cpp for implementation.

class CCertifyPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCertifyPropPage)
	DECLARE_OLECREATE_EX(CCertifyPropPage)

// Constructor
public:
	CCertifyPropPage();

// Dialog Data
	//{{AFX_DATA(CCertifyPropPage)
	enum { IDD = IDD_PROPPAGE_CERTIFY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCertifyPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CERTIFYPPG_H__D6D0A896_DDF8_4EE6_BDA0_F5649960E55D__INCLUDED)
