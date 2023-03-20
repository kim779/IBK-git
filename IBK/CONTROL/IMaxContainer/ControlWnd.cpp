// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IMAXContainer.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Event from IMAXApi
//
#define WUM_IMAXCODECHANGE		WM_USER + 3002
#define WUM_IMAXORDERINFO		WM_USER + 3025
#define WUM_IMAXOPEN			WM_USER + 3026

#define IMAX_INFO_CODE			0
#define IMAX_INFO_ORDERDATA		1
#define IMAX_INFO_ORDERPRICE		2
#define IMAX_INFO_BUTTONEVENT		4


//
//	Get/Set IMAXApi
//
#define WUM_IMAXDATAGET			WM_USER + 201			// 가격 수량 가져오기
#define WUM_IMAXDATASET			WM_USER + 202			// 내부로 옵션 설정 할 경우
#define WUM_RECEIVECODECHANGE		WM_USER + 3003			// 종목코드 변경시 IMAXAPI로 전달

#define IMXID_SETBALANCEVALUEFIRST	20015				// First Jango Grid
#define IMXID_SETBALANCEVALUENEXT	20080				// Next Jango Grid
#define IMXID_SETOPTION			20016				// [코드콤보] 콤보 Open 초기화 옵션 설정 (기본 : 0  , 0x0800 : ETF)
#define IMXID_SETORDERMODE		20034				// 종목별 시세정보
#define IMXID_SETFOCUS			20031				// [코드콤보] SetFocus

#define IMXID_SETPRICETEXT		20022

#define IMAX_BALANCEVALUEFIRST		0				// First Jango Grid
#define IMAX_BALANCEVALUENEXT		1				// Next Jango Grid
#define IMAX_SETORDERMODE		2				// 주문 모드 변경
#define IMAX_SETOPTION			3				// [코드콤보] 콤보 Open 초기화 옵션 설정 (기본 : 0  , 0x0800 : ETF)
#define IMAX_FOCUS			9
#define IMAX_HKCHANGECODE		12				// 홍콩
#define IMAX_SHCHANGECODE		16				// 상해
#define IMAX_SZCHANGECODE		17				// 심천
#define	IMAX_NYCHANGECODE		21				// 뉴욕
#define IMAX_NASCHANGECODE		22				// 나스닥
#define	IMAX_AMSCHANGECODE		23				// 아멕스
#define	IMAX_OTCCHANGECODE		24				// OTC

#define IMAX_PRCSETTING			25

//
// trigger symbol name
//
#define IMAX_EXID			"IMAXEXID"
#define IMAX_CODE			"IMAXCODE"
#define IMAX_CODENAME			"IMAXCODENAME"
#define IMAX_PRICE			"IMAXPRICE"
#define IMAX_ORQTY			"IMAXORQTY"
#define IMAX_MAPNUM			"IMAXMAPNUM"
#define IMAX_ISIN			"IMAXISIN"
#define IMAX_ABLEMEDO			"IMAXABLEMEDO"

#define	IMXID_GETCODE			 11				// 종목코드
#define IMXID_GETCURPRICE		100				// 현재가
#define IMXID_GETBASEPRICE		101				// 기준가
#define IMXID_GETBUYPRICE1		201				// 매수1호가
#define IMXID_GETBUYPRICE2		202
#define IMXID_GETBUYPRICE3		203
#define IMXID_GETBUYPRICE4		204
#define IMXID_GETBUYPRICE5		205
#define IMXID_GETSELPRICE1		301				// 매도1호가
#define IMXID_GETSELPRICE2		302
#define IMXID_GETSELPRICE3		303
#define IMXID_GETSELPRICE4		304
#define IMXID_GETSELPRICE5		305
#define IMXID_GETBUYVOLUME		401				// 매수수량
#define IMXID_GETSELLVOLUME		501				// 매도수량
#define	IMXID_GETCURRENCY		601				// 통화
#define IMXID_GETAUTHORITY		701				// 시세구분


//skin color
#define IMAX_COL_GRAY			1
#define IMAX_COL_BLUE			2
#define IMAX_COL_BROWN			3
#define IMAX_COL_NONE			-1
/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd(CWnd *pParent, struct _param *pParam)
{
	EnableAutomation();
	
	m_pMainWnd	= nullptr;
	m_pParent	= pParent;
	m_sHome		= "";
	m_sViewNo	= "";
	m_bLoinError	= FALSE;
	m_hView		= nullptr;
	m_sUsID		= "";
	m_nGroupNo	= 1;
	m_bStaf		= false;

	m_sExid		= "";
	m_sCode		= "";
	m_sCodeName	= "";
	m_sBidUnit	= "";
	m_sAskUnit	= "";
	m_sSelPrice	= "";
	m_sBidPrice	= "";
	m_sAskPrice	= "";
	m_sCurrency	= "";
	m_sOrderQty	= "";

	SetParam(pParam);
	m_iSkin = IMAX_COL_NONE;
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
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WUM_IMAXCODECHANGE, IMAXOnMessageCODEChange)		// WM_USER + 3002
	ON_MESSAGE(WUM_IMAXORDERINFO, IMAXOnMessageORDERINFO)		// WM_USER + 3025
	ON_MESSAGE(WUM_IMAXOPEN, IMAXOnMessageOPEN)			// WM_USER + 3026
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "SetIMAXGroup", _SetIMAXGroup, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CControlWnd, "SetIMAXCIFId", _SetIMAXCIFId, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetIMAXSkin", _SetIMAXSkin, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CControlWnd, "SetIMAXData", _SetIMAXData, VT_EMPTY, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetIMAXValue", _GetIMAXValue, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "InitIMAX", _InitIMAX, VT_I2, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetVisible", _SetVisible, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "SetEnable", _SetEnable, VT_EMPTY, VTS_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A59B7CBB-358C-41A3-AA9A-E22DA4D76E94}
static const IID IID_IControlWnd =
{ 0xa59b7cbb, 0x358c, 0x41a3, { 0xaa, 0x9a, 0xe2, 0x2d, 0xa4, 0xd7, 0x6e, 0x94 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
long CControlWnd::IMAXOnMessageOPEN(WPARAM wParam, LPARAM lParam)
{
m_tmpX.Format("IMAXOnMessageOPEN() viewNo[%s]msg[%s]map[%s]", m_sViewNo, (LPCTSTR)lParam, (LPCTSTR)wParam); dprint();

	CString sIMAXMessageInfo = (LPCTSTR)lParam;
	CString	sImaxMapNum = (LPCTSTR)wParam;
	CString	stmp;
	
	if (sImaxMapNum == "3111")   //시세신청
	{
		stmp.Format("%s /S /p5 /t0", "IB599000");
		openView(typePOPUP, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "8520")   //기업정보
	{
		stmp.Format("%s /S /p5 /t0", "IB567100");
		openView(typePOPUP, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "2112")   //현재가
	{
		stmp.Format("%s /t", "IB567000");
		openView(typeVIEW, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "3333")  //차트
	{
		stmp.Format("%s /S/t0", "IB586000");
		openView(typeVIEW, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "1448") //주문종합
	{
		stmp.Format("%s /S/t0", "IB550000");
		openView(typeVIEW, stmp); 
		return 0;
	}
	else if (!sImaxMapNum.IsEmpty())
	{
		stmp.Format("%s\t%s", IMAX_MAPNUM, sImaxMapNum);
		IMAXToMapTrigger(stmp);
	}

	parseIMAXMessageData(IMAX_INFO_BUTTONEVENT, sIMAXMessageInfo);

	return 0;
}

long CControlWnd::IMAXOnMessageCODEChange(WPARAM wParam, LPARAM lParam)
{
m_tmpX.Format("IMAXOnMessageCODEChange() viewNo[%s]msg[%s]", m_sViewNo, (LPCTSTR)lParam); dprint();

	CString sIMAXMessageInfo = (LPCTSTR)lParam;
	parseIMAXMessageData(IMAX_INFO_CODE, sIMAXMessageInfo);
	return 0;
}

long CControlWnd::IMAXOnMessageORDERINFO(WPARAM wParam, LPARAM lParam)
{
m_tmpX.Format("IMAXOnMessageORDERINFO() viewNo[%s]msg[%s]", m_sViewNo, (LPCTSTR)lParam); dprint();	
	
	CString sIMAXMessageInfo = (LPCTSTR)lParam;

	if (sIMAXMessageInfo.Find('\t') < 0)
		parseIMAXMessageData(IMAX_INFO_ORDERPRICE, sIMAXMessageInfo);
	else
		parseIMAXMessageData(IMAX_INFO_ORDERDATA, sIMAXMessageInfo);

	return 0;
}

void CControlWnd::parseIMAXMessageData(int nKind, CString sIMAXInfo)
{
	bool	bEvent = false;
	CString stmp;
	CString sArr[16];
	CString sExid, sCode, sCodeName, sSelPrice, sOrderQty, sISINCode, sablemedo;

	if (nKind == IMAX_INFO_CODE)
	{
		for (int ii = 0; ii < 16; ii++)
		{
			sArr[ii].Empty();
			AfxExtractSubString(sArr[ii], sIMAXInfo, ii, '\t');
		}
	}
	else
	{
		for (int ii = 0; ii < 9; ii++)
		{
			sArr[ii].Empty();
			AfxExtractSubString(sArr[ii], sIMAXInfo, ii, '\t');
		}
	}

	
	switch (nKind)
	{
	case IMAX_INFO_CODE:
		sExid		= sArr[0];
		sCode		= sArr[1];
		sCodeName	= sArr[2];
		m_sBidUnit	= sArr[3];
		m_sAskUnit	= sArr[4];
		sSelPrice	= sArr[5];
		m_sBidPrice	= sArr[6];
		m_sAskPrice	= sArr[7];
		m_sCurrency	= sArr[8];

		sISINCode	= sArr[14];
		sablemedo	= sArr[15];
m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_CODE) viewNo[%s]Kind[%d]sExid[%s]sCode[%s]sCodeName[%s]m_sBidUnit[%s]m_sAskUnit[%s]m_sSelPrice[%s]m_sBidPrice[%s]m_sAskPrice[%s]m_sCurrency[%s]sASINCode[%s]", 
		m_sViewNo, nKind, sExid, sCode, sCodeName, m_sBidUnit, m_sAskUnit, m_sSelPrice, m_sBidPrice, m_sAskPrice, m_sCurrency, sISINCode); dprint();
		break;

	case IMAX_INFO_ORDERDATA:
		sExid		= sArr[0];
		sCode		= sArr[1];
		sCodeName	= sArr[2];
		m_sBidUnit	= sArr[3];
		m_sAskUnit	= sArr[4];
		sSelPrice	= sArr[5];
		sOrderQty	= sArr[6];
m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_ORDERDATA) viewNo[%s]Kind[%d]sExid[%s]sCode[%s]sCodeName[%s]m_sBidUnit[%s]m_sAskUnit[%s]sSelPrice[%s]sOrderQty[%s]", 
		m_sViewNo, nKind, sExid, sCode, sCodeName, m_sBidUnit, m_sAskUnit, sSelPrice, sOrderQty); dprint();
		break;

	case IMAX_INFO_ORDERPRICE:
		sSelPrice	= sArr[0];
m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_ORDERPRICE) viewNo[%s]Kind[%d]sSelPrice[%s]", m_sViewNo, nKind, sSelPrice); dprint();
		break;

	case IMAX_INFO_BUTTONEVENT:
		sExid		= sArr[0];
		sCode		= sArr[1];
		sCodeName	= sArr[2];
		m_sBidUnit	= sArr[3];
		m_sAskUnit	= sArr[4];
m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_BUTTONEVENT) viewNo[%s]Kind[%d]sExid[%s]sCode[%s]sCodeName[%s]m_sBidUnit[%s]m_sAskUnit[%s]", 
		m_sViewNo, nKind, sExid, sCode, sCodeName, m_sBidUnit, m_sAskUnit); dprint();
		break;

	default:break;
	}

	if (!sablemedo.IsEmpty())
	{
		stmp.Format("%s\t%s", IMAX_ABLEMEDO, sablemedo);
		IMAXToMapTrigger(stmp);
		m_tmpX = _T("parseIMAXMessageData() ABLEMEDO_") + stmp; dprint();		
	}

	if (!sISINCode.IsEmpty())
	{
		m_sExid = sExid;
		stmp.Format("%s\t%s", IMAX_ISIN, sISINCode);
		IMAXToMapTrigger(stmp);
		m_tmpX = _T("parseIMAXMessageData() ASIN_") + stmp; dprint();		
	}

	if (!sExid.IsEmpty())
	{
		m_sExid = sExid;
		stmp.Format("%s\t%s", IMAX_EXID, sExid);
		if (m_sViewNo != "1427")
			IMAXToMapTrigger(stmp);
m_tmpX = _T("parseIMAXMessageData() EXID_") + stmp; dprint();		
	}

	if (!sCode.IsEmpty())
	{
		if (sCode.CompareNoCase(m_sCode) || m_sCode.IsEmpty())
		{
			m_sCode = sCode;
			stmp.Format("%s\t%s", IMAX_CODE, sCode);
			IMAXToMapTrigger(stmp);
			bEvent = true;
		}
m_tmpX = _T("parseIMAXMessageData() CODE_") + stmp; dprint();
	}

	if (!sCodeName.IsEmpty())
	{
		if (sCodeName.CompareNoCase(m_sCodeName) || m_sCodeName.IsEmpty())
		{
			m_sCodeName = sCodeName;
			stmp.Format("%s\t%s", IMAX_CODENAME, sCodeName);
			IMAXToMapTrigger(stmp);
			bEvent = true;
		}
m_tmpX = _T("parseIMAXMessageData() CODENAME_") + stmp; dprint();	
	}

	if (!sSelPrice.IsEmpty())
	{
		m_sSelPrice = sSelPrice;
		stmp.Format("%s\t%s", IMAX_PRICE, sSelPrice);
		IMAXToMapTrigger(stmp);
m_tmpX = _T("parseIMAXMessageData() PRICE_") + stmp; dprint();		
	}

	if (!sOrderQty.IsEmpty())
	{
		m_sOrderQty = sOrderQty;
		stmp.Format("%s\t%s", IMAX_ORQTY, sOrderQty);
		IMAXToMapTrigger(stmp);
m_tmpX = _T("parseIMAXMessageData() JQTY_") + stmp; dprint();		
	}

	if (bEvent)
	{
		DllEvent(nKind);
m_tmpX = _T("parseIMAXMessageData() EVENT") + stmp; dprint();
	}
}

void CControlWnd::SetParam(struct _param *pParam)
{
	CString	keys, text, tmps;

	m_param.key	= pParam->key;
	m_param.name	= CString(pParam->name, pParam->name.GetLength());
	m_param.rect	= CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts	= CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point	= pParam->point;
	m_param.style	= pParam->style;
	m_param.tRGB	= pParam->tRGB;
	m_param.pRGB	= pParam->pRGB;
	m_param.options	= CString(pParam->options, pParam->options.GetLength());
	
	tmps = _T("/ ");
	keys = _T("t");
	
	for (int ii = 0, idx = 0, pos = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = m_param.options.Find(tmps);
		if (idx < 0)
			continue;
		
		idx += 2;
		pos = m_param.options.Find('/', idx);
		
		text = (pos < 0) ? m_param.options.Mid(idx) : m_param.options.Mid(idx, pos-idx);
		text.TrimLeft();
		
		switch (keys[ii])
		{
		case 't':
			m_sViewNo = text;
			break;
		}
	}
m_tmpX.Format("SetParam() viewNo[%s]cx[%d]cy[%d]", m_sViewNo, m_param.rect.Width(), m_param.rect.Height()); dprint();
}

void CControlWnd::Init()
{
	m_sHome	= Variant(homeCC);
	m_bStaf = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, prohibitCC), 0) ? true : false;

m_tmpX.Format("Init() viewNo[%s],Home[%s],User[%s]\n", m_sViewNo, m_sHome, m_sUsID); dprint();

//	m_pMainWnd = (CWnd*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getMAIN, 0), 0);
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString ret;
	const	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());
	
	if ((long)dta > 1)
		ret = dta;
	
	return ret;
}

void CControlWnd::LoginIMAX()
{
m_tmpX.Format("LoginIMAX() call"); dprint();

	const	int	nIntetNetKind = 0;
	LPTSTR	szLogin = m_sUsID.GetBuffer(m_sUsID.GetLength() + 1);
	HWND	hFrameWnd = this->GetSafeHwnd();

	m_bLoinError = FALSE;
	if (!m_IMAXApi.IMAXInit(hFrameWnd, szLogin, nIntetNetKind))
		m_bLoinError = TRUE;
m_tmpX.Format("LoginIMAX() viewNo[%s]usid[%s]frameWnd[%d]error[%d]\n", m_sViewNo, m_sUsID, (int)hFrameWnd, m_bLoinError); dprint();

	m_sUsID.ReleaseBuffer();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_bLoinError)
		return;

	CRect rc;
	
	GetClientRect(&rc);
	m_IMAXApi.Resize(m_hView, rc.Width(), rc.Height());
}

void CControlWnd::IMAXToMapTrigger(CString sValue)
{
m_tmpX.Format("IMAXToMapTrigger() : viewNo[%s]Trigger[%s]", m_sViewNo, sValue); dprint();

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(formDLL, -1), (LPARAM)sValue.GetString());
	Variant(triggerCC, sValue);
}

void CControlWnd::DllEvent(int nKind)
{
	CString sName = m_param.name;

m_tmpX.Format("DllEvent() viewNo[%s]nKind[%d]", m_sViewNo, nKind); dprint();

	switch (nKind)
	{
	case IMAX_INFO_CODE:
	case IMAX_INFO_BUTTONEVENT:
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)sName.GetString());
		break;
	case IMAX_INFO_ORDERDATA:
	case IMAX_INFO_ORDERPRICE:
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)sName.GetString());
		break;
	}
}

void CControlWnd::_SetIMAXCIFId(LPCTSTR CIFId) 
{
	m_sUsID = CString(CIFId);	
}

short CControlWnd::_InitIMAX() 
{
m_tmpX.Format("InitMAX() call"); dprint();

	m_IMAXApi.LoadImaxGate();
	LoginIMAX();
	return LoadIMAXView(m_sViewNo, m_param.rect);
}

bool CControlWnd::LoadIMAXView(CString ViewName, CRect rcCli)
{
	if (m_bLoinError || ViewName.IsEmpty())
		return false;
	
	if (m_iSkin > 0)
		m_IMAXApi.ChangeSkin(m_iSkin);
	else
		m_IMAXApi.ChangeSkin(2);
	
	LPTSTR szImaxNo = ViewName.GetBuffer(ViewName.GetLength() +1);
	LPTSTR szCodeInfo = m_sCode.GetBuffer(m_sCode.GetLength() +1); 

m_tmpX.Format("LoadIMAXView() API.Opne(hwnd[%p]viewNo[%s]code[%s]width[%d]height[%d])", this->GetSafeHwnd(), szImaxNo, szCodeInfo, rcCli.Width(), rcCli.Height()); dprint();
	m_hView = m_IMAXApi.Open(this->GetSafeHwnd(), szImaxNo, szCodeInfo, rcCli.Width(), rcCli.Height()); dprint();
	if (m_hView == nullptr)
	{
m_tmpX.Format("LoadIMAXView() API.Open call error : m_hView is nullptr"); dprint();
		ViewName.ReleaseBuffer(); m_sCode.ReleaseBuffer();
		return false;
	}
m_tmpX.Format("LoadIMAXView() API.Open call OK"); dprint();
	
	m_IMAXApi.SetGroup(m_hView, m_nGroupNo);
	ViewName.ReleaseBuffer(); m_sCode.ReleaseBuffer();

	return true;
}

void CControlWnd::_SetIMAXGroup(short nGroup) 
{
m_tmpX.Format("SetIMAXGroup() viewNo[%s]group[%d]", m_sViewNo, nGroup); dprint();

	if (m_bLoinError)
		return;

	m_nGroupNo = nGroup;
	if (!m_hView)
		return;

	m_IMAXApi.SetGroup(m_hView, nGroup);
}

void CControlWnd::_SetIMAXSkin(short nSkin) 
{
m_tmpX.Format("SetIMAXSkin() viewNo[%s]skin[%d]", m_sViewNo, nSkin); dprint();
	
	if (m_bLoinError)
		return;

	m_iSkin = nSkin;
	m_IMAXApi.ChangeSkin(nSkin);
}

void CControlWnd::_SetIMAXData(short nKind, LPCTSTR pData) 
{
	if (m_bLoinError)
		return;

	CString	sValue = CString(pData);
	CString	sCodeFormat;

	HWND	hWnd = m_IMAXApi.GetHwnd(m_hView);

	if (!IsWindow(hWnd))
		return;

	::SendMessage(hWnd, WUM_IMAXDATASET, 0, 0);

	switch ((int)nKind)
	{
	case IMAX_BALANCEVALUEFIRST:
m_tmpX.Format("SetIMAXData(IMAX_BALANCEVALUE) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();
	::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETBALANCEVALUEFIRST, (LPARAM)sValue.GetString());
		break;

	case IMAX_BALANCEVALUENEXT:
m_tmpX.Format("SetIMAXData(IMAX_BALANCEVALUENEXT) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();	
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETBALANCEVALUENEXT, (LPARAM)sValue.GetString());
		break;

	case IMAX_SETORDERMODE:
m_tmpX.Format("SetIMAXData(IMAX_SETORDERMODE) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, ""); dprint();		
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETORDERMODE, 0);
		break;

	case IMAX_SETOPTION:
m_tmpX.Format("SetIMAXData(IMAX_SETOPTION) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETOPTION, atoi(sValue));
		break;

	case IMAX_FOCUS:
m_tmpX.Format("SetIMAXData(IMAX_FOCUS) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETFOCUS, (LPARAM)(LPCSTR)0);
		break;

	case IMAX_HKCHANGECODE:	case IMAX_SHCHANGECODE:	 case IMAX_SZCHANGECODE:
	case IMAX_NYCHANGECODE:	case IMAX_NASCHANGECODE: case IMAX_AMSCHANGECODE:
	case IMAX_OTCCHANGECODE:  //ctrl_code.SetIMAXData GetIMAXMk(smkgubn), GetIMaxMkcode(smkgubn) & vbTab & scode
m_tmpX.Format("SetIMAXData(IMAX_CHANGECODE) viewNo[%s]kind[%d]value[%s]code[%s]", m_sViewNo, nKind, sValue, m_sCode); dprint();	
		if (m_sCode.CompareNoCase(sValue) || m_sCode.IsEmpty())
		{
	//		sCodeFormat.Format("%04d\t%s", (int)nKind, sValue);
	//		::SendMessage(hWnd, WUM_RECEIVECODECHANGE, 770, (LPARAM)sCodeFormat.GetString());
			sCodeFormat.Format("%s", sValue);
			m_IMAXApi.ChangeCodeWindow(hWnd, (char *)sCodeFormat.GetString());
			m_sCode = sValue;
		}
	case IMAX_PRCSETTING:  //호가스핀에디트 값변경
		{
			::SendMessage(hWnd, IMXID_SETPRICETEXT, (WPARAM)0, (LPARAM)sValue.GetString());
		}
		break;
	}
}

BSTR CControlWnd::_GetIMAXValue(LPCTSTR sValueText) 
{
	CString strResult;

	if (m_bLoinError)
		return strResult.AllocSysString();

	int	nKey = 0;
	CString sText, sValue = "";
	char	szBuf[256]{};
	HWND	hWnd = m_IMAXApi.GetHwnd(m_hView);

	if (!IsWindow(hWnd))
		return strResult.AllocSysString();

	sText = CString(sValueText); sText.Trim();
	if (sText.IsEmpty()) return strResult.AllocSysString();

	if (sText == "code")		nKey = IMXID_GETCODE;
	else if (sText == "curr")	nKey = IMXID_GETCURPRICE;
	else if (sText == "base")	nKey = IMXID_GETBASEPRICE;
	else if (sText == "buy1")	nKey = IMXID_GETBUYPRICE1;
	else if (sText == "buy2")	nKey = IMXID_GETBUYPRICE2;
	else if (sText == "buy3")	nKey = IMXID_GETBUYPRICE3;
	else if (sText == "buy4")	nKey = IMXID_GETBUYPRICE4;
	else if (sText == "buy5")	nKey = IMXID_GETBUYPRICE5;
	else if (sText == "sel1")	nKey = IMXID_GETSELPRICE1;
	else if (sText == "sel2")	nKey = IMXID_GETSELPRICE2;
	else if (sText == "sel3")	nKey = IMXID_GETSELPRICE3;
	else if (sText == "sel4")	nKey = IMXID_GETSELPRICE4;
	else if (sText == "sel5")	nKey = IMXID_GETSELPRICE5;
	else if (sText == "buyq")	nKey = IMXID_GETBUYVOLUME;
	else if (sText == "selq")	nKey = IMXID_GETSELLVOLUME;
	else if (sText == "currency")	nKey = IMXID_GETCURRENCY;
	else if (sText == "auth")	nKey = IMXID_GETAUTHORITY;

	::SendMessage(hWnd, WUM_IMAXDATAGET, (WPARAM)nKey, (LPARAM)szBuf);
	sValue = szBuf;

	if (!sValue.IsEmpty())
	{
		sValue.Trim();
	}

	if (sValue.IsEmpty())
	{
		if (sText == "buyq")
			sValue = m_sBidUnit;
		else if (sText == "edprc")
			sValue = m_sSelPrice;

m_tmpX.Format("parseIMAXMessageData(IMAX__) sValue empty"); dprint();
dprint();
	}

	strResult = sValue;

m_tmpX.Format("parseIMAXMessageData(IMAX__) GetIMAXValue() videNo[%s]sText[%s]nKey[%d]strResult[%s]", m_sViewNo, sText, nKey, strResult); dprint();
dprint();	
	return strResult.AllocSysString();
}

void CControlWnd::OnDestroy() 
{
m_tmpX.Format("Close() viewNo[%s]hView[%p]", m_sViewNo, m_hView); dprint();

	m_IMAXApi.Close(m_hView);

	CWnd::OnDestroy();	
}

void CControlWnd::_SetVisible(BOOL bVisible) 
{
	if (bVisible)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CControlWnd::_SetEnable(BOOL bEnable) 
{
	EnableWindow(bEnable);
}

void CControlWnd::dprint()
{
	if (m_tmpX.IsEmpty())
		return;

	m_tmpX.Insert(0, _T("[IMAX:CON] : "));

// 필요시 사용할것. 평소엔 주석처리
//	OutputDebugString(m_tmpX);

	m_tmpX.Empty();
}

int CControlWnd::openView(int type, CString data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.GetString());
}