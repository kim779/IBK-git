// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_SimpleEasy.h"
#include "MainWnd.h"
#include "socket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
#define	TM_CONNECT	9875
#define	TM_RECEIVE	9874

#define DF_OTPSTATE_SEARCH_OTP	1
#define DF_OTPSTATE_CERT_OTP	2

CMainWnd::CMainWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_sock = nullptr;
	m_iType = DF_TYPE_LOGIN;
	m_otpState = DF_OTPSTATE_SEARCH_OTP;
}

CMainWnd::~CMainWnd()
{
	
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 12, OnSockManage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_PROPERTY_NOTIFY(CMainWnd, "ERRcode", m_errCode, OnERRcodeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPsuccYN", m_otpSuccYN, OnOTPsuccYNChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPMaxErrCnt", m_otpMaxErrCnt, OnOTPMaxErrCntChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPErrCnt", m_otpErrCnt, OnOTPErrCntChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPState", m_otpState, OnOTPStateChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPSeqNo", m_otpSeqNo, OnOTPSeqNoChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "ERRMsg", m_errMsg, OnERRMsgChanged, VT_BSTR)
	DISP_FUNCTION(CMainWnd, "SetIPandPORT", SetIPandPORT, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "ShowQRcode", ShowQRcode, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "testmethod", testmethod, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "CertOTP", CertOTP, VT_I2, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetCustNumber", GetCustNumber, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SearchOTP", SearchOTP, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "CertOTPMP", CertOTPMP, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {E046FA1A-7D7E-48A9-88D9-DCF317AC1C76}
static const IID IID_IMainWnd =
{ 0xe046fa1a, 0x7d7e, 0x48a9, { 0x88, 0xd9, 0xdc, 0xf3, 0x17, 0xac, 0x1c, 0x76 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_root = Variant(homeCC, "");

	if (m_sock)
		m_sock.reset();

//	m_sock = std::make_unique<Csocket>(this);

	return 0;
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rec;

	GetClientRect(rec);
	dc.FillSolidRect(rec, RGB(255,255,255));
	
	// Do not call CWnd::OnPaint() for painting messages
}


void CMainWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

//	m_pathPhone = dlg.GetPathName();

	CWnd::OnLButtonUp(nFlags, point);
}

void CMainWnd::SetIPandPORT(LPCTSTR ip, LPCTSTR port) 
{
	// TODO: Add your dispatch handler code here

}

void CMainWnd::ShowQRcode()
{
	// TODO: Add your dispatch handler code here
	m_DlgQr.m_iType = m_iType;
	m_DlgQr.m_strpath = m_root;
	m_DlgQr.DoModal();
}

LPCTSTR CMainWnd::Variant(int cmd, LPCTSTR data)
{
	return (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

void CMainWnd::SetParam(_param *pParam)
{
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	m_param.options = CString(pParam->options, pParam->options.GetLength());

	if (m_param.options == "/k1")
		m_iType = DF_TYPE_TRD;		//금융거래
	else if (m_param.options == "/k2")
		m_iType = DF_TYPE_OTP;		//OTP 수동
	else if (m_param.options == "/k3")
		m_iType = DF_TYPE_AUTO_OTP;	//OTP 자동

//	m_DlgQr.m_iType = m_iType;
}

void CMainWnd::testmethod(LPCTSTR sdata) 
{
	// TODO: Add your dispatch handler code here

}

CString CMainWnd::GetMacAddr()
{
	IP_ADAPTER_INFO AdapterInfo[16]{};
	DWORD	dwBufLen = sizeof(AdapterInfo);
	const	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);

	m_macaddr.Empty();
	if (dwStatus == ERROR_SUCCESS)
	{
		m_macaddr.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
			AdapterInfo[0].Address[0], 
			AdapterInfo[0].Address[1], 
			AdapterInfo[0].Address[2], 
			AdapterInfo[0].Address[3], 
			AdapterInfo[0].Address[4], 
			AdapterInfo[0].Address[5]);
	}
	return m_macaddr;
}

void CMainWnd::SearchOTP_MP(CString sUserNum)
{
	sUserNum.TrimRight();

	m_otpState = DF_OTPSTATE_SEARCH_OTP;
	m_errCode.Empty();
	GetMacAddr();

	if (m_sock == nullptr)
	{
		AfxMessageBox("socket close. 화면 다시 실행해주세요");
		return;
	}

	CString	tmps;
	const	int	datl = L_fmH + L_axisH + SZ_COMHEADER + SZ_tagOTP_SEARCH;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);
	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	struct st_comheader* pst_header = (struct st_comheader*)&datb[L_fmH + L_axisH];
	struct tagOTP_SEARCH* pst_midatca = (struct tagOTP_SEARCH*) & datb[L_fmH + L_axisH + SZ_COMHEADER];

	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	tmps.Format("%05d", L_axisH + SZ_COMHEADER + SZ_tagOTP_SEARCH);
	CopyMemory(fmH->datL, tmps, sizeof(fmH->datL));

	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = TRKEY_OPTSEARCH;
	CopyMemory(axisH->trxC, "piboatca", sizeof(axisH->trxC));
	tmps.Format("%05d", SZ_COMHEADER + SZ_tagOTP_SEARCH);
	CopyMemory(axisH->datL, tmps, sizeof(axisH->datL));

	// st_comheader
	FillMemory(pst_header, SZ_COMHEADER, ' ');
	CopyMemory(pst_header->msgid, "0203", 4);
	CopyMemory(pst_header->uvalue, m_macaddr, sizeof(pst_header->uvalue));
	
	//  tagOTP_SEARCH
	FillMemory(pst_midatca, SZ_tagOTP_SEARCH, ' ');
	CopyMemory(pst_midatca->RQST_CHNL_DCD, "HTS", 3);
	CopyMemory(pst_midatca->CUST_NO, sUserNum, sUserNum.GetLength());					//고객번호

	if (m_sock->Write(datb.get(), datl))
		SetTimer(TM_RECEIVE, 2000, NULL);

	datb.reset();
}

void CMainWnd::CertOTPMP(LPCTSTR sCustNum, LPCTSTR sPinNum) 
{
	CString stmp = CString(sCustNum);
	m_strPinNumber = CString(sPinNum);
	SearchOTP_MP(stmp);   //고객번호로 OTP일련번호조회
}

//맵에서 호출하지 않는 함수이다
void CMainWnd::CertOTP_MP(CString sOTPNum, CString strCOTPNum)  //OTP일련번호 , 고객에게 입력받은 넘버
{
	m_otpState = DF_OTPSTATE_CERT_OTP;
	m_errCode.Empty();
	m_otpSuccYN.Empty();
	m_otpMaxErrCnt.Empty();
	m_otpErrCnt.Empty();

	GetMacAddr();
	if (m_sock == nullptr)
	{
		AfxMessageBox("socket close. 화면 다시 실행해주세요");
		return;
	}

	CString	tmps;
	const	int	datl = L_fmH + L_axisH + SZ_COMHEADER + SZ_tagOTP_CERTREG;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);
	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	struct st_comheader* pst_header = (struct st_comheader*)&datb[L_fmH + L_axisH];
	struct tagOTP_CERTREG* pst_midatca = (struct tagOTP_CERTREG*)&datb[L_fmH + L_axisH + SZ_COMHEADER];

	// st_comheader
	FillMemory(pst_header, ' ', sizeof(struct st_comheader));
	CopyMemory(pst_header->msgid, "0208", 4);
	CopyMemory(pst_header->uvalue, m_macaddr, sizeof(pst_header->uvalue));

	// tagOTP_CERTREG
	FillMemory(pst_midatca, SZ_tagOTP_CERTREG, ' ');
	CopyMemory(pst_midatca->RQST_CHNL_DCD, "HTS", 3);
	CopyMemory(pst_midatca->MSB_VRSN_VL, "*****", 5);				//라이브러리 버전정보
//	CopyMemory(pst_midatca->MSB_VRSN_VL, "25c0f526b71a4ea22", 17);			//라이브러리 버전정보

	CopyMemory(pst_midatca->CUST_NO, m_strClientNum, m_strClientNum.GetLength());	//고객번호
	CopyMemory(pst_midatca->OTP_SEQ_NO, sOTPNum, sOTPNum.GetLength());		//otp일련번호  
	CopyMemory(pst_midatca->OTP_TRAN_NO, strCOTPNum, strCOTPNum.GetLength());	//otp거래번호 
	CopyMemory(pst_midatca->PRE_TRAN_USE_YN, "N", 1);				//사전거래사용여부
	CopyMemory(pst_midatca->OTP_CLI_VL, strCOTPNum, strCOTPNum.GetLength());	//고객입력OTP값 (모바일에서 조회해서 보여준다) //수동일때는 손가락입력	

	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	tmps.Format("%05d", L_axisH + SZ_COMHEADER + SZ_tagOTP_CERTREG);
	CopyMemory(fmH->datL, tmps, sizeof(fmH->datL));

	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = TRKEY_OPTCERT;
	CopyMemory(axisH->trxC, "pmhoatca", sizeof(axisH->trxC));
	tmps.Format("%05d", SZ_COMHEADER + SZ_tagOTP_CERTREG);
	CopyMemory(axisH->datL, tmps, sizeof(axisH->datL));

	if (m_sock->Write(datb.get(), datl))
		SetTimer(TM_RECEIVE, 2000, NULL);

	datb.reset();
}

//MAP 호출
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
short CMainWnd::SearchOTP(LPCTSTR custnum)
{
	CString custnumber = CString(custnum);
	custnumber.TrimRight();

	m_otpState = DF_OTPSTATE_SEARCH_OTP;
	GetMacAddr();

	const	int	datl = SZ_COMHEADER + SZ_tagOTP_SEARCH;
	char	datb[datl + 1]{};
	FillMemory(datb, datl, ' ');

	// st_comheader
	struct st_comheader* pst_header = (struct st_comheader*)datb;
	memcpy(pst_header->msgid, "0203", 4);
	memcpy(pst_header->uvalue, m_macaddr, sizeof(pst_header->uvalue));

	// tagOTP_SEARCH
	struct tagOTP_SEARCH* pst_midatca = (struct tagOTP_SEARCH*)&datb[SZ_COMHEADER];
	memcpy(pst_midatca->RQST_CHNL_DCD, "HTS", 3);
	memcpy(pst_midatca->CUST_NO, custnumber, custnumber.GetLength());	//고객번호

	return SendTR(_T("piboatca"), datb, datl, (BYTE)TRKEY_OPTSEARCH);
}

short CMainWnd::CertOTP(LPCTSTR sCustNum, LPCTSTR spinNum)   //고객번호 , 고객인입력한pnumber
{
	m_strPinNumber = CString(spinNum);
	m_strPinNumber.TrimRight();
	SearchOTP(sCustNum);
	
	return 1;
}

void CMainWnd::CertOTP_BP(CString sCustNum, CString spinNum)	// spinNum : 사용자가 입력한 거래번호
{
	m_errCode.Empty();
	m_otpSuccYN.Empty();
	m_otpMaxErrCnt.Empty();
	m_otpErrCnt.Empty();

	m_otpState = DF_OTPSTATE_CERT_OTP;
	GetMacAddr();

	const	int	datl = SZ_COMHEADER + SZ_tagOTP_CERTREG;
	char	datb[datl + 1]{};
	FillMemory(datb, datl, ' ');

	// st_comheader
	struct st_comheader* pst_header = (struct st_comheader*)datb;
	memcpy(pst_header->msgid, "0208", 4);
	memcpy(pst_header->uvalue, (LPSTR)(LPCTSTR)m_macaddr, sizeof(pst_header->uvalue));

	// tagOTP_CERTREG
	struct tagOTP_CERTREG* pst_midatca = (struct tagOTP_CERTREG*)datb[SZ_COMHEADER];
	memcpy(pst_midatca->RQST_CHNL_DCD, "HTS", 3);
	memcpy(pst_midatca->MSB_VRSN_VL, "*****", 5);				//라이브러리 버전정보
	memcpy(pst_midatca->CUST_NO, sCustNum, sCustNum.GetLength());		//고객번호
	memcpy(pst_midatca->OTP_SEQ_NO, m_otpSeqNo, m_otpSeqNo.GetLength());	//otp일련번호  
	memcpy(pst_midatca->OTP_TRAN_NO, spinNum, spinNum.GetLength());		//otp거래번호 (모바일에서 조회해서 보여준다) //수동일때는 손가락입력	
	memcpy(pst_midatca->PRE_TRAN_USE_YN, "N", 1);				//사전거래사용여부
	memcpy(pst_midatca->OTP_CLI_VL, spinNum, spinNum.GetLength());		//고객입력OTP값

	SendTR(_T("piboatca"), datb, datl, (BYTE)TRKEY_OPTCERT);
}

BOOL CMainWnd::SendTR(CString strName, char* pData, int nData, BYTE key)
{
	int	idx = 0;
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(L_userTH + nData + 128);

	// control key & name
	pWb[idx++] = (char)key;
	CopyMemory(&pWb[idx], m_param.name, m_param.name.GetLength());
	idx += m_param.name.GetLength();
	pWb[idx++] = '\t';

	// userTH
	struct	_userTH* udat = (struct _userTH*)&pWb[idx];
	CopyMemory(udat->trc, strName, sizeof(udat->trc));
	udat->stat = US_KEY;
	udat->key = m_param.key;
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], pData, nData);
	const	int	ret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nData), (LPARAM)pWb.get());
	pWb.reset();
	return ret;
}

LRESULT CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			struct	_extTHx* exth = (struct _extTHx*)lParam;
			switch (exth->key)
			{
			case TRKEY_OPTCERT:
				m_errCode = CString(exth->data, 6);
				m_errCode.TrimRight();
				if (m_errCode == "000000")
				{
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
				}
				else
				{
					m_errMsg = CString(exth->data, 80);
					m_errMsg.TrimRight();

					tagOTP_RES_CERTREG* pout = (tagOTP_RES_CERTREG*)&exth->data[80];
					m_otpMaxErrCnt = CString(pout->OTP_VRFC_MAX_EROR_ALWN_CNT);	m_otpMaxErrCnt.TrimRight();
					m_otpErrCnt = CString(pout->OTP_VRFC_EROR_CNT);	m_otpErrCnt.TrimRight();
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
				}
				break;

			case TRKEY_OPTSEARCH:
				tagOTP_RES_SEARCH* pout;
				pout = (tagOTP_RES_SEARCH*)&exth->data[80];

				m_errCode = CString(exth->data, 6);
				m_errCode.TrimRight();
				if (m_errCode == "000000")
				{
					m_otpSeqNo = CString(pout->OTP_SEQ_NO);	m_otpSeqNo.TrimRight();		//OTP일련번호  //m_strClientNum
					m_strClientNum = CString(pout->CUST_NO); m_strClientNum.TrimRight();	//고객번호
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
					CertOTP_BP(m_strClientNum, m_strPinNumber);
				}
				else
				{
					m_errMsg = CString(exth->data, 80);
					m_errMsg.TrimRight();

					m_errCode = CString(pout->ERR_CD);	m_errCode.TrimRight();
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
				}

			default:break;
			}
		}
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		{
			const	int	key = HIBYTE(LOWORD(wParam));
			const	int	len = HIWORD(wParam);
			if (len > 0)
			{
				char* pData = (char*)lParam;

				//	if (len >= 80 + SZ_tagOTP_RES_CERTREG) //OTP검증
				if (m_otpState == DF_OTPSTATE_CERT_OTP)
				{
					m_errCode = CString(pData, 6);
					m_errCode.TrimRight();
					if (m_errCode == "000000")
					{
						m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
					}
					else
					{
						m_errMsg = CString(pData, 80);
						m_errMsg.TrimRight();

						tagOTP_RES_CERTREG* pout = (tagOTP_RES_CERTREG*)&pData[80];
						m_otpMaxErrCnt = CString(pout->OTP_VRFC_MAX_EROR_ALWN_CNT); m_otpMaxErrCnt.TrimRight();
						m_otpErrCnt = CString(pout->OTP_VRFC_EROR_CNT); m_otpErrCnt.TrimRight();
						m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
					}
				}
				else	//OTP조회
				{
					m_errCode = CString(pData, 6);
					m_errCode.TrimRight();

					tagOTP_RES_SEARCH* pout = (tagOTP_RES_SEARCH*)&pData[80];

					if (m_errCode == "000000")
					{
						m_otpSeqNo = CString(pout->OTP_SEQ_NO); m_otpSeqNo.TrimRight();		//OTP일련번호  //m_strClientNum
						m_strClientNum = CString(pout->CUST_NO); m_strClientNum.TrimRight();	//고객번호
						m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
						CertOTP_BP(m_strClientNum, m_strPinNumber);
					}
					else
					{
						m_errMsg = CString(pData, 80);
						m_errMsg.TrimRight();

						m_errCode = CString(pout->ERR_CD); m_errCode.TrimRight();
						m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
					}
				}
			}
		}
		break;

	case DLL_INIT:
	case DLL_INB:
	case DLL_ALERT:
	case DLL_TRIGGER:
	case DLL_DOMINO:
	case DLL_SETPAL:
	case DLL_GUIDE:
	case DLL_SETFONT:
	default:
		break;
	}

	return FALSE;
}

BSTR CMainWnd::GetCustNumber()
{
	CString strResult;
	CWinApp* app = AfxGetApp();

	strResult = CString((LPCTSTR)(app->GetMainWnd())->SendMessage(WM_USER, 0x08));
	return strResult.AllocSysString();
}

void CMainWnd::dispatch(char* pBytes, int nBytes)
{
	struct	_axisH* axisH = nullptr;

	pBytes += L_fmH;
	nBytes -= L_fmH;

	axisH = (struct _axisH*)pBytes;
	pBytes += L_axisH;
	nBytes -= L_axisH;
	nBytes = min(nBytes, atoi(CString(axisH->datL, sizeof(axisH->datL))));

	switch (axisH->trxK)
	{
	case TRKEY_OPTSEARCH:   //OTP조회
		{
			tagOTP_RES_SEARCH* pout = (tagOTP_RES_SEARCH*)&pBytes[80];
			m_errCode = CString(pout->ERR_CD); m_errCode.TrimRight();
			if (m_errCode == "000000")
			{
				m_otpSeqNo = CString(pout->OTP_SEQ_NO); m_otpSeqNo.TrimRight();		//OTP일련번호  //m_strClientNum
				m_strClientNum = CString(pout->CUST_NO); m_strClientNum.TrimRight();	//고객번호
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
				CertOTP_MP(m_otpSeqNo, m_strPinNumber);
			}
			else
			{
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
			}
		}
		break;

	case TRKEY_OPTCERT:  //OTP검증
		{
			tagOTP_RES_CERTREG* pout = (tagOTP_RES_CERTREG*)&pBytes[80];
			m_errCode = CString(pout->ERR_CD); m_errCode.TrimRight();
			m_otpSuccYN = CString(pout->OTP_VRFC_SUCC_YN); m_otpSuccYN.TrimRight();

			if (m_errCode == "000000")
			{
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
			}
			else
			{
				m_errMsg = CString(pBytes, 80); m_errMsg.TrimRight();
				m_otpMaxErrCnt = CString(pout->OTP_VRFC_MAX_EROR_ALWN_CNT); m_otpMaxErrCnt.TrimRight();
				m_otpErrCnt = CString(pout->OTP_VRFC_EROR_CNT); m_otpErrCnt.TrimRight();
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
			}
		}
		break;

	case DF_ST_SEARCHTRDNUM:  //인증거래번호조회 
	case DF_ST_VEFYLOGIN:  //로그인거래검증 
	case DF_ST_VEFYTRADE:  //금융거래검증 
	default:
		break;
	}
}

LONG CMainWnd::OnSockManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case sm_CLOSE:
		if (m_sock)
		{
			m_sock.reset();
			m_sock = std::make_unique<Csocket>(this);
			if (m_sock->Open(m_sIP, atoi(m_sPort)))
				SetTimer(TM_CONNECT, 2000, NULL);
		}
		break;

	case sm_CONNECT:
		KillTimer(TM_CONNECT);
		break;

	case sm_RECEIVE:
		KillTimer(TM_CONNECT);
		KillTimer(TM_RECEIVE);
		dispatch((char*)lParam, (int)HIWORD(wParam));
		break;
	}

	return 0;
}

void CMainWnd::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case TM_CONNECT:
		KillTimer(nIDEvent);
		if (m_sock)
			m_sock.reset();
		break;

	case TM_RECEIVE:
		KillTimer(nIDEvent);
		if (m_sock)
			m_sock.reset();
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CMainWnd::OnDestroy()
{
	CWnd::OnDestroy();

	if (m_sock)
		m_sock.reset();
}

void CMainWnd::OnERRcodeChanged()
{
	// TODO: Add notification handler code

}

void CMainWnd::OnOTPsuccYNChanged()
{
	// TODO: Add notification handler code

}

void CMainWnd::OnOTPMaxErrCntChanged()
{
	// TODO: Add notification handler code

}

void CMainWnd::OnOTPErrCntChanged()
{
	// TODO: Add notification handler code

}

void CMainWnd::OnOTPStateChanged()
{
	// TODO: Add notification handler code

}

void CMainWnd::OnOTPSeqNoChanged()
{
	// TODO: Add notification handler code

}

void CMainWnd::OnERRMsgChanged()
{
	// TODO: Add notification handler code

}