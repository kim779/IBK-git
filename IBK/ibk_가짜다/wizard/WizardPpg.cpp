// WizardPpg.cpp : Implementation of the CWizardPropPage property page class.

#include "stdafx.h"
#include "Wizard.h"
#include "WizardPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CWizardPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWizardPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CWizardPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

//IMPLEMENT_OLECREATE_EX(CWizardPropPage, "AxisWizard.WizardPropPage.IBK", //vc2019 ibk2019
IMPLEMENT_OLECREATE_EX(CWizardPropPage, "AxisWizard.WizardPropPage.IBK2019",
	0x551d6bde, 0xaddc, 0x46ca, 0x87, 0xa3, 0xb5, 0xe4, 0x37, 0x31, 0x65, 0xc1)


/////////////////////////////////////////////////////////////////////////////
// CWizardPropPage::CWizardPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CWizardPropPage

BOOL CWizardPropPage::CWizardPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WIZARD_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CWizardPropPage::CWizardPropPage - Constructor

CWizardPropPage::CWizardPropPage() :
	COlePropertyPage(IDD, IDS_WIZARD_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CWizardPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CWizardPropPage::DoDataExchange - Moves data between page and properties

void CWizardPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CWizardPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CWizardPropPage message handlers
