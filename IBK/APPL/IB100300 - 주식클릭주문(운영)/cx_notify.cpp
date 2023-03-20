// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "cx_notify.h"
#include "../../h/AxisVar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LEN_JGGB	2
#define	LEN_ACODE	12		// �ֽ� ��ü�ڵ� ����
#define	LEN_SYGB	2		// �ſ뱸�� ����
#define	LEN_DATE	8		// ��¥
#define	LEN_TOTAL	24		// key Length

/*
/////////////////////////////////////////////////////////////////////////////
// IControlWnd properties

CString IControlWnd::GetSetAccount()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetSetAccount(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString IControlWnd::GetDataList()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetDataList(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString IControlWnd::GetFlag()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetFlag(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IControlWnd operations

CString IControlWnd::GetProperties()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IControlWnd::SetProperties(LPCTSTR sProperties)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sProperties);
}

void IControlWnd::Send(LPCTSTR sAccn, LPCTSTR sPswd)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sAccn, sPswd);
}
*/

/////////////////////////////////////////////////////////////////////////////
// CNotify

CNotify::CNotify(CWnd* sendW, int sendK)
{
	m_sendW = sendW;
	m_sendK = sendK;

	m_saveK = -1;

	m_hInst = NULL;
	m_Wcontrol = NULL;

	axCreate = NULL;

//	m_Icontrol = NULL;
}

CNotify::~CNotify()
{
	sary.RemoveAll();
}


BEGIN_MESSAGE_MAP(CNotify, CWnd)
	//{{AFX_MSG_MAP(CNotify)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER,    OnSend)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNotify message handlers
BOOL CNotify::CreateNotify(CWnd* parent, CRect rect, CString root)
{
	m_parent = parent;
	m_root   = root;

	return CWnd::Create(NULL, "Notify_window", WS_CHILD|WS_VISIBLE, rect, parent, 5001);
}

int CNotify::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxEnableControlContainer();

	struct	_param	param;
	CString	path; path.Format("%s/%s/%s", m_root, DEVDIR, "cx_Notify.DLL");

//edit + Flag(�ֽ�:0, ����:1) + "|" + fieldNum + "|" +   fieldNum....

//"00.�����ڵ�");
//"01.�����");
//"02.�����ܰ�");
//"03.�ֹ��ܷ�");
//"04.������հ�");
//"05.�ŵ������ܰ�");
//"06.���簡");
//"07.�򰡱ݾ�");
//"08.�򰡼���");
//"09.�򰡼��ͷ�");
//"10.���Աݾ�");
//"12.�ܰ���");
//"16.�ܰ���");

	param.name	= "cx_Notify";
	param.fonts	= "����ü";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 232;
	param.pRGB	= 192;
//	param.options = "/edit0|00|02|04|08|12|16";	
	//�����ڵ�, �����ܰ�, ������հ�, �򰡼���, �ſ��ڵ�, �ܰ���, �����, ���ɼ���, �򰡱ݾ�, ���ͷ�, ������
	param.options = "/edit0|00|02|04|08|12|16|01|05|07|09|11";	  
	GetClientRect(&param.rect);

	m_hInst = LoadLibrary(path);
	if (!m_hInst)
	{
		TRACE("cx_Notify control load error....\n");
		return -1;
	}

	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hInst, "axCreate");
	if (axCreate == NULL)
	{
		TRACE("cx_Notify axCreate proc error...\n");
		return -1;
	}

	m_Wcontrol = axCreate(this, (void *) &param);

//	m_Icontrol = new IControlWnd(m_Wcontrol->GetIDispatch(TRUE));
	
	return 0;
}

void CNotify::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_Wcontrol != NULL) 
	{
		if (m_Wcontrol->GetSafeHwnd() != 0)
			m_Wcontrol->DestroyWindow();
		delete m_Wcontrol;
//		m_Icontrol->DetachDispatch(); delete m_Icontrol;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
}

void CNotify::OnClose() 
{
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow(); 
		delete m_Wcontrol;
//		delete m_Icontrol;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
	
	DestroyWindow(); CWnd::OnClose();
}

LONG CNotify::OnSend(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case invokeTRx:
		break;
	case variantDLL:
	case getPALETTE:
	case getBITMAP:
	case getFONT:
	case getPEN:
		return m_sendW->SendMessage(WM_USER, wParam, lParam);
	case formDLL:
		{
			CString strData = (char*) lParam;
			if (strData.Find("uide\t"))
			{
				strData.Delete(0,6);
				if (strData.GetAt(0) != '0')
				{
					MessageBox(strData, "IBK��������", MB_OK);
				}
			}
		}
		break;
	case eventDLL:
		CheckData();
		break;

	default:
		TRACE("OnSend = [0x%x][%d]\n", LOWORD(wParam), HIWORD(wParam));
		break;
	}

	return 0;
}

void CNotify::SetJango()
{
	CString	strData = "";
	CString strJCod;
	CString strJango;
	CString strPgdg;
	CString strPgsi;
	CString strJggb;
	CString strJggb2;
	CString strLoadDate;
	CString tmp;
	POSITION accPos{};

	CString strgubn, strtmp;
	CString slog;

//m_strCODX
	strgubn.Format("%s%s%s%s", m_pMapwnd->GetSiseCode(), m_pMapwnd->m_strJangoType, m_pMapwnd->m_strCreditType, m_pMapwnd->m_strLoanDate); //�ڵ� �ܰ��� �ſ뱸�� ������
//slog.Format("[1301] CNotify::SetJango() [%s]\n", strgubn);
//OutputDebugString(slog);
//	int igubn = m_pMapwnd->GetCondition();  //0 �Ϲ� 1�ſ� 2����
	for (accPos = sary.GetStartPosition(); accPos != NULL;)
	{
		strData = "";
		sary.GetNextAssoc(accPos, strJCod, strData);
		strJCod = Parser(strData, "\t");
		strJango = Parser(strData, "\t");
		strPgdg = Parser(strData, "\t");
		strPgsi = Parser(strData, "\t");
		strJggb = Parser(strData, "\t");      //�ſ뱸��
		strJggb2 = Parser(strData, "\t");     //�ܰ���
		strLoadDate = Parser(strData, "\t");  //������
		
		strtmp.Format("%s%s%s%s", strJCod,strJggb2,strJggb,strLoadDate);

		if(strgubn == strtmp)
		{
			m_pControlWnd->SetJango(strJCod, strJango, strPgdg, strPgsi);
		}
	}
}

[[gsl::suppress(26496)]]
void CNotify::CheckData()
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);

	if (pDisp)
	{
		USES_CONVERSION_CONST2;

		CComDispatchDriver driver(pDisp);

		VARIANT var;
		VariantInit(&var);
		driver.GetPropertyByName(L"Flag", &var);
		if (var.vt!=VT_BSTR) return;
		CString flagS = OLE2T(var.bstrVal);

		VariantClear(&var);
		driver.GetPropertyByName(L"DataList", &var);
		if (var.vt!=VT_BSTR) return;
		CString strData = OLE2T(var.bstrVal);

//				CString flagS = m_Icontrol->GetFlag();				
//					count \t code \t mmgb \t jgvl \n ......
//				CString	strData = m_Icontrol->GetDataList();

//TRACE("notify = %s\n", strData.Left(500));
		
		CString strJCod;
		CString strJango;
		CString strPgdg;
		CString strPgsi;
		CString strJggb;
		CString strJggb2;
		CString strCodeName;
		CString strPossCnt;
		CString strPprice;
		CString strPpercent;
		CString strSdate;

		CString stemp;
		CString strgubn;

CString slog;
slog.Format("[IB100300]notify [%s]\n", flagS);
//OutputDebugString(slog);


		if (flagS == "A")
		{
			CString strHead = Parser(strData, "\t");
			const int iCount = atoi(strHead);
			m_pMapwnd->DeleteAllJangoRow();  //��ü ��ȸ������
			
			for (int ii = 0; ii < iCount; ii++)
			{
				CString strLine = Parser(strData, "\n");
			
				//�����ڵ�, �����ܰ�, ������հ�, �򰡼���, �ſ��ڵ�, �ܰ���,    �����, ���ɼ���, �򰡱ݾ�, ���ͷ�, ������
				strJCod = Parser(strLine, "\t");	//�����ڵ�
				strJango = Parser(strLine, "\t");	//�����ܰ�
				strPgdg = Parser(strLine, "\t");	//������հ�
				strPgsi = Parser(strLine, "\t");	//�򰡼���
				strJggb = Parser(strLine, "\t");	//�ſ��ڵ�

				if(strJggb == _T("05"))  //��������ΰ�� ����
					continue;

				strJggb2 = Parser(strLine, "\t");	//�ܰ���

				strCodeName = Parser(strLine, "\t");	//�����
				strPossCnt = Parser(strLine, "\t");		//���ɼ���
				if(atoi(strPossCnt) <= 0)
					strPossCnt = "";
				strPprice = Parser(strLine, "\t");		//�򰡱ݾ�
				strPpercent = Parser(strLine, "\t");	//���ͷ�
				strSdate = Parser(strLine, "\t");		//������
				strSdate.TrimRight();

				strgubn.Format("%s%s%s%s", strJCod, strJggb2, strJggb, strSdate);    //�ڵ�, �ܰ���, �ſ뱸��, ������
				sary.SetAt(strgubn, strJCod+"\t"+strJango+"\t"+strPgdg+"\t"+strPgsi+"\t"+strJggb+"\t"+strJggb2+"\t"+strSdate);   //�ڵ�, �ܰ�, ������հ�, �򰡼���, �ſ��ڵ�, �ܰ���, ������

				stemp.Format("%s%s%s%s", m_pMapwnd->GetSiseCode(), m_pMapwnd->m_strJangoType, m_pMapwnd->m_strCreditType, m_pMapwnd->m_strLoanDate); //�ڵ� �ܰ��� �ſ뱸�� ������
				if(strgubn == stemp)
					m_pControlWnd->SetJango(strJCod, strJango, strPgdg, strPgsi);
					
				if(strSdate.GetLength() > 0)
					strSdate = strSdate.Left(4) + "/" + strSdate.Mid(4,2) + "/" + strSdate.Mid(6,2);
				m_pMapwnd->SetJangoFromDll(flagS, strJCod, strCodeName, strJggb2, strPossCnt, strPgdg, strPprice, strPgsi,  strPpercent, strJggb, strSdate);
/*
slog.Format("[1003]notify \n");
OutputDebugString(slog);
slog.Format("[1003]notify [%s] [%s] [%s] [%s]\n", strJCod, strJango, strPgdg, strPgsi );
OutputDebugString(slog);
slog.Format("[1003]notify [%s] stemp = [%s]\n", flagS, stemp);
OutputDebugString(slog);
slog.Format("[1003]notify [%s] strgubn = [%s]\n", flagS, strgubn);
OutputDebugString(slog);
*/

			}
		}
		else if (flagS != "E")
		{
			stemp = strData;
			CString keyS = strData.Left(LEN_TOTAL);
			strData = strData.Mid(LEN_TOTAL);
	
			if (flagS == "D")   
			{
				strJCod = keyS.Mid(LEN_JGGB, LEN_ACODE);
				strJCod.TrimRight();
				strJggb2 = keyS.Mid(0, 2);   //�ܰ���
				strSdate =  keyS.Mid(14, 8); //������
				strSdate.TrimRight();
				strJggb =  keyS.Mid(22, 2);  //�ſ뱸��

				if(strJggb == _T("05"))  //��������ΰ�� ����
					return;
				
				strgubn.Format("%s%s%s%s", strJCod, strJggb2, strJggb, strSdate);  //�ڵ�, �ܰ���, �ſ뱸��, ������
				CString tmp;
				if (sary.Lookup(strgubn, tmp))
					sary.RemoveKey(strgubn);

				stemp.Format("%s%s%s%s", m_pMapwnd->GetSiseCode(), m_pMapwnd->m_strJangoType, m_pMapwnd->m_strCreditType, m_pMapwnd->m_strLoanDate); //�ڵ� �ܰ��� �ſ뱸�� ������
/*
slog.Format("[1003]notify \n");
OutputDebugString(slog);
slog.Format("[1003]notify [%s] [%s] [%s] [%s]\n", strJCod, strJango, strPgdg, strPgsi );
OutputDebugString(slog);
slog.Format("[1003]notify D stemp = [%s]\n", stemp);
OutputDebugString(slog);
slog.Format("[1003]notify D strgubn = [%s]\n", strgubn);
OutputDebugString(slog);
*/
				if(strgubn == stemp)
					m_pControlWnd->JangoClear(strJCod);

				if(strSdate.GetLength() > 0)
					strSdate = strSdate.Left(4) + "/" + strSdate.Mid(4,2) + "/" + strSdate.Mid(6,2);
				m_pMapwnd->SetJangoFromDll(flagS, strJCod, strCodeName, strJggb2, strPossCnt, strPgdg, strPprice, strPgsi,  strPpercent, strJggb, strSdate);
			}
			else if (flagS == "U" || flagS == "I")
			{
				strJCod = Parser(strData, "\t");
				CString tmp;
			
				strJango = Parser(strData, "\t");
				strPgdg = Parser(strData, "\t");   //������հ�
				strPgsi = Parser(strData, "\t");   //�򰡼���
				strJggb = Parser(strData, "\t");  
				strJggb2 = Parser(strData, "\t");

				strCodeName = Parser(strData, "\t");	//�����
				strPossCnt = Parser(strData, "\t");		//���ɼ���
				if(atoi(strPossCnt) <= 0)
					strPossCnt = "";
				strPprice = Parser(strData, "\t");		//�򰡱ݾ�
				strPpercent = Parser(strData, "\t");	//���ͷ�
				strSdate = Parser(strData, "\t");		//������
				strSdate.TrimRight();

				if(strJggb2 == _T("05"))  //��������ΰ�� ����
					return;

				strgubn.Format("%s%s%s%s", strJCod, strJggb2, strJggb, strSdate);    //�ڵ�, �ܰ���, �ſ뱸��, ������
				sary.SetAt(strgubn, strJCod+"\t"+strJango+"\t"+strPgdg+"\t"+strPgsi+"\t"+strJggb+"\t"+strJggb2+"\t"+strSdate);   //�ڵ�, �ܰ�, ������հ�, �򰡼���, �ſ��ڵ�, �ܰ���, ������

				if(strSdate.GetLength() > 0)
					strSdate = strSdate.Left(4) + "/" + strSdate.Mid(4,2) + "/" + strSdate.Mid(6,2);
				m_pMapwnd->SetJangoFromDll(flagS, strJCod, strCodeName, strJggb2, strPossCnt, strPgdg, strPprice, strPgsi,  strPpercent, strJggb, strSdate);

				stemp.Format("%s%s%s%s", m_pMapwnd->GetSiseCode(), m_pMapwnd->m_strJangoType, m_pMapwnd->m_strCreditType, m_pMapwnd->m_strLoanDate); //�ڵ� �ܰ��� �ſ뱸�� ������
/*
slog.Format("[1003]notify \n");
OutputDebugString(slog);
slog.Format("[1003]notify [%s] [%s] [%s] [%s]\n", strJCod, strJango, strPgdg, strPgsi );
OutputDebugString(slog);
slog.Format("[1003]notify [%s]   stemp = [%s]\n", flagS, stemp);
OutputDebugString(slog);
slog.Format("[1003]notify [%s] strgubn = [%s]\n", flagS, strgubn);
OutputDebugString(slog);
*/
				if(strgubn == stemp)
					m_pControlWnd->SetJango(strJCod, strJango, strPgdg, strPgsi);
			}	
		}
		else if (flagS == "E")
		{
			m_pMapwnd->DeleteAllJangoRow();
		}
		
		if (m_pControlWnd->GetWaitMode())
			m_pControlWnd->SetWaitMode(FALSE);
	}
}

void CNotify::SendJango(CString acc, CString pswd, CString param3, CString param4, bool bjango)
{
	CString slog;

//	if (m_Icontrol == NULL)
	if(!m_Wcontrol) return;
	if(!m_Wcontrol->GetSafeHwnd()) return;

	if (m_acnt == acc && !bjango)
	{
slog.Format("[1003] ---- SendJango->SetJango  acc=[%s] bjango=[%d]\n", acc, bjango);
OutputDebugString(slog);
		SetJango();
		return;
	}

slog.Format("[1003] ---- SendJango  acc=[%s] bjango=[%d]\n", acc, bjango);
OutputDebugString(slog);

	m_acnt = acc;
	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);

	if (pDisp == NULL)
		return;

	CComDispatchDriver driver(pDisp);

//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = acc.AllocSysString();

	driver.PutPropertyByName(L"SetAccount", &var);

	if (pswd.IsEmpty())
		return;

	_variant_t varArgs[4] = { param4, param3, pswd, acc };

	const HRESULT hr = driver.InvokeN(L"Send", &varArgs[0], 4);
	if (FAILED(hr))
		TRACE("CNotifyCtrl.Send is failed. HR[%X]", hr);
	else
		m_pMapwnd->DeleteAllJangoRow();
//	m_Wcontrol->InvokeHelper(DISPID_SEND, DISPATCH_METHOD, VT_EMPTY, NULL, (BYTE*)acc.AllocSysString(), (BYTE*)pswd.AllocSysString());
//	m_Icontrol->Send(acc, pswd);
}

CString CNotify::Parser(CString &srcstr, CString substr)
{
	CString temp;
	temp.Empty();
	if (srcstr.Find(substr) == -1)
	{
		temp = srcstr;
		srcstr.Empty();
	}
	else
	{
		temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	}

	temp.TrimLeft();
	temp.TrimRight();
	return temp;
}

void CNotify::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	CWnd::OnTimer(nIDEvent);
}
