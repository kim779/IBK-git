// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Notify.h"
#include "ControlWnd.h"
#include "ShMemory.h"

#include "../../h/ledger.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_REMAIN		WM_USER + 2
#define WM_ALLACCNREMAIN	WM_USER + 3
#define WM_REMAIN_CLOSE		WM_USER + 4
#define WM_LAWCHEG		WM_USER + 5

#define	ENCTEST

#define	SC_TEST				// CCriticalSection test
#define	CREDIT				// �ſ��ܰ� test

#define	CREDIT_DATE		11	// �ſ��ܰ�¥ index
#define	CREDIT_SYGB		12	// �ſ뱸�� index
#define CREDIT_JGGB		16	// �ܰ��� index

#define LEN_JGGB		2	// �ܰ���
#define	LEN_ACODE		12	// �ֽ� ��ü�ڵ� ����
#define	LEN_SYGB		2	// �ſ뱸�� ����
#define	LEN_DATE		8	// ��¥

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_ShMemory = nullptr;
	m_arField.RemoveAll();
	m_bAllAccn = false;
	m_bFuture = false;
	m_bLaw = false;
	m_bFilter = false;
	m_iJanType = 0;
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_REMAIN, OnRemainMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "SetAccount", m_setAccount, OnSetAccountChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "DataList", m_dataList, OnDataListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Flag", m_flag, OnFlagChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "DataMsg", m_dataMsg, OnDataMsgChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "SetMcgb", m_setMcgb, OnSetMcgbChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Send", Send, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendEx", SendEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "AllAccnSend", AllAccnSend, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "AddReminData", AddReminData, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendX", SendX, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendRaw", SendRaw, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetFilterAcc", SetFilterAcc, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {7E5363E5-D4A1-4866-B9FF-A97FC342672D}
static const IID IID_IControlWnd =
{ 0x7e5363e5, 0xd4a1, 0x4866, {0xb9, 0xff, 0xa9, 0x7f, 0xc3, 0x42, 0x67, 0x2d}};

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

#define DF_SHARED
//=================================================================================================
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
#ifdef	SC_TEST
	m_cs.Lock();
#endif
	CString path = Variant(homeCC, "");
	path.Replace("\\", "_");

	m_ShMemory = std::make_unique<CShMemory>();

	DWORD processID = GetCurrentProcessId();
	CString sSHMENAME;
	
#ifdef DF_SHARED
	if (!m_bFuture)
	{
		sSHMENAME.Format("%s%s%d", path, "_remainCtrl_", processID);
		m_ShMemory->m_remainSHMEMNAME = sSHMENAME;
	}
	else
	{
		sSHMENAME.Format("%s%s%d", path, "_remainCtrl_Future", processID);
		m_ShMemory->m_remainSHMEMNAME = sSHMENAME;
	}

#else
	if (!m_bFuture)
		m_ShMemory->m_remainSHMEMNAME = path + "_remainCtrl_";
	else
		m_ShMemory->m_remainSHMEMNAME = path + "_remainCtrl_Future";
#endif

	m_ShMemory->InitSharedMemory(this->m_hWnd);
	m_ShMemory->AddHandle(this->m_hWnd);

#ifdef	SC_TEST
	m_cs.Unlock();
#endif
	return 0;
}

void CControlWnd::OnDestroy() 
{
	if (m_ShMemory == nullptr)
		return;

	HWND	MasterHwnd = m_ShMemory->GetHandle(0);
	
	//CString	Data;
	//Data.Format("%d\t%s\t%s\t%d\t", (m_bFuture?1:0), m_setAccount, "close", 1);	// 1:�ſ���

	m_ShMemory->RemoveHandle(this->m_hWnd);
	m_ShMemory.reset();

	::SendMessage(MasterHwnd, WM_REMAIN_CLOSE, (WPARAM)this->m_hWnd, (LPARAM)m_setAccount.GetString());

	CWnd::OnDestroy();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		m_sCtrlData.Empty();
		return (long)m_sCtrlData.GetString();
	case DLL_OUB:
		break;
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:	// Remain Data Send to Map!
		if (!m_bLaw)
		{
			// IBXXXX01 sheared ���� �����ִ� ����Ÿ(�ǽð��ܰ�)
			CString sData = (char*)lParam, sAccn, sTemp;
			if (m_bAllAccn)
			{
				sAccn = Parser(sData, "\t");
				if (m_AccnMap.Lookup(sAccn, sTemp))
					SendToMap(sData, false, sAccn);	
			}
			else if (m_setAccount == Parser(sData, "\t"))
				SendToMap(sData, false);
		}
		else
		{
				
			CString sData = (char*)lParam;
			if (m_bFilter)
			{
				CString filler;
				if (!m_AccnFilter.Lookup(sData.Mid(0,11), filler))
					return 0;
			}

			const	int pos = sData.Find("#");

			m_dataList = sData.Mid(0, pos);
			m_dataMsg = sData.Mid(pos+1, sData.GetLength()-pos);

			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
						(LPARAM)m_Param.name.GetString());
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick/*DblClick*/)),
						(LPARAM)m_Param.name.GetString());
		}
		break;
	case DLL_DOMINO:
		break;
	case DLL_SETFONT:
		{
			m_Param.point = HIWORD(wParam);
			CString sFont = CString((char*)lParam);
			if (!sFont.IsEmpty())
				m_Param.fonts = sFont;
		}
		break;
	case DLL_NOTICE:
		break;
	}
	return 0;
}

// IBXXXX01 sheared ���� �����ִ� ����Ÿ (�ܰ� ����Ÿ �� �����޼���)
long CControlWnd::OnRemainMessage(WPARAM wParam, LPARAM lParam)
{
	CString stmp;
	CString sData = (char*)lParam;
	stmp.Format("[IBXXXX00][cx_notify] OnRemainMessage [%s]", sData);
	OutputDebugString(stmp);
	if (Parser(sData, "\t") == "ERR")	// guide �޼���(���� or ����), guide + data
	{
		sData = "guide\t" + sData;

		m_pParent->SendMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)sData.GetString());

		if (sData.GetAt(0) != _T('0'))	// ����
		{
			m_flag = _T("E");
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
						(LPARAM)m_Param.name.GetString());
		}
	}
	else
	{
		SendToMap((char*)lParam, true);	// ��ü data
	}
	return 0;
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this);
	CRect	rc;

	GetClientRect(rc);
	dc.FillSolidRect(rc, RGB(0,0,0));
}


//=================================================================================================
void CControlWnd::OnSetAccountChanged() 
{
}

void CControlWnd::OnDataListChanged() 
{
}

void CControlWnd::OnFlagChanged() 
{
}

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;
	return strResult.AllocSysString();
}

void CControlWnd::SetProperties(LPCTSTR sProperties) 
{
	m_Param.options = sProperties;
}

// �ܰ���ȸ
void CControlWnd::Send(LPCTSTR sAccn, LPCTSTR sPswd,LPCTSTR dfee,LPCTSTR dmass) 
{
	SendEx(sAccn, sPswd, dfee, dmass, _T("0"));
}


void CControlWnd::SendEx( LPCTSTR sAccn, LPCTSTR sPswd, LPCTSTR dfee, LPCTSTR dmass, LPCTSTR dCalcType/*=_T("0")*/ )
{
	CString stmp;
	stmp.Format("[twopc]   CControlWnd::SendEx ");
	OutputDebugString(stmp);

	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);
	stmp.Format("[twopc]   CControlWnd::SendEx   MasterHwnd =[%x]", MasterHwnd);
	OutputDebugString(stmp);
	m_bLaw = false;
	m_CodeMap.RemoveAll();
	
	m_setAccount = sAccn;
	Data.Format("%d\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t", m_bFuture ? 1 : 0, 
							sAccn,
							sPswd,
							1,
							dfee,
							dmass,
							dCalcType,
							m_setMcgb);	// 1:�ſ���	//2015.04.22 KSJ ��ü���� �߰�
	::SendMessage(MasterHwnd, WM_REMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}


// �ܰ���ȸ : �ſ�����
void CControlWnd::SendX(LPCTSTR sAccn, LPCTSTR sPswd) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_CodeMap.RemoveAll();

	m_setAccount = sAccn;
	Data.Format("%d\t%s\t%s\t%d\t%s\t", m_bFuture ? 1 : 0,
					    sAccn,
					    sPswd,
					    0,
					    m_setMcgb);			// �ſ�����	//2015.04.22 KSJ ��ü���� �߰�
	::SendMessage(MasterHwnd, WM_REMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}


// ��ü�ܰ���ȸ : ��뿩�� ?
void CControlWnd::AllAccnSend(LPCTSTR sUserID, LPCTSTR sPswd, LPCTSTR sCode) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bAllAccn = true;
	m_CodeMap.RemoveAll();

	Data.Format("%d\t%s\t%s\t%s", m_bFuture ? 1 : 0,
				      sUserID,
				      sPswd,
				      sCode);
	::SendMessage(MasterHwnd, WM_ALLACCNREMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}

void CControlWnd::Clear() 
{
	m_setAccount.Empty();
	m_CodeMap.RemoveAll();
	m_AccnMap.RemoveAll();
}

void CControlWnd::AddReminData(LPCTSTR sAccn, LPCTSTR sCode, LPCTSTR sRowData) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_AccnMap.SetAt(sAccn, "1");
	Data.Format("%d\t%s\t%s\t%d\t%s\t", m_bFuture ? 1 : 0,
					    sAccn,
					    sCode,
					    1,
					    sRowData);	// �ſ���
	::SendMessage(MasterHwnd, WM_ALLACCNREMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}

//=================================================================================================
void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString tmp = m_Param.options;
	CString fieldS = OptionParser(tmp, "/edit");

	fieldS.Replace("|", "\t");
	m_bFuture = atoi(Parser(fieldS, "\t"))?true:false;
	if (tmp.Find("accnAll") != -1)
		m_bAllAccn = true;

	UINT	idx = 0;
	while (!fieldS.IsEmpty())
	{
		idx = atoi(Parser(fieldS, "\t"));
		m_arField.Add(idx);
	}

	CString sJtype = OptionParser(tmp, "/q");  //0��ü 1������ָ� 2�����������
	if (!sJtype.IsEmpty())
		m_iJanType = atoi(sJtype);
}

// bAll -> true : �ܰ���ȸ, false : �ǽð� �ܰ�
// sAccn -> !IsEmpty() : �������¿������

void CControlWnd::SendToMap(CString sData, bool bAll, CString sAccn/* = ""*/)
{
	if (sData.Find("165T3000") >= 0)
		TRACE("!23");
	CString stmp;
	stmp.Format("\r\n[IBXXXX00][cx_notify] sData=[%s]", sData);
	OutputDebugString(stmp);
#ifdef	SC_TEST
	m_cs.Lock();
#endif
	CString sSendData, sCodeData, sCode;
	CString keyS, dateS, sygbS, jggb;
	CStringArray sDataArr;
	int	ii = 0, idx = 0, bound = 0;

	if (bAll)	
	{
		//==================================================
		// �ܰ���ȸ
		// flag : 'A'
		// �ڵ尹��\t + { {��������Ÿ\t} * n�� +  \n }
		//==================================================
		m_CodeMap.RemoveAll();

		while (!sData.IsEmpty())
		{
			sDataArr.RemoveAll();
						
			sCodeData = Parser(sData, "\n");		// ���� �ܰ�
			while (!sCodeData.IsEmpty())
				sDataArr.Add(Parser(sCodeData, "\t"));		
			if (sDataArr.GetSize() <= 0)
				continue;

			bound = sDataArr.GetUpperBound();
			sCode = sDataArr.GetAt(0);
			if (m_bFuture)
				keyS = getKey(sCode, "", "", "");
			else
			{
				getDate(sDataArr, dateS, sygbS, jggb);
				keyS = getKey(sCode, dateS, sygbS, jggb);
			}

			if (m_iJanType == 1 && sygbS != "05")   //������ָ�
				continue;

			if (m_iJanType == 2 && sygbS == "05")	//������� ����
				continue;
						
			for (ii = 0; ii < m_arField.GetSize(); ii++)
			{
				idx = m_arField.GetAt(ii);
				if (idx > bound)
					continue;
				sSendData += sDataArr.GetAt(idx) + "\t";
			}
			m_CodeMap.SetAt(keyS, sSendData);
			sSendData += "\n";
		}
		m_flag = "A";
		m_dataList.Format("%d\t", m_CodeMap.GetCount());
		m_dataList += sSendData;
	}
	else
	{
		//==================================================
		// IBXXXX01���� �޴� ����Ÿ : sData = "sFlag \t sData \t ..."
		// �ǽð� �ܰ�
		// flag : 'I'(insert), 'D'(delete), 'U'(update)
		// �Ϲ� : code + {��������Ÿ\t} * n�� 
		// �������� : ���¹�ȣ + code + ���¹�ȣ\t + {��������Ÿ\t} * n�� 
		// code + date(8)
		//==================================================
		const	int	FindIndex = sData.Find("|");

		if (FindIndex > -1)
			sData.Delete(FindIndex,sData.GetLength()-FindIndex);

		if (sData.IsEmpty())
		{
#ifdef	SC_TEST
			m_cs.Unlock();
#endif
			return;
		}
		
		m_flag = Parser(sData, "\t");
		stmp.Format("[IBXXXX00][cx_notify] not A m_flag=[%s] ", m_flag);
		OutputDebugString(stmp);

		while (!sData.IsEmpty())
			sDataArr.Add(Parser(sData, "\t"));

		stmp.Format("[IBXXXX00][cx_notify] not A while end");
		OutputDebugString(stmp);

		if (sDataArr.GetSize() <= 0)
		{
#ifdef	SC_TEST
			m_cs.Unlock();
#endif
			return;
		}
		
		bound = sDataArr.GetUpperBound();
		sCode = sDataArr.GetAt(0);
		//AfxMessageBox("[sCode: "+sCode+"]");
		if (m_bFuture)
			keyS = getKey(sCode, "", "", "");
		else
		{
			if (m_flag != "D")
			{
				getDate(sDataArr, dateS, sygbS, jggb);
				keyS = getKey(sCode, dateS, sygbS, jggb);

				if (m_iJanType == 1 && sygbS != "05")   //������ָ�
					return;

				if (m_iJanType == 2 && sygbS == "05") //������� ����
					return;
			}
			else
			{
				/*
				if (sygbS == "  ")
				{
					keyS = sCode.Left(22)+"00";
				}
				else
				*/
					keyS = sCode;
			}
		}

		if (m_flag == "D")					// Delete
			m_CodeMap.RemoveKey(keyS);
		else
		{
			if (!m_CodeMap.Lookup(keyS, sSendData))		// Insert
				m_flag = "I";				
		
			sSendData.Empty();
			if (m_bAllAccn)
				sSendData = sAccn + "\t";

			for (ii = 0; ii < m_arField.GetSize(); ii++)
			{
				idx = m_arField.GetAt(ii);
				if (idx > bound)	continue;
				sSendData += sDataArr.GetAt(idx) + "\t";
			}
			m_CodeMap.SetAt(keyS, sSendData);
		}
#ifdef	CREDIT
		if (m_bFuture)
			m_dataList = sCode + sSendData;	
		else	// ���� : code(11) + date(8) + �ſ뱸��(2)
			m_dataList = keyS + sSendData;	
#else
		m_dataList = sCode + sSendData;	
#endif
		if (m_bAllAccn)
			m_dataList = sAccn + m_dataList;
	}
stmp.Format("\r\n[IBXXXX00][cx_notify] m_dataList =[%s] ", m_dataList);
OutputDebugString(stmp);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
					(LPARAM)m_Param.name.GetString());
#ifdef	SC_TEST
	m_cs.Unlock();
#endif
}

BOOL CControlWnd::SendTR(CString sTR, BYTE type, CString data, int key /*= -1*/)
{
	struct	_userTH	udat {};
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(L_userTH + data.GetLength() + 128);
	int	idx = 0;

	if (key != -1)
	{
		type |= US_KEY;
		pWb[idx++] = key;
		key = MAKELONG(key, m_Param.key);
	}
	else
	{
		type &= ~US_KEY;
		key = m_Param.key;
		idx = m_Param.name.GetLength();
		CopyMemory(pWb.get(), m_Param.name.GetString(), idx);
	}
	pWb[idx++] = '\t';

	CopyMemory(udat.trc, sTR.GetString(), min(sizeof(udat.trc), sTR.GetLength()));
	udat.key  = key;
	udat.stat = type;

	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	CopyMemory(&pWb[idx], data.GetString(), data.GetLength());

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)pWb.get());
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

int CControlWnd::OpenView(int type, CString data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.GetString());
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;

	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}
 
// �ֽ� : code(12) + date(8) + sygb(2)
CString CControlWnd::getKey(CString codeS, CString dateS, CString sygbS, CString jggb)
{
	if (m_bFuture)
		return codeS;

	/*
				
	if (atoi(sygbS) >= 10 || atoi(sygbS) == 0)	// �ſ뱸�� �ƴϸ� 0
		sygbS = "  ";
	*/
	CString keyS;

	keyS.Format("%-*s%-*s%-*s%-*s", LEN_JGGB, jggb, LEN_ACODE, codeS, LEN_DATE, dateS, LEN_SYGB, sygbS);
	return keyS;
}

void CControlWnd::getDate(const CStringArray& arData, CString& dateS, CString& sygbS, CString& jggb)
{
	dateS.Empty();
	sygbS.Empty();

	if (m_bFuture || arData.GetUpperBound() < CREDIT_SYGB)
		return;

	dateS = arData.GetAt(CREDIT_DATE);
	sygbS = arData.GetAt(CREDIT_SYGB);
	jggb  = arData.GetAt(CREDIT_JGGB);
}

void CControlWnd::SendRaw(LPCTSTR sName) 
{
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bLaw = true;
	m_CodeMap.RemoveAll();

	::SendMessage(MasterHwnd, WM_LAWCHEG, (WPARAM)this->m_hWnd, (LPARAM)sName);
}

void CControlWnd::SetFilterAcc(LPCTSTR sAccList) 
{
	CString list = sAccList;
	CString acc;

	m_AccnFilter.RemoveAll();
	while (!list.IsEmpty())
	{
		acc = Parser(list, "\t");
		acc.TrimRight();
		m_AccnFilter.SetAt(acc, "account");
	}
	
	m_bFilter = (m_AccnFilter.GetCount() > 0) ? true : false;
}

void CControlWnd::OnDataMsgChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnSetMcgbChanged() 
{
	// TODO: Add notification handler code

}
