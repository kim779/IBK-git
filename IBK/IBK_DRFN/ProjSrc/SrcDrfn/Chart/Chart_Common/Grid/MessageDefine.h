#if !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)
#define AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_


// ���°���
#define		CODE_LENGTH		7
#define		ACCT_LENGTH		11 // "-" ���ŵ� ���� 13

// ���ð���
#define		NPRSN_LOCAL_RESIZE		"MAP_RESIZE"			// ���ΰ��� ����(������ �� 9�ڸ��� �׻� "!NP_LOCAL"�̾�� ��)


// �޽��� 
#define	UDM_DBCLKJANGOLIST			WM_USER+1100			// �ܰ� ����Ʈ���� Double Clik

// GridCtrl.h���� ����
//#define	UDM_DROPORDER_CANCEL		WM_USER+1300		// ��� �ֹ����� ������ Drop
//#define	UDM_DROPORDER_CHANGE		WM_USER+1400		// ���� �ֹ����� ������ Drop
//#define	UDM_TRACE_MOUSEMOVE			WM_USER+1600		// �׸��忡�� Mouse Move

// PopupOrder.h���� ����
//#define	UDM_DROPGRID_FROM_POPUP		WM_USER+1500		// �˾����� �׸���� Drop

#define	UDM_HIDE_CANCEL_DLG			WM_USER+1800
#define	UDM_DESTROY_SETUPNUMBER_DLG	WM_USER+1900
#define	UDM_DESTROY_MAXJUMUN_DLG	WM_USER+1910		// JSJ_Add_040421
#define	UDM_DESTROY_SETUPPRICE_DLG	WM_USER+1920		// JSJ_Add_040421
#define	UDM_DBCLK_MICHAELIST		WM_USER+1930		// JSJ_Add_040503
#define UDM_CODECHANGE_YES			WM_USER+1940		// JSJ_Add_040908
#define UDM_CODECHANGE_NO			WM_USER+1950		// JSJ_Add_040908
#define	UDM_DESTROY_VOLUMESET_DLG	WM_USER+1960		// JSJ_Add_040503

#define	UDM_CODESHAREFROMEDITBOX	WM_USER+2000
#define UDM_SPIN_CHANGE				WM_USER+2100		// UpDown Message	
#define	UDM_BUTTON_CLICK			WM_USER+2200		// ��Ÿ ȣ�� Message

#define	UDM_PASS_ENTER				WM_USER+2400		// Pass���� Enter
#define	UDM_NUMBER_ENTER			WM_USER+2410		// ����, �ݾ׿��� Enter

#define	UDM_FROM_ACCT				WM_USER+2500		// AcctEdit���� LengthFull, Enter



const UINT RMSG_FROMCODELIST			= ::RegisterWindowMessage(_T("RMSG_FROMCODELIST"));			// �����ڵ� ����
const UINT RMSG_SETSHAREDATA			= ::RegisterWindowMessage(_T("RMSG_SETSHAREDATA"));			// ���� �޼��� 
const UINT RMSG_RESIZEDLL				= ::RegisterWindowMessage(_T("RMSG_RESIZEDLL"));			// ȭ�� ������¡
const UINT RMSG_DISPMESSAGE				= ::RegisterWindowMessage(_T("RMSG_DISPMESSAGE"));			// �޽��� ����
const UINT RMSG_OPENMAPWITHDATA			= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHDATA"));		// ���� ���� �����͸� �ѱ�
const UINT RMSG_OPENMAPWITHSHARE		= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHSHARE"));		// ���� ���� �����ڵ带 �ѱ�
const UINT RMSG_REAUESTMAINFUNC			= ::RegisterWindowMessage(_T("RMSG_REAUESTMAINFUNC"));		// ��й�ȣ ����
const UINT RMSG_GETSKINVALUE			= ::RegisterWindowMessage(_T("RMSG_GETSKINVALUE"));			// ����
const UINT RMSG_GETMAPTR				= ::RegisterWindowMessage(_T("RMSG_GETMAPTR"));				// ȭ���ȣ ��û
const UINT RMSG_GETACCTPASSENCKEY		= ::RegisterWindowMessage(_T("RMSG_GETACCTPASSENCKEY"));	// ���º�й�ȣ ��û
const UINT RMSG_VERIFYACCTPASS			= ::RegisterWindowMessage(_T("RMSG_VERIFYACCTPASS"));		// ���¹�ȣ ����(����, �ܺ� ���о���)
const UINT RMSG_REFRESHCOMPONENT		= ::RegisterWindowMessage(_T("RMSG_REFRESHCOMPONENT"));		// ��й�ȣ ����(��������ȸ)
const UINT RMSG_SETFOCUSTOCODE			= ::RegisterWindowMessage(_T("RMSG_SETFOCUSTOCODE"));		// �����ڵ� ��Ŀ��

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)

