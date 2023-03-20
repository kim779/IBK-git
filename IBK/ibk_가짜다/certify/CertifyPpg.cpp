// CertifyPpg.cpp : Implementation of the CCertifyPropPage property page class.

#include "stdafx.h"
#include "Certify.h"
#include "CertifyPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCertifyPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCertifyPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCertifyPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCertifyPropPage, "AxisCertify.CertifyPropPage.IBK",
	0x118202b2, 0x9719, 0x4600, 0xbc, 0xf7, 0x57, 0xe8, 0x2e, 0x83, 0xe2, 0x9f)


/////////////////////////////////////////////////////////////////////////////
// CCertifyPropPage::CCertifyPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCertifyPropPage

BOOL CCertifyPropPage::CCertifyPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CERTIFY_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyPropPage::CCertifyPropPage - Constructor

CCertifyPropPage::CCertifyPropPage() :
	COlePropertyPage(IDD, IDS_CERTIFY_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCertifyPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyPropPage::DoDataExchange - Moves data between page and properties

void CCertifyPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCertifyPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyPropPage message handlers
