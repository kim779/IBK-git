#if !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)
#define AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_



#ifndef __CHARTMSG_DEF_H
#define __CHARTMSG_DEF_H
#endif 
/////////////////////////////////////////////////////////////////////////////
// ����Ʈ ���� 
//

#define	UDM_DBCLKJANGOLIST			WM_USER+1100		// �ܰ� ����Ʈ���� Double Clik
#define	UDM_DBCLKCOUNTLIST			WM_USER+1200		// ��ü�� ����Ʈ���� Double Clik

#define	UDM_DESTROYJANGODLG			WM_USER+1700
#define	UDM_DESTROYCOUNTDLG			WM_USER+1800
#define	UDM_DESTROYSETUPNUMBERDLG	WM_USER+1900
#define	UDM_DESTROYMAXJUMUNDLG		WM_USER+1910		// JSJ_Add_040421
#define	UDM_DESTROYSETUPPRICEDLG	WM_USER+1920		// JSJ_Add_040421
#define	UDM_DESTROYMAEDOMAESUDLG	WM_USER+1930		// JSJ_Add_040503
#define	UDM_CODESHAREFROMEDITBOX	WM_USER+2000

#define UDM_SPIN_CHANGE				WM_USER+2100		// UpDown Message	
#define	UDM_BUTTON_CLICK			WM_USER+2200		// ��Ÿ ȣ�� Message

#define	UDM_PASS_ENTER				WM_USER+2400		// Pass���� Enter

#define	UDM_FROM_ACCT				WM_USER+2500		// AcctEdit���� LengthFull, Enter

const UINT RMSG_FROMCODELIST = ::RegisterWindowMessage(_T("RMSG_FROMCODELIST"));
const UINT RMSG_SETSHAREDATA = ::RegisterWindowMessage(_T("RMSG_SETSHAREDATA"));
const UINT	RMSG_OPENMAP2		= ::RegisterWindowMessage(_T("RMSG_OPENMAP2"));
const UINT RMSG_FROMHOGAJUMUN_DESTROY = ::RegisterWindowMessage(_T("RMSG_FROMHOGAJUMUN_DESTROY"));
const UINT RMSG_HOGACHANGEDATA_CODE = ::RegisterWindowMessage(_T("RMSG_HOGACHANGEDATA_CODE"));
const UINT RMSG_HOGACHANGEDATA_ACCT = ::RegisterWindowMessage(_T("RMSG_HOGACHANGEDATA_ACCT"));
const UINT RMSG_SHARED_PIN = ::RegisterWindowMessage("RMSG_SHARED_PIN");
const UINT RMSG_RESIZEFROMGWAN = ::RegisterWindowMessage("RMSG_RESIZEFROMGWAN");
const UINT RMSG_MESSAGESHOW = ::RegisterWindowMessage("RMSG_MESSAGESHOW");
const UINT RMSG_GETSKINVALUE	=	::RegisterWindowMessage("RMSG_GETSKINVALUE");

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)

