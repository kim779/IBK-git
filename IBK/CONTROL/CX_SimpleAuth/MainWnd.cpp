// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_SimpleAuth.h"
#include "MainWnd.h"
#include "SimpleAuth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
#define DF_OTPSTATE_SEARCH_OTP	1
#define DF_OTPSTATE_CERT_OTP	2

CMainWnd::CMainWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_iType = DF_TYPE_LOGIN;
	m_oTPState = DF_OTPSTATE_SEARCH_OTP;
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
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_PROPERTY_NOTIFY(CMainWnd, "ERRcode", m_eRRcode, OnERRcodeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPsuccYN", m_oTPsuccYN, OnOTPsuccYNChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPMaxErrCnt", m_oTPMaxErrCnt, OnOTPMaxErrCntChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPErrCnt", m_oTPErrCnt, OnOTPErrCntChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPState", m_oTPState, OnOTPStateChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CMainWnd, "OTPSeqNo", m_oTPSeqNo, OnOTPSeqNoChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CMainWnd, "ERRMsg", m_eRRMsg, OnERRMsgChanged, VT_BSTR)
	DISP_FUNCTION(CMainWnd, "CertOTP", CertOTP, VT_I2, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "SearchOTP", dispidSearchOTP, SearchOTP, VT_I2, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C7DA29C9-D7D4-4AAD-A595-A42BDDDB0AD0}
static const IID IID_IMainWnd =
{ 0xc7da29c9, 0xd7d4, 0x4aad, { 0xa5, 0x95, 0xa4, 0x2b, 0xdd, 0xdb, 0xa, 0xd0 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

short CMainWnd::CertOTP(LPCTSTR sCustNum, LPCTSTR spinNum) 
{
	// TODO: Add your dispatch handler code here
	m_strPinNumber = CString(spinNum);
	m_strPinNumber.TrimRight();

	CString sCnum;
	sCnum.Format("%s", sCustNum);
	SearchOTP(sCnum.AllocSysString());
	return 0;
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
			m_eRRcode = CString(exth->data, 6);
			m_eRRcode.TrimRight();
			if (m_eRRcode == "000000")
			{
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
			}
			else
			{
				m_eRRMsg = CString(exth->data, 80);
				m_eRRMsg.TrimRight();

				tagOTP_RES_CERTREG* pout = (tagOTP_RES_CERTREG*)&exth->data[80];
				m_oTPMaxErrCnt = CString(pout->OTP_VRFC_MAX_EROR_ALWN_CNT);	m_oTPMaxErrCnt.TrimRight();
				m_oTPErrCnt = CString(pout->OTP_VRFC_EROR_CNT);	m_oTPErrCnt.TrimRight();
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
			}
			break;

		case TRKEY_OPTSEARCH:
			tagOTP_RES_SEARCH* pout;
			pout = (tagOTP_RES_SEARCH*)&exth->data[80];

			m_eRRcode = CString(exth->data, 6);
			m_eRRcode.TrimRight();
			if (m_eRRcode == "000000")
			{
				m_oTPSeqNo = CString(pout->OTP_SEQ_NO);	m_oTPSeqNo.TrimRight();		//OTP�Ϸù�ȣ  //m_strClientNum
				m_strClientNum = CString(pout->CUST_NO); m_strClientNum.TrimRight();	//����ȣ
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
				CertOTP_BP(m_strClientNum, m_strPinNumber);
			}
			else
			{
				m_eRRMsg = CString(exth->data, 80);
				m_eRRMsg.TrimRight();

				m_eRRcode = CString(pout->ERR_CD);	m_eRRcode.TrimRight();
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

			//	if (len >= 80 + SZ_tagOTP_RES_CERTREG) //OTP����
			if (m_oTPState == DF_OTPSTATE_CERT_OTP)
			{
				m_eRRcode = CString(pData, 6);
				m_eRRcode.TrimRight();
				if (m_eRRcode == "000000")
				{
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
				}
				else
				{
					m_eRRMsg = CString(pData, 80);
					m_eRRMsg.TrimRight();

					tagOTP_RES_CERTREG* pout = (tagOTP_RES_CERTREG*)&pData[80];
					m_oTPMaxErrCnt = CString(pout->OTP_VRFC_MAX_EROR_ALWN_CNT); m_oTPMaxErrCnt.TrimRight();
					m_oTPErrCnt = CString(pout->OTP_VRFC_EROR_CNT); m_oTPErrCnt.TrimRight();
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)m_param.name.GetString());
				}
			}
			else	//OTP��ȸ
			{
				m_eRRcode = CString(pData, 6);
				m_eRRcode.TrimRight();

				tagOTP_RES_SEARCH* pout = (tagOTP_RES_SEARCH*)&pData[80];

				if (m_eRRcode == "000000")
				{
					m_oTPSeqNo = CString(pout->OTP_SEQ_NO); m_oTPSeqNo.TrimRight();		//OTP�Ϸù�ȣ  //m_strClientNum
					m_strClientNum = CString(pout->CUST_NO); m_strClientNum.TrimRight();	//����ȣ
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
					CertOTP_BP(m_strClientNum, m_strPinNumber);
				}
				else
				{
					m_eRRMsg = CString(pData, 80);
					m_eRRMsg.TrimRight();

					m_eRRcode = CString(pout->ERR_CD); m_eRRcode.TrimRight();
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

void CMainWnd::CertOTP_BP(CString sCustNum, CString spinNum)	// spinNum : ����ڰ� �Է��� �ŷ���ȣ
{
	m_eRRcode.Empty();
	m_oTPsuccYN.Empty();
	m_oTPMaxErrCnt.Empty();
	m_oTPErrCnt.Empty();

	m_oTPState = DF_OTPSTATE_CERT_OTP;
	GetMacAddr();

	const	int	datl = SZ_COMHEADER + SZ_tagOTP_CERTREG;
	char	datb[datl + 1]{};
	FillMemory(datb, datl, ' ');

	// st_comheader
	struct st_comheader* pst_header = (struct st_comheader*)datb;
	memcpy(pst_header->msgid, "0208", 4);
	memcpy(pst_header->uvalue, (LPSTR)(LPCTSTR)m_macaddr, sizeof(pst_header->uvalue));


	// tagOTP_CERTREG
	struct tagOTP_CERTREG* pst_midatca = (struct tagOTP_CERTREG*)&datb[SZ_COMHEADER];
	memcpy(pst_midatca->RQST_CHNL_DCD, "HTS", 3);
	memcpy(pst_midatca->MSB_VRSN_VL, "*****", 5);				//���̺귯�� ��������
	memcpy(pst_midatca->CUST_NO, sCustNum, sCustNum.GetLength());		//����ȣ
	memcpy(pst_midatca->OTP_SEQ_NO, m_oTPSeqNo, m_oTPSeqNo.GetLength());	//otp�Ϸù�ȣ  
	memcpy(pst_midatca->OTP_TRAN_NO, spinNum, spinNum.GetLength());		//otp�ŷ���ȣ (����Ͽ��� ��ȸ�ؼ� �����ش�) //�����϶��� �հ����Է�	
	memcpy(pst_midatca->PRE_TRAN_USE_YN, "N", 1);				//�����ŷ���뿩��
	memcpy(pst_midatca->OTP_CLI_VL, spinNum, spinNum.GetLength());		//���Է�OTP��

	SendTR(_T("piboatca"), datb, datl, (BYTE)TRKEY_OPTCERT);
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

void CMainWnd::SetParam(_param *pParam)
{
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
	
	if(m_param.options == "/k1")
		m_iType = DF_TYPE_TRD; //�����ŷ�
	else if(m_param.options == "/k2")
		m_iType = DF_TYPE_OTP; //OTP ����
	else if(m_param.options == "/k3")
		m_iType = DF_TYPE_AUTO_OTP; //OTP �ڵ�
	
	//	m_DlgQr.m_iType = m_iType;
}

LPCTSTR CMainWnd::Variant( int cmd, LPCTSTR data )
{
	return (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_root = Variant(homeCC, "");
	return 0;
}


SHORT CMainWnd::SearchOTP(BSTR custnum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ���⿡ ����ġ ó���� �ڵ带 �߰��մϴ�.
	CString custnumber = CString(custnum);
	custnumber.TrimRight();

	m_oTPState = DF_OTPSTATE_SEARCH_OTP;
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
	memcpy(pst_midatca->CUST_NO, custnumber, custnumber.GetLength());	//����ȣ

	return SendTR(_T("piboatca"), datb, datl, (BYTE)TRKEY_OPTSEARCH);
}
