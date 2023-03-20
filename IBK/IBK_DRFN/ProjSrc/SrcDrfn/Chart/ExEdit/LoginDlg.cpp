// LoginDlg.cpp : implementation file
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

//****************************************************************************
// INCLUDE HAEDER
//****************************************************************************
#include "stdafx.h"
#include <imm.h>
#include <direct.h>
#include "LoginDlg.h"
#include "UserInfo.h"
#include "WndVersion.h"	
#include "../ToolKit/Registry/Registry.h"

//****************************************************************************
// DEFINE MACRO
//****************************************************************************
#define			FILE_IPDAT			"\\Data\\IP.dat"
#define			FILE_MAININI		"\\Data\\Main.ini"
#define			IMG_LOGGIN		    "\\Login_building.bmp"

#define			TYPE_STAFF			-1	// WinnetPro
#define			TYPE_CLIENT			 2	// WinnetProMini
#define			TYPE_JANGOJOHO		-2	// �ܰ���ȸ��
#define			TYPE_EXTRA			-3
#define			TYPE_UNKNOW			 0	//Unknown

// �α��� ����
#define		    NORMAL_INPUT		0x50			// �Ϲ����� �α���
#define			NEWREG_INPUT		0x58			// �ű� ���԰�
#define			GUEST_INPUT			0x57			// guest
#define			SISEONLY_INPUT		0x59			// �ü�����

//****************************************************************************
// REGISTER MESSAGE
//****************************************************************************
const UINT RMSG_KEYMESSAGE = ::RegisterWindowMessage("RMSG_KEYMESSAGE");

//****************************************************************************
// GLOBAL FUNCTION
//****************************************************************************
int gGetProgrameType()
{
	CRegistry registry;
	registry.SetRootKey(HKEY_CURRENT_USER);
	registry.SetKey("Software\\Dongbu Securities\\WinNetNew\\Setup", FALSE);

	CString m_szUserType = registry.ReadString("Type", "Client");

	if(m_szUserType.CompareNoCase("Staff")==0)		return TYPE_STAFF;
	if(m_szUserType.CompareNoCase("Client")==0)		return TYPE_CLIENT;
	if(m_szUserType.CompareNoCase("JangoJoho")==0)	return TYPE_JANGOJOHO;
	if(m_szUserType.CompareNoCase("Extra")==0)		return TYPE_EXTRA;
	return TYPE_UNKNOW;
}


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CLoginDlg::CLoginDlg(HWND hSockDlg, void* pBuffer, void* pUserInfo, LPCTSTR szRootPath, int nSetupType,CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent),
	m_hSocket(NULL),
	m_bVerCheckCompleted(FALSE)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_szID		= _T("");
	m_szPassWd	= _T("");
	//}}AFX_DATA_INIT

//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nSetupType = nSetupType;
	m_szRootPath = szRootPath;

	m_boldFont.CreatePointFont (90,_T("����ü"));
	LOGFONT logFont;
	m_boldFont.GetLogFont (&logFont);
	m_boldFont.DeleteObject();

	logFont.lfWeight = FW_BOLD;
	m_boldFont.CreateFontIndirect(&logFont);


	m_szIni.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), DEFMAININI);
	m_szDat.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), "main.dat");
	m_szLoginFile = _T("");
	m_szCmd = _T("general");

	// {{ �Ķ���͸� üũ�Ѵ�. 
	char chBuf[7+28+1] = {0,};
	char chTmp[30] = {0,};
	LPSTR lpUser = (LPSTR)pUserInfo;
	memcpy(chBuf, lpUser, 36);
	
	memcpy(chTmp, chBuf, 7);

	if (strcmp(chTmp, "noversion")==0)
	{
		m_szCmd = _T("noversion");
		m_bVerCheckCompleted = TRUE;
	}
	else if((strcmp(chTmp, "version")==0) || (strcmp(chTmp, "restart")==0))
	{	// ���� �Ǵ� ����ۿ� ���� ���� 
		if (strcmp(chTmp, "version")==0)		m_szCmd = _T("version");
		else									m_szCmd = _T("restart");
		memset(chTmp, 0, 30);
		memcpy(chTmp, chBuf+7, 20);
		m_szID = chTmp;
		m_szID.TrimRight();
		memset(chTmp, 0, 30);
		memcpy(chTmp, chBuf+27, 8);
		m_szPassWd = chTmp;
		m_szPassWd.TrimRight();
	}
	else if (strcmp(chTmp, "noversi")==0)
	{
		m_szCmd = _T("noversi");
		m_bVerCheckCompleted = TRUE;
	}

//	FindStyle();

	m_hSocketDlg = hSockDlg;
	m_pBuffer    = pBuffer;
	
	m_bUseCert   = TRUE;
	m_bSiseOnly  = FALSE;
	m_bAutoPass  = FALSE;
	m_bIDSave	 = FALSE;

	m_nIDType		= -1;
	m_nUserGB		= 0;
	m_nMethod		= 0;
	m_nAuthSkip		= 0;
	m_nProgrameType = 0;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDT_LOGINID, m_szID);
	DDX_Text(pDX, IDC_EDT_PASSWD,  m_szPassWd);
	DDX_Control(pDX, IDC_BTN_SETTING, m_btnSetting );
	DDX_Control(pDX, IDOK,			  m_btnConnect);
	DDX_Control(pDX, IDCANCEL,		  m_btnExit );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_COPYDATA()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	SetImeMode(IME_CMODE_ROMAN);

	LoadImages();
	MoveCtrls();

/*
	LRESULT lResult = InstallKeyHook();
	if (lResult != KH_OK)
	{
		AfxMessageBox("�ʱ�ȭ�� �����Ͽ����ϴ�.");
		return TRUE;
	}

	m_entry.nMessage = RMSG_KEYMESSAGE; // Our message ID
	m_entry.hCallWnd = m_hWnd; // Send message to this window
	m_entry.hHookWnd = 0; // Capture key-strokes occurred in any windows
	m_entry.iCombKeys = 0; // combination keys don't matter
	m_entry.iIndicators = 0; // Caps-lock, Num-lock, Scroll-lock on/off states do not matter
	m_entry.iKeyEvent = 0; // Capture all key events
	m_entry.iMinVKCode = 0x00; // Capture all keys
	m_entry.iMaxVKCode = 0xff;

	// Add the entry to the hook
	lResult = AddKeyEntry(&m_entry);
	if (lResult != KH_OK)
	{
		AfxMessageBox("��Ͽ� �����Ͽ����ϴ�.");
		return TRUE;
	}
*/

	if(!LoadDlgData()) {
		EndDialog(IDCANCEL);
		return FALSE;
	}

	InitControl();

	if(m_szCmd.CompareNoCase("Version")==0) {	// ����ó���� ��쿣 �α�ȭ���� �ٽ� �������� ���� 
		ShowWindow(SW_MINIMIZE);	
		OnConnectVersion();
	}
	else if (m_szCmd.CompareNoCase("Restart")==0)
	{
		ShowWindow(SW_MINIMIZE);	
		OnConnect();	
	}

	GetDlgItem(IDC_EDT_LOGINID)->SetWindowText ( m_szRealID );
	GetDlgItem(IDC_EDT_PASSWD )->SetFocus();
	GetDlgItem(IDC_EDT_CERTPWD)->EnableWindow (FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLoginDlg::SetImeMode(DWORD mode)
{
	//�������� : �α��ν� �ѱ� �Է� ��带 �������� ����
	HIMC hIMC;
	DWORD dwConv, dwSent;
	DWORD dwTemp;

	hIMC = ImmGetContext(GetSafeHwnd());
	ImmSetOpenStatus(hIMC, TRUE);
	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);
	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	dwTemp |= mode;

	dwConv = dwTemp;
	ImmSetConversionStatus(hIMC, dwConv, dwSent);
	ImmReleaseContext(GetSafeHwnd(), hIMC);
}


BOOL CLoginDlg::LoadDlgData()
{
	CFileFind find;
	if(find.FindFile(m_szIni)==FALSE) {
		CString szErr;
		szErr.Format("���࿡ �ʿ��� ����[%s]�� ã�� �� �����ϴ�.", m_szIni);
		AfxMessageBox(szErr);
		return FALSE;
	}

	m_nMethod = ::GetPrivateProfileInt("CONNECT", "METHOD", 0, m_szIni);

	if(m_szRealID.GetLength()==0) {			// ������ ���� ����� ��� ID�� �ٽ� ���� �ʿ䰡 ����. 
		CString szPath;
		szPath.Format("%s%s", m_szRootPath, _T("\\Data\\"));
		CUserInfo userInfo(szPath);
		userInfo.GetUserID(m_szRealID);
	}

	::GetPrivateProfileString("CALL", "LOGINMSG",
		"�̿��� ID�� ��й�ȣ�� �Է��Ͻñ� �ٶ��ϴ�.", 
		m_szMessage.GetBuffer(MAX_PATH),
		MAX_PATH ,
		m_szDat);
	m_szMessage.ReleaseBuffer();

	CString szIPData;
	szIPData.Format("%s\\Data\\IP.dat", m_szRootPath);
	m_nAuthSkip = GetPrivateProfileInt("OPTION", "AUTH", 0, szIPData);

	return TRUE;
}


void CLoginDlg::OnConnect() 
{
	UpdateData(TRUE);

	int nTestMode = ::GetPrivateProfileInt("DEBUG", "TestMode", 0, m_szIni);
	if(nTestMode==111)
	{
//		BTNSTATE(FALSE);
		m_bVerCheckCompleted=TRUE;
		TestSocket();
		TestMode();
		return;
	}

	// ������� �Է��� üũ�Ѵ�. ���̵�, ��й�ȣ
	if(!CheckAndSaveUserinfo()) 
	{
		return;
	}
	
	// ��ư ���¸� ��� ���̰� �Ѵ�.
//	BTNSTATE(FALSE);
		
	if(m_nMethod==1) 
	{ // ��ȭ�����ΰ��
		return;
	}

	SetConnectServer();
	
	CString tmp_IsVerPath;
	tmp_IsVerPath.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), _T("main.ini"));
	int tmp_IsVer = ::GetPrivateProfileInt(_T("CONFIG"), _T("VERSION"), 1, tmp_IsVerPath);
	if (tmp_IsVer != 0)
	{
		if(VersionCheck()==VER_OK) {
			m_bVerCheckCompleted=TRUE;
			CreateSocket();
			SetupUserFile();
		}
	}
	else
	{
		m_bVerCheckCompleted=TRUE;
		CreateSocket();
		SetupUserFile();
	}

	WriteLoginInfo();
//	BTNSTATE(TRUE);
}

void CLoginDlg::TestSocket()
{
	char buffer[28] = {0,};
	memcpy(buffer, m_szRealID, 20);
	memcpy(buffer+20, m_szPassWd, 8);
	
	LRESULT lRet = ::SendMessage(m_hSocketDlg, UDM_SOCK_CREATE, 0, (LPARAM)(LPSTR)buffer);
		
	char chRet[6] = {0,};
	memcpy(chRet, (LPSTR)(LPVOID)lRet, 6);
	CString szRet = chRet;
	if(szRet.GetLength()==0)	{ return; }
	
	CString szTemp = szRet.Left(1);
	int nResult =  atoi(szTemp);
	if(nResult==1) {
		szRet.Delete(0);
		HWND hSock = (HWND)(atol((LPCTSTR)szRet));
	}
}

void CLoginDlg::TestMode()
{
	char aID[20]={0,},
		aPasswd[9]={0,},
		aLevel[33]={0,},
		aIP[15]={0,},
		aGrade[4]={0,},
		aAcctList[200]={0,};
	int nAcctCount=0;

	::GetPrivateProfileString("TestMode", "ID", "tuja", aID, sizeof(aID), m_szIni);
	::GetPrivateProfileString("TestMode", "PASSWD", "tuja", aPasswd, sizeof(aPasswd), m_szIni);
	::GetPrivateProfileString("TestMode", "LEVEL", "", aLevel, sizeof(aLevel), m_szIni);
	::GetPrivateProfileString("CONNECT", "IP2", "", aIP, sizeof(aIP), m_szIni);
	::GetPrivateProfileString("TestMode", "GRADE", "", aGrade, sizeof(aGrade), m_szIni);
	::GetPrivateProfileString("TestMode", "ACCTLIST", "", aAcctList, sizeof(aAcctList), m_szIni);
	nAcctCount = ::GetPrivateProfileInt("TestMode", "ACCTCOUNT", 0, m_szIni);

	int nSize = nAcctCount * GEJA_SIZE;
	int nTotal = sizeof(STUSERINFO) + nSize +10;
	char*  LoginList   = new char[nTotal];
	STUSERINFO* pUInfo = (STUSERINFO*)LoginList;

	memset(pUInfo, 0x00, nTotal);
	memcpy(pUInfo->ID, aID, min(strlen(aID), 20));
	memcpy(pUInfo->Passwd, aPasswd, min(strlen(aPasswd), 8));
	memcpy(pUInfo->Level, aLevel, 32);  // Level ���� AccCount���� �޸𸮺��� 
	sprintf(pUInfo->AccCount, "%03.3d", nAcctCount);
	memcpy(pUInfo->AccList, aAcctList, nSize);	
	memcpy((LPSTR)m_pBuffer, pUInfo, nTotal);
	delete pUInfo;

	EndDialog(IDOK);
}

BOOL CLoginDlg::CheckAndSaveUserinfo()
{
	UpdateData(TRUE);

	// Temp
/*
	BOOL bHTSGuest = IsDlgButtonChecked(IDC_CHECK_NEWREG);

	CString szMsg;
	CString szID = m_szRealID;
	if(szID.GetLength()==0) {
		GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		if(bHTSGuest)
			ShowBallonTip(IDC_ID, _T("���¹�ȣ�� �Է��ϼ���"));
		else
			ShowBallonTip(IDC_ID, _T("���̵� �Է��ϼ���"));
		return FALSE;
	}
	else if(m_szPassWd.GetLength()==0) {
		if(bHTSGuest==FALSE)
		{
			if(szID.CompareNoCase(STRING_GUEST)!=0)
			{
				GetDlgItem(IDC_PASSWD)->SetFocus();
				ShowBallonTip(IDC_PASSWD, _T("��й�ȣ�� �Է��ϼ���"));
				return FALSE;
			}
		}
	}

*/
	CString szLoginID;
	GetDlgItem(IDC_EDT_LOGINID)->GetWindowText(szLoginID);

	szLoginID.TrimLeft();
	szLoginID.TrimRight();
	if ( szLoginID.IsEmpty() )
	{
		AfxMessageBox("�α� ���̵� �Է��� �ֽʽÿ�.");
		return FALSE;
	}
	else if ( szLoginID != m_szRealID )
	{
		m_szRealID = szLoginID;
	}

	CString szPath;
	szPath.Format("%s%s", m_szRootPath, _T("\\Data\\"));
	CUserInfo userInfo(szPath);
	userInfo.SetUserID(m_szRealID);

	return TRUE;
}


void CLoginDlg::SetConnectServer()
{
	CString szIPDat, szMainIni, szIP, szPort, szVPort, szIPB;	
	CString szSection;
	szIPDat		= m_szRootPath + FILE_IPDAT;
	szMainIni	= m_szRootPath + FILE_MAININI;

	int nLanType = GetPrivateProfileInt("CONNECT", "LANTYPE", 0, szMainIni);

	if (m_nSetupType == 0)
	{
		int nUserGB = GetPrivateProfileInt("CONNECT", "USERGB", 0, szMainIni);

		if (nUserGB == 0)
		{
			m_nUserGB = nUserGB;

			//Temp
//			int nIDType = ((CButton*)GetDlgItem(IDC_CHECK_HTSID))->GetCheck();
			int nIDType = 0;
			if (nIDType == 0)
			{
				m_nIDType = 1;
				m_bUseCert = FALSE;
				m_bAutoPass = FALSE;
				m_bSiseOnly = FALSE;
				if (nLanType == 0)
				{
					szSection = "WinTop_����(�系)";
				}
				else if (nLanType == 1)
				{
					szSection = "WinTop_����(�����ͼ���)";
				}
				else if (nLanType == 2)
				{
					szSection = "WinTop_�ü�(�ܺ�)";
				}
				else 
				{
					szSection = "WinTop_����(�系)";
				}
				//szSection = "WinTop_����(�系)";
			}
			else
			{
				m_nIDType = 2;
				m_bUseCert = TRUE;
				m_bAutoPass = TRUE;
				m_bSiseOnly = FALSE;
				szSection = "WinNet_���̹���";
				if (nLanType == 0)
				{
					szSection = "WinNet_���̹���";
				}
				else if (nLanType == 1)
				{
					szSection = "WinNet_���̹���(�����ͼ���)";
				}
				else if (nLanType == 2)
				{
					szSection = "WinNet_���̹���(�����ͼ���)";
				}
				else
				{
					szSection = "WinNet_���̹���";
				}

			}
		}
		//else if (szUserGB.CompareNoCase("1") == 0)
		else if (nUserGB == 1)
		{
			m_nUserGB = nUserGB;
			m_nIDType = 2;
			m_bUseCert = TRUE;
			m_bAutoPass = TRUE;
			m_bSiseOnly = TRUE;
			szSection = "WinNet_���̹���";
			if (nLanType == 0)
			{
				szSection = "WinNet_���̹���";
			}
			else if (nLanType == 1)
			{
				szSection = "WinNet_���̹���(�����ͼ���)";
			}
			else if (nLanType == 2)
			{
				szSection = "WinNet_���̹���(�����ͼ���)";
			}
			else
			{
				szSection = "WinNet_���̹���";
			}
		}
		//else if (szUserGB.CompareNoCase("2") == 0)
		else if (nUserGB == 2)
		{
			m_nUserGB = nUserGB;
			m_nIDType = 1;
			m_bUseCert = FALSE;
			m_bAutoPass = FALSE;
			m_bSiseOnly = TRUE;
			szSection = "WinTop_�ü�(�ܺ�)";
			if (nLanType == 0)
			{
				szSection = "WinTop_�ü�(�ܺ�)";
			}
			else if (nLanType == 1)
			{
				szSection = "WinTop_����(�����ͼ���)";
			}
			else if (nLanType == 2)
			{
				szSection = "WinTop_�ü�(�ܺ�)";
			}
			else
			{
				szSection = "WinTop_�ü�(�ܺ�)";
			}
		}
	}
	else if (m_nSetupType == 1)
	{
		m_nIDType = 2;
		szSection = "WinNet_�����";
		if (nLanType == 0)
		{
			szSection = "WinNet_�����";
		}
		else if (nLanType == 1)
		{
			szSection = "WinNet_�����(�����ͼ���)";
		}
		else
		{
			szSection = "WinNet_�����";
		}
	}
	else if (m_nSetupType == 2)
	{
		m_nUserGB = 0;
		m_nIDType = 1;
		m_bUseCert = FALSE;
		m_bAutoPass = FALSE;
		m_bSiseOnly = FALSE;
		szSection = "WinTop_����(�系)";
		if (nLanType == 0)
		{
			szSection = "WinTop_����(�系)";
		}
		else if (nLanType == 1)
		{
			szSection = "WinTop_����(�����ͼ���)";
		}
		else if (nLanType == 2)
		{
			szSection = "WinTop_�ü�(�ܺ�)";
		}
		else
		{
			szSection = "WinTop_����(�����ͼ���)";
		}
	}

	GetPrivateProfileString(szSection, "IP", "", szIP.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szIP.ReleaseBuffer();
	GetPrivateProfileString(szSection, "IPB", "", szIPB.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szIPB.ReleaseBuffer();
	GetPrivateProfileString(szSection, "PORT01", "", szPort.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szPort.ReleaseBuffer();
	GetPrivateProfileString(szSection, "VPORT01", "", szVPort.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szVPort.ReleaseBuffer();

	szSection = "CONNECT";
	WritePrivateProfileString(szSection, "IP1", szIP, szMainIni);
	WritePrivateProfileString(szSection, "IP2", szIP, szMainIni);
	WritePrivateProfileString(szSection, "IPB", szIPB, szMainIni);
	WritePrivateProfileString(szSection, "PORT", szPort, szMainIni);
	WritePrivateProfileString(szSection, "VPORT", szVPort, szMainIni);
}


int CLoginDlg::VersionCheck()
{
	int nChk = ::GetPrivateProfileInt("DH_DEBUG", "VERSION", 0, "HANARO.INI");
	if(nChk==100)
		return VER_OK;

#ifdef _DEBUG
	return VER_OK;
#endif

	if(m_bVerCheckCompleted)
		return VER_OK;

	char buffer[28] = {0,};
	memcpy(buffer, m_szRealID, 20);
	memcpy(buffer+20, m_szPassWd, 8);

	CWndVersion version(this);
	int nResult = version.VersionCheck(buffer);

	switch(nResult) {
		case VER_OK:
			break;
		case VER_READY:
			break;
		case VER_TIMEOUT:
		case VER_EXECERROR:
		case VER_ERROREXIT:
		case VER_USERCANCEL:
			SendMessage(WM_COMMAND, IDCANCEL);
			break;
		case VER_RESTART:
			EndDialog(IDCANCEL);
			break;
	}

	return nResult;
}


void CLoginDlg::CreateSocket()
{
	if(!m_hSocket)
	{
		char buffer[28] = {0,};
		memset(buffer , 0x20, 28);
		memcpy(buffer, m_szRealID, 20);
		memcpy(buffer+20, m_szPassWd, 8);

		LRESULT lRet = ::SendMessage(m_hSocketDlg, UDM_SOCK_CREATE, 0, (LPARAM)(LPSTR)buffer);

		//////////////////////////////////////////////////////////////////////////
		// ���� ���� ���� IP / �ܺ� IP ������ üũ �Ѵ� (���� ���� üũ)
		CString szClientIP;
		LRESULT lRetGetIP = ::SendMessage(m_hSocketDlg, UDM_GET_IPADDRESS, 0, (LPARAM)&szClientIP);
		if(lRetGetIP)
		{
			if(!ClientIPCheck(szClientIP))
			{
				CString szMSG;
				szMSG.Format("Client IP : %s \r\nȯ�漳���� �� �� �Ǿ����ϴ�.",szClientIP);
				AfxMessageBox(szMSG);
				::SendMessage(m_hSocketDlg, UDM_SOCK_DISCONNECT, 0, 0);
				return ;
			}
		}

		STSOCKRETURN stRet;
		memcpy(&stRet, (LPVOID)lRet, sizeof(STSOCKRETURN));

		CString szMsg;
		int nResult = stRet.m_nResult;
		if(nResult==SOCKET_OK)					// ����ó��
		{
			m_hSocket = stRet.m_hSock;
			if(!::IsWindow(m_hSocket))
			{
				AfxMessageBox("Socket Error!");
			}
			LoginTRSend(m_hSocket);
		}
		else if(nResult==SOCKET_TIMEOUT)		// �ð��ʰ�
		{
			szMsg.LoadString(IDS_SOCKET_TIMEOUT);
			ShowEndMessage(szMsg);
		}
		else if(nResult==SOCKET_SOCKCREATEERR)	// ���� ���� ����
		{
			szMsg.LoadString(IDS_SOCKET_SOCKCREATEERR);
			ShowEndMessage(szMsg);
		}
		else if(nResult==SOCKET_CONNECTERRROR)	// ���� ����
		{
			szMsg.LoadString(IDS_SOCKET_CONNECTERRROR);
			ShowEndMessage(szMsg);
		}
		else if(nResult==SOCKET_RECEIVEQUIT)	// ���� ��� 
		{
			szMsg.LoadString(IDS_SOCKET_RECEIVEQUIT);
			ShowEndMessage(szMsg);
		}
		else									// ��Ÿ ����
		{
			szMsg.LoadString(IDS_SOCKET_UNKNOWNERR);
			ShowEndMessage(szMsg);
		}
	}
	else
	{
		LoginTRSend(m_hSocket);	
	}
}

void CLoginDlg::SetupUserFile()
{
	CString szUserFile;
	szUserFile.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), DEF_USERSELECT);

	int nLastUserType = ::GetPrivateProfileInt("UserType", "LastUserType", 0, szUserFile);
}


void CLoginDlg::WriteLoginInfo()
{
	UpdateData(TRUE);

	CFileFind ffind;
	CString UserFoler;
	UserFoler = m_szRootPath + "\\Usr";
	if(ffind.FindFile(UserFoler)==0)
	{
		_mkdir(UserFoler);
	}

/*
	UserFoler += "\\"+m_szRealID;
	if(ffind.FindFile(UserFoler)==0)
	{
		_mkdir(UserFoler);
	}
*/
	
	char chBuf[3] = {0,};
	
	::WritePrivateProfileString("CONNECT", "ID", m_szRealID, m_szIni);
	_itoa((int)m_bIDSave, chBuf, 10);
	::WritePrivateProfileString("IDSAVE", "SAVE", (LPCTSTR)(LPSTR)chBuf, m_szLoginFile);

	if (m_nSetupType == 1)
	{
/*		temp
		m_bUseCert = ((CButton*)GetDlgItem(IDC_CHECK_USECERT))->GetCheck();
		m_bSiseOnly = ((CButton*)GetDlgItem(IDC_CHECK_SISEONLY))->GetCheck();
		m_bAutoPass = ((CButton*)GetDlgItem(IDC_CHECK_AUTOPASS))->GetCheck();
*/
	}

	::WritePrivateProfileString("CERITIFICATION", "SISEONLY", m_bSiseOnly ? "1" : "0", m_szLoginFile);
	::WritePrivateProfileString("CERITIFICATION", "AUTOPASS", m_bAutoPass ? "1" : "0", m_szLoginFile);

/*   temp
	//Guest�� ��쿡�� �����������κ��� Check�� �Ǿ� �ִ��� �۵����� �ʰ�
	BOOL bHTSGuest = IsDlgButtonChecked(IDC_CHECK_NEWREG);
	if(bHTSGuest>0)
	{
		 m_bUseCert = FALSE;
		 m_bAutoPass = FALSE;
	}
*/
}

//////////////////////////////////////////////////////////////////////////
// Ŭ���̾�Ʈ�� ip �� üũ�Ͽ� ������ ȯ�漳�� ���װ� ���� ������ ������ 
// Close ��Ű�� Return �Ѵ�.
// 1. Staff :	-. �系 ����(L1 or L3) ���(L4) ������ �ľ��Ѵ�.
//				-. ���̹���(L3)������ �ľ��Ѵ�.
//				-. Hts(L3) / ����(L1) / �ݼ���(L1) ���������� �ľ��Ѵ�.
//
// 2. Client :  -. (L2)
//
// 3. Extra  :	-. �系�ΰ�� ip�� üũ�Ͽ� �ڵ��α��� �Ѵ�.(L1)
//				-. �ܺ��� ���(L4)
//				-. 20040330 ����: Extra �ϰ��� Client ip �� üũ���� �ʴ´�.
BOOL CLoginDlg::ClientIPCheck(CString szClientIP)
{

	return TRUE;
	CString szLanType , szUserGB;

	// ���� Ÿ��(L1 ~ L4)
	GetPrivateProfileString("CONNECT", "LAN", "" , szLanType.GetBuffer(50), 50, m_szIni);
	szLanType.ReleaseBuffer();

	// ����� ���� (szUserGB : 0(Staff), 1(HTS), 2(Cyber Jijum), 3(CyberRoom), 4(Extra))
	GetPrivateProfileString("CONNECT", "USERGB", "" , szUserGB.GetBuffer(50), 50, m_szIni);
	szUserGB.ReleaseBuffer();

	int nCheckState = GetPrivateProfileInt("CONNECT", "CHECKIP", 1 , m_szIni);

	if(!nCheckState)
		return TRUE;

	CString szIP_A,szIP_B,szIP_C,szIP_D;
	int ipIndex = szClientIP.Find('.');
	szIP_A = szClientIP.Left(ipIndex);
		szClientIP = szClientIP.Mid(ipIndex+1);
		ipIndex	   = szClientIP.Find('.');
	szIP_B = szClientIP.Left(ipIndex);
		szClientIP = szClientIP.Mid(ipIndex+1);
		ipIndex	   = szClientIP.Find('.');
	szIP_C = szClientIP.Left(ipIndex);
		szClientIP = szClientIP.Mid(ipIndex+1);
		//ipIndex    = szClientIP.Find('.');
	szIP_D = szClientIP;
	
	//////////////////////////////////////////////////////////////////////////
	// ���� IP ������ üũ�Ѵ�. 
	// ���� ip �� ip�� "192" �� �����ϴ� ip�� �����Ѵ�.
	BOOL bInnerClient;

#ifdef _BDS_TEST_

		bInnerClient = FALSE;
#else

	if(szIP_A == "192")
		bInnerClient = TRUE;
	else 
		bInnerClient = FALSE;
#endif

	if(m_nProgrameType == TYPE_STAFF)
	{
		if(bInnerClient)	// �系�� ���
		{
			// 1. ���̹� �������� �Ǵ��Ѵ�.(192.10.xx.40 ~ 69)
			int nIP_D = atoi(szIP_D);
			if(szIP_B == "10" && (nIP_D >= 40 && nIP_D <= 69))
			{
				if(szLanType != "L3")
					return FALSE;
			}
			// 2. ���̹� ���� �ƴϸ� ����� ���а��� ���� �Ǵ��Ѵ�.
			else
			{
				// ����� ����(szUserGB : 0(Staff), 1(HTS), 2(Cyber Jijum), 3(CyberRoom), 4(Extra))
				if(szUserGB == "0" )
				{
					if(szLanType != "L1")
						return FALSE;					
				}
				else if(szUserGB == "1")
				{
					if(szLanType != "L3")
						return FALSE;
				}
				else if(szUserGB == "2")
				{
					if(szLanType != "L1")
						return FALSE;
				}
				else
					return FALSE;

			}
		}
		else				// ����� ���
		{
			if(szLanType != "L4")
				return FALSE;
		}
		
	}
	else if(m_nProgrameType ==TYPE_CLIENT )
	{
		if(szLanType != "L2")
			return FALSE;
	}
	else if(m_nProgrameType == TYPE_EXTRA )
	{
		if(bInnerClient)
		{
			// 1. ���� ip �ΰ�쿡�� ���� ip�� üũ�Ѵ�.
			CString szIP_CFirst = szIP_C.Left(1);
			int nIP_D = atoi(szIP_D); 
			if(szIP_B == "10" && szIP_CFirst == "2" && (nIP_D >= 71 && nIP_D <= 129))
			{
				if(szLanType != "L1")
					return FALSE;					
				else
					return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			if(szLanType != "L4")
				return FALSE;	
		}

	}

	return TRUE;
	
}

///////////////////////////////////////////////////////////////////////////////
// ������             : ���ؿ�(Jun-Ok, Lee)
// Email              : alzioyes@hotmail.com
// ���� ��¥          : 2003/2/3
// �Լ� �̸�          : CLoginDlg::LoginTRSend
// ���� Ÿ��          : void
// �Ķ����
//		HWND hSocket   : 
// �Լ� ����          : IDC_CHECK_NEWREG üũ�Ǿ� ������ ���¹�ȣ�� �ѱ��
//						üũ�Ǿ����� ���� ��쿡�� "guest"�϶��� �ƴҶ��� ����ó���Ѵ�.
//						
///////////////////////////////////////////////////////////////////////////////
void CLoginDlg::LoginTRSend(HWND hSocket)
{
	CString szID = m_szRealID;
	TRLoginData loginData;
	memset(&loginData, 0x20, TRLoginDataSize);

/*
	// {{ TRLOGINDATA Packet �ۼ� 
	BOOL bHTSGuest = IsDlgButtonChecked(IDC_CHECK_NEWREG);;

	if(bHTSGuest)	// ���¹�ȣ �Է½�..
	{
		CheckDlgButton(IDC_CHECK_SISEONLY, 1);
		CheckDlgButton(IDC_CHECK_USECERT, 0);
		szID.Remove('-');
 		loginData.cmd = NEWREG_INPUT;	// 'X'
	}
	else 
	{
		if(szID.CompareNoCase(STRING_GUEST)==0)
		{
			CheckDlgButton(IDC_CHECK_SISEONLY, 1);
			CheckDlgButton(IDC_CHECK_USECERT, 0);
			loginData.cmd = GUEST_INPUT;	// 'W'
		}
		else
		{
			if (IsDlgButtonChecked(IDC_CHECK_SISEONLY))
				loginData.cmd = SISEONLY_INPUT;	
			else
				loginData.cmd = NORMAL_INPUT;	
		}
	}
*/

	// Temp
	loginData.cmd = NORMAL_INPUT;


	memcpy(loginData.userid, szID, szID.GetLength());
	memcpy(loginData.passwd, m_szPassWd, m_szPassWd.GetLength());

	int nUserType = m_nIDType;
	TCHAR cUserType = (nUserType==2)? '2' : '1';
	loginData.userType[0] = cUserType;


	Send2Server(hSocket, "08000", &loginData, TRLoginDataSize);
}

void CLoginDlg::ShowEndMessage(CString _szMsg)
{
	if(_szMsg.GetLength()>0) {
		AfxMessageBox(_szMsg);
		EndDialog(IDCANCEL);	
	}
}

short CLoginDlg::Send2Server(HWND hSock, LPCSTR _szTr, LPVOID szData, int nLen)
{
	if(!hSock) return 0;

	int nSize = sizeof(STTYPE400) + nLen + sizeof(char);

	STTYPE400* pTrData = (STTYPE400*)malloc(nSize+1); 
	memset(pTrData, 0x20, nSize);

	pTrData->bEnc = FALSE;
	pTrData->nLen = nSize;
	pTrData->trHeader.WindowID = (long)m_hWnd;
	memcpy(pTrData->trHeader.TRCode, _szTr, 5);
	memcpy(pTrData->trHeader.MsgCode, "000000", 6);
	memcpy(pTrData->trHeader.Message, "", 0);
	memcpy(pTrData->trHeader.CursorPos, "  ", 2);

	memcpy(pTrData->trHeader.Message2Len, "   ", 3);

	char aTmp[4]={0,};
	sprintf(aTmp, "%-03.3d", 0);
	memcpy(pTrData->trHeader.APDataLen, aTmp, 3);

	if(nLen>0)
		memcpy(pTrData->trHeader.TRData, szData, nLen);
	pTrData->trHeader.TRData[nLen] = 0;

	BOOL bRet = RealSend(hSock, (void*)pTrData, (LPARAM)nSize);

	free(pTrData);

	if(bRet==TRUE) return 0;

	return -5;
}

BOOL CLoginDlg::RealSend(HWND hWnd, LPVOID pData, int nLen)
{
	STTYPE400* prData = (STTYPE400*) pData;

	COPYDATASTRUCT cds;
	cds.dwData = (DWORD) TYPE400;
	cds.cbData = nLen + sizeof(DWORD);
	cds.lpData = prData;

	if(::IsWindow(hWnd)) {
		::SendMessage(hWnd, WM_COPYDATA, 0,(LPARAM)&cds);
	}

	return TRUE;
}

void CLoginDlg::OnOK() 
{
	OnConnect();	

//	CDialog::OnOK();
}

BOOL CLoginDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	STTRHEADER* prData = (STTRHEADER*)pCopyDataStruct->lpData;
	char cType = prData->TRData[0];
//	WriteDebug(prData->TRData);
	if(cType==NULL)
	{
		AfxMessageBox("������ �����߽��ϴ�.");
		EndDialog(IDCANCEL);
		return CDialog::OnCopyData(pWnd, pCopyDataStruct);
	}

	CString tmp;
	tmp.Format("%c", cType);
	int nType = atoi((LPCTSTR)tmp);

	//switch(nType)
	switch(cType)
	{
	case '0':		// ����
		{
		STLOGINSUCCESS* pData = (STLOGINSUCCESS*)prData->TRData;
		if (!CheckIPAuth(pData->RecentData))
		{
			return CDialog::OnCopyData(pWnd, pCopyDataStruct);;
		}
		MakeUserData(pData, pCopyDataStruct->dwData);
		EndDialog(IDOK);
		}
		break;

	case '1':		// ID ���� 
		::MessageBox(NULL, "��ϵ��� ���� ID �Դϴ�. �ٽ� �Է��Ͻʽÿ�\n\n��������� ����������[1588-4200] �Ǵ� ���꼾�ͷ� �����Ͻñ� �ٶ��ϴ�.", "�� �� ", MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		EndDialog(IDCANCEL);
		break;
	case '2':		// PASSWD ����
		{
			char cCount = prData->TRData[1];
			if(cCount=='0')
			{
				if(gGetProgrameType()<0)
				{
					tmp.Format("���Ӻ�й�ȣ�� �¶��� ���ǰŷ��� �������� Ȯ���ϰ�\n\
	������ ��ȣ�� ���Ͽ� 5ȸ�̻� ���� ���� �Է½� ������ �Ұ��� �����ϴ�.\n\n\
	��������� ����������[1588-4200] �Ǵ� ���꼾�ͷ� �����Ͻñ� �ٶ��ϴ�.");
				}
				else
				{
					tmp.Format("���Ӻ�й�ȣ�� �¶��� ���ǰŷ��� �������� Ȯ���ϰ�\n\
	������ ��ȣ�� ���Ͽ� 5ȸ�̻� �� �� ���� �Է½� ������ �Ұ��� �����ϴ�.\n\n\
	��������� ��������ó[1588-4200] �Ǵ� ���꼾�ͷ� �����Ͻñ� �ٶ��ϴ�.");
				}
				::MessageBox(NULL, tmp,	"��й�ȣ ����", MB_OK);
				EndDialog(IDCANCEL);
			}
			else if(cCount=='1')
			{
				char cCnt = prData->TRData[2];
				tmp.Format("���Ӻ�й�ȣ�� �¶��� ���ǰŷ��� �������� Ȯ���ϰ�\n\
������ ��ȣ�� ���Ͽ� 5ȸ�̻� ���� ���� �Է½� ������ �Ұ��� �����ϴ�.\n\n\
���Ӻ�й�ȣ�� %cȸ Ʋ�Ƚ��ϴ�. Ȯ���� �Է��Ͻñ� �ٶ��ϴ�.\n\n\
��������� ��������ó[1588-4200] �Ǵ� ���꼾�ͷ� �����Ͻñ� �ٶ��ϴ�.", cCnt);
				::MessageBox(NULL, tmp, "��й�ȣ ����", MB_OK);
			}
		}
		break;
	case '3':
		{
			tmp = "���� ������ Ȯ���Ͽ� �ֽʽÿ�";
			AfxMessageBox(tmp);
			GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		}
		break;
	case '4': // ��Ÿ ����
		EndDialog(IDCANCEL);
		break;
	case '5':	//���� ����
		tmp = "�Է��Ͻ� ���¹�ȣ�� �������� �ʽ��ϴ�.\n���¹�ȣ�� Ȯ���Ͻð� �ٽ� �Է����ֽñ� �ٶ��ϴ�";
		AfxMessageBox(tmp);
		GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		break;
	case '6':
		tmp = "�Է��Ͻ� ���º�й�ȣ�� ���� �ʽ��ϴ�.\n���º�й�ȣ�� Ȯ���Ͻð� �ٽ� �Է����ֽñ� �ٶ��ϴ�";
		AfxMessageBox(tmp);
		EndDialog(IDCANCEL);
		break;	
	case '7':		// 2003.07.09 �̳���, ��й�ȣ ������ ���� ó��
		{
/* temp
			CDlgPassPresentation dlg(m_hSocket, m_szRealID, m_szPassWd, prData->TRData);
			int nRet = dlg.DoModal();
			if (nRet == IDOK)
			{
				STLOGINSUCCESS* pData = (STLOGINSUCCESS*)prData->TRData;
				if (!CheckIPAuth(pData->RecentData))
					return CDlgRegion::OnCopyData(pWnd, pCopyDataStruct);

				MakeUserData(pData, pCopyDataStruct->dwData);
				EndDialog(IDOK);
			}
			else if (nRet == ID_RELOGIN)
			{
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PASSWD);
				pEdit->SetWindowText("");
				pEdit->SetFocus();
			}
			else
			{
				tmp = "���� ������ ��ҵǾ����ϴ�. �ٽ� �����Ͽ� �ֽʽÿ�";
				AfxMessageBox(tmp);
				EndDialog(IDCANCEL);
			}
*/
			tmp = "���� ������ ��ҵǾ����ϴ�. �ٽ� �����Ͽ� �ֽʽÿ�";
			AfxMessageBox(tmp);
			EndDialog(IDCANCEL);
		}
		break;
	case 'X':	// ���ϰ� ���Ͼ��̵�
	case 'Y':	// ���ϰ� ���̾��̵�
	case 'Z':	// ���̰� ���Ͼ��̵�
		{
/*  temp
			CString szTemp;
			szTemp.Format("%c", cType);
			CDlgWebHelper helper;
			helper.DlgWeb_DoModal("http://www.winnet.co.kr/hts/winnetpro/merge.html", "SendTR",
				m_szRealID, "", szTemp, 1, 750, 650, 1, this);
*/
		tmp = "���� ������ ��ҵǾ����ϴ�. �ٽ� �����Ͽ� �ֽʽÿ�";
		AfxMessageBox(tmp);
		EndDialog(IDCANCEL);
		}
		break;
	default:
		break;
	}
	
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}


BOOL CLoginDlg::CheckIPAuth(char* szAuth)
{
	if (m_nAuthSkip == 1)
		return TRUE;

	if (m_nSetupType == 2)
	{
		CString szClientIP;
		LRESULT lRetGetIP = ::SendMessage(m_hSocketDlg, UDM_GET_IPADDRESS, 0, (LPARAM)&szClientIP);
		CString szIP_A,szIP_B,szIP_C,szIP_D;
		int ipIndex = szClientIP.Find('.');
		szIP_A = szClientIP.Left(ipIndex);
			szClientIP = szClientIP.Mid(ipIndex+1);
			ipIndex	   = szClientIP.Find('.');
		szIP_B = szClientIP.Left(ipIndex);
			szClientIP = szClientIP.Mid(ipIndex+1);
			ipIndex	   = szClientIP.Find('.');
		szIP_C = szClientIP.Left(ipIndex);
			szClientIP = szClientIP.Mid(ipIndex+1);
			//ipIndex    = szClientIP.Find('.');
		szIP_D = szClientIP;
		
		BOOL bInnerClient;
	
		if(szIP_A == "192")
			bInnerClient = TRUE;
		else 
			bInnerClient = FALSE;

		if(bInnerClient)
		{
			// 1. ���� ip �ΰ�쿡�� ���� ip�� üũ�Ѵ�.
			CString szIP_CFirst = szIP_C.Left(1);
			int nIP_D = atoi(szIP_D); 
			if(szIP_B == "10" && szIP_CFirst == "2" && (nIP_D >= 70 && nIP_D <= 129))
			{
				//if(szLanType != "L1")
				//	return FALSE;					
				//else
					return TRUE;
			}
			else
			{
				AfxMessageBox("������� ������ IP������ ������ �˴ϴ�.\nIP�� Ȯ���Ͻðų� �������� ��ġ�Ͽ� �ֽʽÿ�.");
				return FALSE;
			}
		}
		else
		{
			//if(szLanType != "L4")
				AfxMessageBox("������� ������ IP������ ������ �˴ϴ�.\nIP�� Ȯ���Ͻðų� �������� ��ġ�Ͽ� �ֽʽÿ�.");
				return FALSE;	
		}
	}
	else if (m_nSetupType == 0)
	{
		if (m_nIDType == 1)
		{
			if (szAuth[0] == '1')
				return TRUE;
			else
			{
				AfxMessageBox("���� ����Ͻô� IP������ ���������� �����ϽǼ� �����ϴ�.\n������ �ƴϽ� ���, ������ ��ġ�Ͻñ� �ٶ��ϴ�.");
				return FALSE;
			}
		}
		else if (m_nIDType == 2)
		{
			if (szAuth[1] == '1')
				return TRUE;
			else
			{
				AfxMessageBox("���� ����Ͻô� IP������ Hts ID�� �����Ͻ� �� �����ϴ�.");
				return FALSE;
			}
		}
	}
	else if (m_nSetupType == 1)
		return TRUE;

	AfxMessageBox("� ���ѵ� �������� �ʾҽ��ϴ�.\n������ �ƴϽø� ������ ��ġ�Ͻñ� �ٶ��ϴ�.");
	return FALSE;
}


void CLoginDlg::MakeUserData(STLOGINSUCCESS* pLoginData, int nBufLen)
{
#ifdef _DEBUG
	char aIP[15+1]={0,};
	memcpy(aIP, pLoginData->IPAddr, 15);
#endif
	CString tmp;

	char  szCount[3+1] = {0,};
	memcpy(szCount, pLoginData->AccCount, 3);
	int nAcctCount = atoi(szCount);

	int nSize = nAcctCount * GEJA_SIZE;		// GEJA_SIZE : 32byte

	int nTotal = sizeof(STUSERINFO) + nSize +10;
	char*  AccList	= new char[nTotal];
	STUSERINFO* pUInfo = (STUSERINFO*)AccList;
	memset(pUInfo, 0x20, nTotal);

	memcpy(pUInfo->ID, m_szRealID, min(m_szRealID.GetLength(), 20));
	memcpy(pUInfo->Passwd, m_szPassWd, min(m_szPassWd.GetLength(), 8));
	memcpy(pUInfo->RecentDate, pLoginData->RecentData, 8);
	memcpy(pUInfo->Level, pLoginData->Level, 32);
	memcpy(pUInfo->IPAddr, pLoginData->IPAddr, 15);
	memcpy(pUInfo->Grade, pLoginData->Grade, 3);
	memcpy(pUInfo->Name, pLoginData->Name, 20);
	memcpy(pUInfo->Gigum, pLoginData->Gigum, 3);
	memcpy(pUInfo->Gigum, pLoginData->Gigum, 3);
	memcpy(pUInfo->OpenMap, pLoginData->OpenMap, 42);
	memcpy(pUInfo->AccCount, pLoginData->AccCount, 3);
	memcpy(pUInfo->AccList, pLoginData->AccList, nSize);
	memcpy((LPSTR)m_pBuffer, pUInfo, nTotal);

	delete pUInfo;

////////////{{  �������ӽ���� �ʿ�..
//	char DivPITerm = pLoginData->DivPITerm[0];
//	char szIPAddressInDB[16];
//	if(sizeof(pLoginData->IpAddress)<=sizeof(szIPAddressInDB))
//	{
//		memcpy(szIPAddressInDB,pLoginData->IpAddress,sizeof(pLoginData->IpAddress));
//	}
//	if(DivPITerm == '1'&& m_strIPAdress != szIPAddressInDB)
//	{
//		// ��������..
//		CString	str;
//		str.Format("�ٸ� �ܸ��⿡�� ������Դϴ�. ������� �ܸ����� ������ ���� ���� �����ϰڽ��ϱ�?");
//		int nResult = MessageBox(str,NULL,MB_YESNO|MB_ICONSTOP);
//		if(nResult == IDOK)
//		{
//			MultiConnectTRSend();
//		}
//		else
//		{
//		}
//	}
////////////}}

	// Login_xxx.usr�� ���� ��� �ڵ� ���� 
/*
	CString szPath;
	szPath.Format("%s%s", m_szRootPath, _T("\\User\\"));
	CString szLogin;
	//szLogin.Format("%s\\Login_%s.usr", szPath, m_szRealID);
	szLogin.Format("%s%s\\Login.usr", szPath, m_szRealID);
	CString szData;
	szData.Format("%5.5s", pLoginData->OpenMap);
	szData.TrimLeft(); szData.TrimRight();
	::WritePrivateProfileString("MAINSTART", "OPENMAP", szData, szLogin);
*/
}

void CLoginDlg::OnConnectVersion() 
{
	ShowWindow(SW_MINIMIZE);

	SetConnectServer();

	CreateSocket();		

	WriteLoginInfo();
}


void CLoginDlg::InitControl()
{
	int nType = gGetProgrameType();

	switch (nType)
	{
	case TYPE_STAFF     : SetWindowText("������ �α�");		break;
	case TYPE_CLIENT    : SetWindowText("����ڿ� �α�");	break;
	case TYPE_JANGOJOHO : SetWindowText("�ܰ���ȸ�� �α�"); break;
	case TYPE_EXTRA     : SetWindowText("��Ÿ");		    break;
	}
}

void CLoginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Get the client rect.
	CRect rcClient;

	CFont * pOldFont = NULL;

	GetClientRect( &rcClient );

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
//	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&m_LoginBackBitMap.m_bitmap);

	// copy the memory device context back into the original DC via BitBlt().
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);

	// cleanup resources.
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CLoginDlg::LoadImages()
{
	CString szImgPath = m_szRootPath + "\\Image";
	CString str;

	str = szImgPath + IMG_LOGGIN;
	m_LoginBackBitMap.LoadBMPImage(str);

	{
		CBitmap bitmap;

		m_LoginBtnImgList.DeleteImageList();
		m_LoginBtnImgList.Create(49, 22, ILC_COLORDDB, 1, 1);

		str = szImgPath + "\\Login_����_on.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_down.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_over.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_on.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_down.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_over.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_on.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_down.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_����_over.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();
	}

	m_btnSetting.FP_vSetImage(&m_LoginBtnImgList, 0, 1, -1, 2 );
	m_btnConnect.FP_vSetImage(&m_LoginBtnImgList, 3, 4, -1, 5 );
	m_btnExit.FP_vSetImage   (&m_LoginBtnImgList, 6, 7, -1, 8 );
}

void CLoginDlg::MoveCtrls()
{
	SetWindowPos(NULL, 0, 0, 574, 376, SWP_NOZORDER | SWP_NOMOVE);
	GetDlgItem(IDC_EDT_LOGINID)->SetWindowPos(NULL, 390, 126, 90, 20, SWP_NOZORDER );
	GetDlgItem(IDC_EDT_PASSWD )->SetWindowPos(NULL, 390, 148, 90, 20, SWP_NOZORDER );
	GetDlgItem(IDC_EDT_CERTPWD)->SetWindowPos(NULL, 390, 170, 90, 20, SWP_NOZORDER );

	GetDlgItem(IDC_CHK_SAVE    )->SetWindowPos(NULL, 491, 128, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	GetDlgItem(IDC_CHK_SISEONLY)->SetWindowPos(NULL, 335, 198, 0, 0, SWP_NOZORDER | SWP_NOSIZE );

	m_btnConnect.SetWindowPos(NULL,335, 231, 49, 22, SWP_NOZORDER );
	m_btnSetting.SetWindowPos(NULL,390, 231, 49, 22, SWP_NOZORDER );
	m_btnExit.SetWindowPos(NULL,445, 231, 49, 22, SWP_NOZORDER );
}


HBRUSH CLoginDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	if ( pWnd->GetDlgCtrlID() == IDC_CHK_SAVE || pWnd->GetDlgCtrlID() == IDC_CHK_SISEONLY)
		pDC->SetBkMode(TRANSPARENT);

	return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
}

UINT CLoginDlg::OnNcHitTest( CPoint point )
{
	CRect  rt;
	GetWindowRect(&rt);

	if( rt.PtInRect(point) )
	{
		return HTCAPTION;
	}

	return CDialog::OnNcHitTest(point);
}