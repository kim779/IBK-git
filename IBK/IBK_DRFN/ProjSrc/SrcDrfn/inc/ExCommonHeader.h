#if !defined(EXCOMMONHEADER)
#define EXCOMMONHEADER

#include "xUtil.h"

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#define SAFE_DELETE(p)  if(p) { delete p; p = NULL; }
#define _delete			SAFE_DELETE			 

#define SAFE_DELETE_AR(p)	if(p) { delete [] p; p = NULL; }
#define _deleteArray		SAFE_DELETE_AR

#define SAFE_RELEASE(p)		if(p) { (p)->Release(); (p)=NULL; }

//-->Solomon-090829����. 
// #define		BUTTON_DLL		_T("chart\\ExButton.dll")
// #define		STATIC_DLL		_T("chart\\ExStatic.dll")
// #define		EDIT_DLL		_T("chart\\ExEdit.dll")
// #define		ACCTINPUT_DLL	_T("chart\\ExAcctInput.dll")
// #define		CODEINPUT_DLL	_T("chart\\ExCodeInput.dll")
// #define		SPININPUT_DLL	_T("chart\\ExSpinInput.dll")
// #define		TABCTRL_DLL		_T("chart\\ExTabCtrl.dll")
// #define		LISTCTRL_DLL	_T("chart\\ExListCtrl.dll")
// #define		GRIDCTRL_DLL	_T("chart\\ExGridCtrl.dll")
// #define		TOOLTIPCTRL_DLL	_T("chart\\ExToolTipCtrl.dll")
// #define		COMBOBOX_DLL	_T("chart\\ExComboBox.dll")
// #define		SIGNAL_DLL		_T("chart\\ExSignal.dll")
// #define		DEPOSITIMG_DLL	_T("chart\\ExDepositImg.dll")

#define		BUTTON_DLL		_T("ExButton.dll")
#define		STATIC_DLL		_T("ExStatic.dll")
#define		EDIT_DLL		_T("ExEdit.dll")
#define		ACCTINPUT_DLL	_T("ExAcctInput.dll")
#define		CODEINPUT_DLL	_T("ExCodeInput.dll")
#define		SPININPUT_DLL	_T("ExSpinInput.dll")
#define		TABCTRL_DLL		_T("ExTabCtrl.dll")
#define		LISTCTRL_DLL	_T("ExListCtrl.dll")
#define		GRIDCTRL_DLL	_T("ExGridCtrl.dll")
#define		TOOLTIPCTRL_DLL	_T("ExToolTipCtrl.dll")
#define		COMBOBOX_DLL	_T("ExComboBox.dll")
#define		SIGNAL_DLL		_T("ExSignal.dll")
#define		DEPOSITIMG_DLL	_T("ExDepositImg.dll")
//<--Solomon-090829����. 

#define		BTN_TYPE_DRAW			0
#define		BTN_TYPE_BITMAP			1
#define		BTN_TYPE_IMAGELIST		2
#define		BTN_TYPE_ICON			3

#define		BTN_ICON_NORMAL			0
#define		BTN_ICON_RIGHT			1
#define		BTN_ICON_TWOROWS		2

#define		STC_ICON_NORMAL			0
#define		STC_ICON_RIGHT			1
#define		STC_ICON_TWOROWS		2

enum TouchStyle{TCH_ALL, TCH_IGNORE};

typedef struct tagST_ACTIONCONTROL
{
	UINT	m_nFlag;
	LPVOID	m_lpAction;
}ST_ACTIONCONTROL, FAR *LPST_ACTIONCONTROL;


typedef void	(* FpNotifyControl)(UINT);

const UINT EXWM_INITIAL_UPDATE = (WM_USER + 2526);
const UINT RMSG_FORMINITIALCOMPLETED = ::RegisterWindowMessage(_T("RMSG_FORMINITIALCOMPLETED"));

const UINT	EXDLG_INTERFACE_TRCOMM =0x0002;
const UINT	EXDLG_INTERFACE_SKIN = 0x0004;
const UINT	EXDLG_INTERFACE_PCTR = 0x008;
const UINT	EXDLG_INTERFACE_DRDS = 0x0010;
const UINT	EXDLG_INTERFACE_MASTER = 0x0020;
const UINT	EXDLG_INTERFACE_GDI = 0x0040;
const UINT	EXDLG_INTERFACE_DEFSKIN = 0x0080;
const UINT	EXDLG_INTERFACE_ALL = EXDLG_INTERFACE_TRCOMM|EXDLG_INTERFACE_SKIN|EXDLG_INTERFACE_PCTR|EXDLG_INTERFACE_DRDS|EXDLG_INTERFACE_MASTER|EXDLG_INTERFACE_GDI;
const UINT	EXDLG_INTERFACE_DEFALL = EXDLG_INTERFACE_TRCOMM|EXDLG_INTERFACE_DEFSKIN|EXDLG_INTERFACE_PCTR|EXDLG_INTERFACE_DRDS|EXDLG_INTERFACE_MASTER|EXDLG_INTERFACE_GDI;

#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

typedef void (* FpChangeSkinNotify)(void);

typedef struct tagST_CTRLEVENT
{
	LPCSTR lpszEventName;
	LPCSTR lpszParam;
}ST_CTRLEVENT, FAR *LPST_CTRLEVENT;

const UINT RMSG_EVENT_CODEINPUT = ::RegisterWindowMessage(_T("RMSG_EVENT_CODEINPUT"));
const UINT RMSG_EVENT_ACCTINPUT = ::RegisterWindowMessage(_T("RMSG_EVENT_ACCTINPUT"));
const UINT RMSG_EVENT_SPININPUT = ::RegisterWindowMessage(_T("RMSG_EVENT_SPININPUT"));
const UINT RMSG_EVENT_SIGNAL = ::RegisterWindowMessage(_T("RMSG_EVENT_SIGNAL"));

const UINT RMSG_FORMSTATECHANGE = ::RegisterWindowMessage(_T("RMSG_FORMSTATECHANGE"));
const UINT RMSG_TABSTOP = ::RegisterWindowMessage(_T("RMSG_TABSTOP"));

// CExControl���� LButton Double Click Message
// LPARAM : ��Ʈ�� �ڵ�(HWND)
const UINT RMSG_LBTNDBLCLICK = ::RegisterWindowMessage(_T("RMSG_LBTNDBLCLICK"));
// CExControl���� LButton Down Message
// LPARAM : ��Ʈ�� �ڵ�(HWND)
const UINT RMSG_LBTNDOWN = ::RegisterWindowMessage(_T("RMSG_LBTNDOWN"));

// WPARAM : SB_HORZ, SB_VERT
const UINT RMSG_ENDSCROLLPOS = ::RegisterWindowMessage(_T("RMSG_ENDSCROLLPOS"));

const UINT RMSG_LSHAREDCHANGED = ::RegisterWindowMessage("RMSG_LSHAREDCHANGED");

const UINT ACCTINPUT_PROP_STOCK		=	0;		// �ֽ� (�⺻)
const UINT ACCTINPUT_PROP_FO		=	1;		// ����,�ɼ�

const UINT CODEINPUT_PROP_STOCK		=	0;		// �ֽ� (�⺻)
const UINT CODEINPUT_PROP_FO		=	1;		// ����,�ɼ�
const UINT CODEINPUT_PROP_FUT		=	2;		// ����
const UINT CODEINPUT_PROP_OPT		=	3;		// �ɼ�
const UINT CODEINPUT_PROP_ITEM		=	4;		// ����
const UINT CODEINPUT_PROP_FO_02		=	5;		// ����,�ɼ�(��ǲ��ȯ ����)
const UINT CODEINPUT_PROP_FUT_02	=	6;		// ����(��ǲ��ȯ ����)

const UINT SPININPUT_PROP_STOCKQTY	=	0;		// �ֽļ��� (�⺻)
const UINT SPININPUT_PROP_INNERPRC	=	1;		// �峻ȣ��
const UINT SPININPUT_PROP_FUTPRC	=	2;		// ����ȣ��
const UINT SPININPUT_PROP_OPTPRC	=	3;		// �ɼ�ȣ��
const UINT SPININPUT_PROP_FUTQTY	=	4;		// ��������
const UINT SPININPUT_PROP_OPTQTY	=	5;		// �ɼǼ���
const UINT SPININPUT_PROP_NORMAL	=	6;		// �Ϲݽ���

const UINT SIGNAL_PROP_STOCKQTY		=	0;		// �ֽĽ�ȣ�� (�⺻)
const UINT SIGNAL_PROP_NOSHARE		=	1;		// �ֽĽ�ȣ�� (�ڵ���� ����)

#ifdef _COMMFUNC
//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/12/2
// Return		: ������ �Ķ���Ͱ� ������ TRUE, ������ FALSE
// Paramter
//		LPSTR lpszCompare : �˻��� �Ķ���� ���� ���ڿ�
//		CString strParamter : DtsDll.ocx�κ��� ���� �Ķ���� ��ü ���ڿ�
// Comments		: dllȭ�� �ʱ�ȭ���� �Ѱܹ��� �Ķ���ͷ� �б��۾��̳� Flag���涧��
//				  �Ķ���ͷ� �����Ͽ� ����� �� �ֽ��ϴ�.
//				  DtsDll.ocx�� ���� �Ѱܹ��� �Ķ���ʹ� �� 3���� ���ڿ��� �����Ǿ��ְ�
//				  CExOCXDLL������ ����� �����ϰ� ������ ����� ����� ��� �Ķ���͸� 
//				  ������ �ֽ��ϴ�. 
//				  �� �Լ��� �ʱ�ȭ���� �����ϰ� �Ķ���͸� �˻��ϴ� �Լ��Դϴ�.
//
//		��� ��)
//			if(CompareParamter(_T("DLLTYPE=F001"), strParam1))
//				AfxMessageBox("�� ȭ�鿡�� ��밡���� �Ķ�����Դϴ�.");
//			else
//				AfxMessageBox("�� ȭ�鿡�� ��밡���� �Ķ���Ͱ� �����ϴ�.");
//-----------------------------------------------------------------------------
extern BOOL CompareParamter(LPSTR lpszCompare, CString strParamter);

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/12/2
// Return		: void
// Paramter
//		LPCTSTR lpszKey : �Ķ���Ͱ��� Ű ���ڿ�
//		CString& strValue : �Ķ���� �� ���ڿ� ����
//		CString strParamter : ��ü �Ķ���� ���ڿ�
// Comments		: dllȭ�� �ʱ�ȭ���� �Ѱܹ��� �Ķ���Ϳ��� ������ Ű���� �ش��ϴ� 
//				  ���ڿ��� ���Ҷ� ����մϴ�.
//				  CompareParamter �Լ�ȭ�� �޸� �Ķ���� Ű���� �ش��ϴ� ���� �� �� �ֱ⶧����
//				  �ش簪�� ���� �бⰡ �����մϴ�.
//				  CompareParamter �Լ��� ����Ѵٸ� �б��� �� ��ŭ �Ķ���� ��ü ���ڿ��� �˻��մϴ�.
//
//		��� ��)
//			CString strValue(_T(""));
//			GetParamterValue(_T("DLLTYPE"), strValue, strParam1);
//			if(!strValue.CompareNoCase(_T("F001")))
//				AfxMessageBox("�� ȭ�鿡�� ��밡���� �Ķ�����Դϴ�.");
//			else if(!strValue.CompareNoCase(_T("S001")))
//				AfxMessageBox("�� ȭ�鿡�� ��밡���� �Ķ�����Դϴ�.");
//			else
//				AfxMessageBox("�� ȭ�鿡�� ��밡���� �Ķ���Ͱ� �����ϴ�.");
//-----------------------------------------------------------------------------
extern void GetParamterValue(LPCTSTR lpszKey, CString& strValue, CString strParamter);

extern CArray<HANDLE, HANDLE>		m_arPermanentHandle;

extern void SetPermanentHandle(HANDLE h, CObject* permOb);
extern void RemovePermanentHandle(HANDLE h);
#endif

enum {SKINBTN_DEFAULT = 0, SKINBTN_ACCENT, SKINBTN_TAB, SKINBTN_BUY, SKINBTN_SELL, SKINBTN_MODIFY, SKINBTN_CANCEL};
enum {EXTAB_NORMAL = 0, EXTAB_SELL, EXTAB_BUY, EXTAB_MODIFY, EXTAB_02, EXTAB_CANCEL};
enum {DEPOSIT_20 = 0, DEPOSIT_30, DEPOSIT_40, DEPOSIT_100};

#endif // !defined(EXCOMMONHEADER)
