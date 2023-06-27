// CDlg_Cloude.cpp: 구현 파일
//

#include "StdAfx.h"
#include "axis.h"
#include "CDlg_Cloude.h"
#include "afxdialogex.h"


// CDlg_Cloude 대화 상자
#define DF_DEV
#define DEV_CLOUDE_SERVER  "twas.signkorea.com"
#define REAL_CLOUDE_SERVER "cert.signkorea.com"
#define DEV_AGREEMENT_URL "https://tweb.signkorea.com:8700/notice/html/conditionsOfUse.txt"
#define REAL_AGREEMENT_URL "https://center.signkorea.com:8700/notice/html/conditionsOfUse.txt"


IMPLEMENT_DYNAMIC(CDlg_Cloude, CDialog)

CDlg_Cloude::CDlg_Cloude(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIG_CLOUDE, pParent)
{

}

CDlg_Cloude::~CDlg_Cloude()
{
}

void CDlg_Cloude::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Cloude, CDialog)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CDlg_Cloude::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_CON, &CDlg_Cloude::OnBnClickedBtnCon)
	ON_BN_CLICKED(IDC_BTN_KEYPAD, &CDlg_Cloude::OnBnClickedBtnKeypad)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlg_Cloude::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDlg_Cloude::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_CERT_ISSUE, &CDlg_Cloude::OnBnClickedBtnCertIssue)
	ON_BN_CLICKED(IDC_BTN_CERTNEW, &CDlg_Cloude::OnBnClickedBtnCertnew)
	ON_BN_CLICKED(IDC_BTN_PASSCHA, &CDlg_Cloude::OnBnClickedBtnPasscha)
	ON_BN_CLICKED(IDC_BTN_CERTDEL, &CDlg_Cloude::OnBnClickedBtnCertdel)
	ON_BN_CLICKED(IDC_BTN_CERTUP, &CDlg_Cloude::OnBnClickedBtnCertup)
	ON_BN_CLICKED(IDC_BTN_CERTDOWN, &CDlg_Cloude::OnBnClickedBtnCertdown)
	ON_BN_CLICKED(IDC_BTN_CONNTCHK, &CDlg_Cloude::OnBnClickedBtnConntchk)
	ON_BN_CLICKED(IDC_BTN_AUTOCHK, &CDlg_Cloude::OnBnClickedBtnAutochk)
	ON_BN_CLICKED(IDC_BTN_WITHDRAW, &CDlg_Cloude::OnBnClickedBtnWithdraw)
	ON_BN_CLICKED(IDC_BTN_CONCLOUDE, &CDlg_Cloude::OnBnClickedBtnConcloude)
	ON_BN_CLICKED(IDOK, &CDlg_Cloude::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlg_Cloude::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CERT_ORDER, &CDlg_Cloude::OnBnClickedBtnCertOrder)
	ON_BN_CLICKED(IDC_BTN_CERT_TRANSFER, &CDlg_Cloude::OnBnClickedBtnCertTransfer)
END_MESSAGE_MAP()


// CDlg_Cloude 메시지 처리기
BOOL CDlg_Cloude::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CloudConfig config;
	memset(&config, 0x00, sizeof(CloudConfig));
	config.SITE_CODE[0] = "U1MwMDY4XzA2";
	config.SERVER_HOST = DEV_CLOUDE_SERVER;
	config.AGREEMENT_URL = DEV_AGREEMENT_URL;
	config.CUSTOMER_ID = "SS0068";

	config.VERSION = "1.0.0";
	config.SERVER_PORT = 8500;

	//타임아웃 기간을 설정합니다 1~10초 (msec 단위)
	config.TIMEOUT_MSEC = 3000;

	sk_if_Set_CloudConfig(config);
	sk_if_DialogModalMode(m_hWnd); //모달 모드


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//로그인(선택 + 풀서명)
void CDlg_Cloude::OnBnClickedBtnLogin()  
{
	int rc = 0;
	SD_API_CONTEXT_NEW Context; //구조체 선언(클라우드 인증서 정보를 담을)
	memset(&Context, 0x00, sizeof(Context)); //구조체 선언

	memset(&m_appContext, 0, sizeof(APP_CONTEXT));
	sk_if_cert_InitContextApp(&m_appContext, NULL, (int)false); //초기화

	CString cstrDn;
	GetDlgItemText(IDC_EDIT_DN, cstrDn);

	if (cstrDn.GetLength() > 0) //인증서 Dn 필터시 사용
		strncpy(Context.sd.szUserId, cstrDn.GetBuffer(), cstrDn.GetLength());

	//sk_if_SetPolicyFilter(0, NULL); //인증서 OID 필터 기능 적용
	//sk_if_SetExipreCheckSkip(TRUE); //인증서 갱신안내 스킵옵션
	//sk_if_Set_Show_OnlyValidateCloudCert_flag(1); //클라우드 인증서 유효한것만 보이기(컴퓨터시간 기준)
	//sk_if_SetKeySaferMode(nflag); //키보드보안 모듈 연동	

	//클라우드 인증서 선택 기능
	//클라우드 인증서 선택을 성공하면 해당 구조체에
	//인증서 정보와 개인키, 임시 비밀번호가 담깁니다
	//사용 완료시까지 임시 비밀번호와 개인키 정보를 초기화 해서는 안됩니다
	//성공시 0, 실패시 0이와에 값이 반환됩니다

	//선택 옵션
	// SELECT_CLOUDCERT_OPTION_NOSEARCHLOCAL	1 - Dn 입력한 인증서 로컬저장소에서 검색하지 않겠다   
	// SELECT_CLOUDCERT_OPTION_NOCERT_OUT		4 - 클라우드에 인증서가 없으면 바로 에러 반환 (에러코드 2500)
	rc = sk_if_CloudCertSetSelectExt(&Context, selectoption);
	if (rc == 0)
	{
		MessageBox("성공", "클라우드 인증서 선택", MB_OK);
	}
	else
	{
		if (rc == -2) //-2면 연결끊기를 한것이다.
		{
			return;
		}

		int errorCode = sk_if_GetLastErrorCode();
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드 인증서 선택 오류", MB_OK);

		return;
	}

	//Context.sd.bOldStorage == 7 은 클라우드 저장소에 있는 인증서임을 표시합니다

	//이뒤로 서명
	sk_if_cert_preset_context(&m_appContext, &Context.sd);
	CString plain, strResult;
	UString p1, p2, p3;

	plain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=";

	p1.value = (unsigned char*)LPCTSTR(plain);
	p1.length = plain.GetLength();


	//p1 = 원문 , p2 결과값 , p3 = R값 결과
	memset(&p3, 0x00, sizeof(UString));
	rc = sk_if_cert_SignDataWithR(&m_appContext, "", &p1, &p2, &p3);

	if (rc) {
		MessageBox(sk_if_GetLastErrorMsg(), "전자서명 오류", MB_OK);
		return;
	}
	else
	{
		MessageBox("로그인 성공", "전자 서명", MB_OK);

	}

	memcpy(&m_Context, &Context, sizeof(SD_API_CONTEXT_NEW));


	//ShowWindow(SW_HIDE);
	//CDlg_Cloude betanetbddlg;
	//betanetbddlg.SetAppContext(&m_appContext, &Context);
	//betanetbddlg.DoModal();
	//ShowWindow(SW_SHOW);
}

void CDlg_Cloude::SetAppContext(APP_CONTEXT* appContext, SD_API_CONTEXT_NEW* Context)
{
	memcpy(&m_appContext, appContext, sizeof(APP_CONTEXT));
	memcpy(&m_Context, Context, sizeof(SD_API_CONTEXT_NEW));
	sk_if_cert_preset_context(&m_appContext, &m_Context.sd);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//설정
void CDlg_Cloude::OnBnClickedBtnCon()  
{
	CString csselect = "";
	GetDlgItemText(IDC_EDIT_DN2, csselect);

	int nselectoption = 0;

	if (csselect.GetLength() > 0)
	{
		nselectoption = _ttoi(csselect);
	}

	CString str = "";

	str.Format("선택옵션 : %d 설정 완료\n", nselectoption);

	MessageBox(str, "설정완료", MB_OK);

	selectoption = nselectoption;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//키패드 사용 설정
void CDlg_Cloude::OnBnClickedBtnKeypad()  
{
	if (keypaduse == 0)
	{
		keypaduse = 1;
		sk_if_Cloud_KeyPadUse(keypaduse); //키패드 사용 세팅
		MessageBox("간편번호 입력할 때 키패드 화면을 이용합니다.", "설정완료", MB_OK);
	}
	else
	{
		keypaduse = 0;
		sk_if_Cloud_KeyPadUse(keypaduse);
		MessageBox("간편번호 입력할 때 키패드 화면을 이용하지 않습니다.", "설정완료", MB_OK);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//설정
void CDlg_Cloude::OnBnClickedButton4()  
{
	int nkey = 0;

	CString cskey = "";
	GetDlgItemText(IDC_EDIT_DN3, cskey);

	if (cskey.GetLength() > 0)
	{
		nkey = _ttoi(cskey);
	}

	sk_if_SetKeySaferMode(nkey); //키보드보안 모듈 연동	

	CString str = "";

	str.Format("KEYSAFER : %d 설정 완료\n", nkey);

	MessageBox(str, "설정완료", MB_OK);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//종료
void CDlg_Cloude::OnBnClickedBtnExit()  
{
	EndDialog(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//인증서발급
void CDlg_Cloude::OnBnClickedBtnCertIssue()  
{
	SD_API_CONTEXT_NEW Context;
	memset(&Context, 0x00, sizeof(Context));

	int rc = 0;
	rc = sk_if_IssueCert_toCloud(&Context, 0);
	if (rc == 0)
	{
		MessageBox("성공", "클라우드 인증서 발급", MB_OK);
	}
	else
	{
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드 인증서 발급 오류", MB_OK);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//인증서갱신
void CDlg_Cloude::OnBnClickedBtnCertnew()  
{
	SD_API_CONTEXT_NEW Context;
	memset(&Context, 0x00, sizeof(Context));

	CString cstrDn;
	GetDlgItemText(IDC_EDIT_DN, cstrDn);

	if (cstrDn.GetLength() > 0)
		strncpy(Context.sd.szUserId, cstrDn.GetBuffer(), cstrDn.GetLength());

	int rc = 0;
	rc = sk_if_CertNew_toCloud(&Context, 0);
	if (rc == 0)
	{
		MessageBox("성공", "클라우드 인증서 갱신", MB_OK);
	}
	else
	{
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드 인증서 갱신 오류", MB_OK);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//간편비밀번호변경
void CDlg_Cloude::OnBnClickedBtnPasscha()  
{
	int rc = 0;
	SD_API_CONTEXT_NEW Context;
	memset(&Context, 0x00, sizeof(Context));

	CString cstrDn;
	GetDlgItemText(IDC_EDIT_DN, cstrDn);

	if (cstrDn.GetLength() > 0)
		strncpy(Context.sd.szUserId, cstrDn.GetBuffer(), cstrDn.GetLength());

	rc = sk_if_CertChangePin_inCloud(&Context, 0);
	if (rc == 0)
	{
		MessageBox("성공", "클라우드 인증서 간편비밀번호 변경", MB_OK);
	}
	else
	{
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드 인증서 간편비밀번호 변경 오류", MB_OK);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//인증서 삭제
void CDlg_Cloude::OnBnClickedBtnCertdel()  
{
	int rc = 0;
	rc = sk_if_DeleteCert_inCloud(0);
	if (rc == 0)
	{
		MessageBox("성공", "클라우드 인증서 삭제", MB_OK);
	}
	else
	{
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드 인증서 삭제 오류", MB_OK);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//인증서 올리기
void CDlg_Cloude::OnBnClickedBtnCertup()  
{
	int rc = 0;
	SD_API_CONTEXT_NEW Context;
	memset(&Context, 0x00, sizeof(Context));

	CString cstrDn;
	GetDlgItemText(IDC_EDIT_DN, cstrDn);

	if (cstrDn.GetLength() > 0)
		strncpy(Context.sd.szUserId, cstrDn.GetBuffer(), cstrDn.GetLength());

	rc = sk_if_UploadPCtoCloud(&Context, 0);
	if (rc == 0)
	{
		MessageBox("성공", "클라우드로 인증서 올리기", MB_OK);
	}
	else
	{
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드로 인증서 올리기 오류", MB_OK);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//인증서 내려받기
void CDlg_Cloude::OnBnClickedBtnCertdown()  
{
	int rc = 0;
	rc = sk_if_DownloadCloudtoPC(0);
	if (rc == 0)
	{
		MessageBox("성공", "클라우드에서 인증서리 내려받기", MB_OK);
	}
	else
	{
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드에서 인증서리 내려받기 오류", MB_OK);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//연결확인
void CDlg_Cloude::OnBnClickedBtnConntchk()  
{
	int rc = 0;
	rc = sk_if_Connected_CloudUser_Confirm(0); //1 : 화면을 보지않고 연결확인 된 것만 알겠다
	if (rc == 1)
	{
		MessageBox("클라우드 연결 끊기 성공", "클라우드 연결 끊기", MB_OK);
	}

	if (rc < 0)
	{
		MessageBox(sk_if_GetLastErrorMsg(), "클라우드에 연결된 정보 확인", MB_OK);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//자동연결된 기기 조회
void CDlg_Cloude::OnBnClickedBtnAutochk()  
{
	int rc = 0;
	rc = sk_if_Cloud_AutoConnected_Device(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //회원탈퇴
void CDlg_Cloude::OnBnClickedBtnWithdraw() 
{
	int rc = 0;
	rc = sk_if_CloudUser_DeleteAccount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //설정
void CDlg_Cloude::OnBnClickedBtnConcloude() 
{
	CString cscmpip = "";
	GetDlgItemText(IDC_EDIT_CMPIP, cscmpip);

	char ip[300];
	memset(ip, 0x00, sizeof(ip));

	if (cscmpip.GetLength() <= 0)
		return;

	strncpy(ip, cscmpip.GetBuffer(), cscmpip.GetLength());

	CString csport = "";
	GetDlgItemText(IDC_EDIT_CMPPORT, csport);

	int nport = 4099;

	if (csport.GetLength() > 0)
	{
		nport = _ttoi(csport);
	}

	//테스트 cmp 세팅
	sk_if_CMP_SetCmpConfig(ip, nport);


	CString str = "";

	str.Format("CMP IP : %s\nCMP PORT : %d 설정 완료\n", ip, nport);

	MessageBox(str, "설정완료", MB_OK);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDlg_Cloude::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDlg_Cloude::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}





void CDlg_Cloude::OnBnClickedBtnCertOrder()
{
	int rc = 0;
	CString plain, strResult;
	UString p1, p2, p3;

	plain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=";

	p1.value = (unsigned char*)LPCTSTR(plain);
	p1.length = plain.GetLength();

	//p1 = 원문 , p2 결과값 , p3 = R값 결과
	memset(&p2, 0x00, sizeof(UString));
	memset(&p3, 0x00, sizeof(UString));

	//클라우드 저장소에 저장된 인증서인데 임시 비밀번호가 없다면
	//다시 받는다
	if (m_Context.sd.bOldStorage == 7 &&
		(m_Context.sd.szOldPasswd[0] == NULL ||
			strlen(m_Context.sd.szOldPasswd) <= 0))
	{
		SD_API_CONTEXT_NEW tContext; //구조체 선언(클라우드 인증서 정보를 담을)
		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));

		memcpy(&tContext, &m_Context, sizeof(SD_API_CONTEXT_NEW));
		memset(&tContext.sd.szOldPasswd, 0x00, sizeof(tContext.sd.szOldPasswd));

		rc = sk_if_CloudCertSetSelectExt(&tContext, 1);
		if (rc == 0)
		{
			memset(&m_Context, 0x00, sizeof(SD_API_CONTEXT_NEW));
			memcpy(&m_Context, &tContext, sizeof(SD_API_CONTEXT_NEW));
			sk_if_cert_preset_context(&m_appContext, &m_Context.sd);
		}
		else
		{
			int errorCode = sk_if_GetLastErrorCode();
			MessageBox(sk_if_GetLastErrorMsg(), "축약서명 오류.", MB_OK);
		}

		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	}

	if (sk_if_cert_SignData_notEncode(&m_appContext, "", &p1, &p2, NULL))
	{
		MessageBox("sk_if_GetLastErrorMsg()", "축약서명 오류..", MB_OK);
		return;
	}
	else
	{
		MessageBox("주문 성공", "축약 서명", MB_OK);
	}
}


void CDlg_Cloude::OnBnClickedBtnCertTransfer()
{
	SD_API_CONTEXT_NEW tContext; //구조체 선언(클라우드 인증서 정보를 담을)
	memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	int rc = 0;

	CString plain, strResult;
	UString p1, p2, p3;

	//간편비밀번호를 다시 입력 받고 싶다면
	if (m_Context.sd.bOldStorage == 7)
	{
		memcpy(&tContext, &m_Context, sizeof(SD_API_CONTEXT_NEW));
		memset(&tContext.sd.szOldPasswd, 0x00, sizeof(tContext.sd.szOldPasswd)); //비밀번호 부분만 초기화

		rc = sk_if_CloudCertSetSelectExt(&tContext, 1);

		if (rc == 0)
		{
			memset(&m_Context, 0x00, sizeof(SD_API_CONTEXT_NEW));
			memcpy(&m_Context, &tContext, sizeof(SD_API_CONTEXT_NEW));
			sk_if_cert_preset_context(&m_appContext, &m_Context.sd);

		}
		else
		{
			int errorCode = sk_if_GetLastErrorCode();
			MessageBox(sk_if_GetLastErrorMsg(), "전자 서명 오류", MB_OK);

			return;
		}
		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	}

	plain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=";

	p1.value = (unsigned char*)LPCTSTR(plain);
	p1.length = plain.GetLength();

	//p1 = 원문 , p2 결과값 , p3 = R값 결과
	memset(&p3, 0x00, sizeof(UString));
	rc = sk_if_cert_SignDataWithR(&m_appContext, "", &p1, &p2, &p3);

	if (rc) {
		MessageBox(sk_if_GetLastErrorMsg(), "전자서명 오류", MB_OK);
		return;
	}
	else
	{
		MessageBox("계좌이체 성공", "전자 서명", MB_OK);

	}
}
