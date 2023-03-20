// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_K1driver.h"
#include "ControlWnd.h"
#include "trsmain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();
}

CControlWnd::CControlWnd(CWnd *parent)
{
	m_pWizard = parent;
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "start", start, VT_EMPTY, VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D10011FE-28CA-460A-ABBF-0DC89CA9DAAC}
static const IID IID_IControlWnd =
{ 0xd10011fe, 0x28ca, 0x460a, { 0xab, 0xbf, 0xd, 0xc8, 0x9c, 0xa9, 0xda, 0xac } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

void CControlWnd::start(short nindex) 
{
	// TODO: Add your dispatch handler code here
	
	//==============��Ʈ ����============
	if(PHONEPAD_PortOpen(0))
	{
		PHONEPAD_InitDevice();
		PHONEPAD_SetConfig(0,10);	
		PHONEPAD_GetPWSignalWindow(m_hWnd);
		
		
		
		MessageBox("���е� ���� ����(Port Open Success)!!","IBK��������");
	}
	else
	{
		//char * tmp = "0423";
		//PostMessage(WM_PHONEPAD,0,(long)tmp);
		MessageBox("���е� ���� ����(Port Open Failure)!!","IBK��������");
		return;
	}
	//==============��й�ȣ �о���� ����============
	// TODO: Add your control notification handler code here
	char * temp = NULL;
	int  loop=1;
	int  limit = 0;
	
	
	while(loop)
	{			
		//temp = PHONEPAD_GetPassWDSTR();
		//if(temp != NULL) break;		
		Sleep(300);
		
		switch(PHONEPAD_Status())
		{
		case 0: 
			break;     // normal
			
		case 1: 
			loop = 0; 
			temp = PHONEPAD_GetPassWDSTR();
			break;     //  ��й�ȣ �Է¿Ϸ�
			
		case 2: 
			MessageBox("�ð��� �ʰ��Ͽ����ϴ�.","IBK��������");
			m_pWizard->CloseWindow();
			//m_phone_dlgs = NULL;
			return; 
			break;       //  ��й�ȣ �Է����
		}
		Sleep(300);
		
		limit++;
		if (limit>=20) break;
	}
	if (limit == 20)
	{
		m_pWizard->CloseWindow();
		//m_phone_dlgs = NULL;
		MessageBox("�Է� �ð��� �ʰ��Ǿ����ϴ�.","IBK��������");
		
	}	
	
	AfxMessageBox((LPCTSTR)temp);
	PHONEPAD_PortClose();

}
