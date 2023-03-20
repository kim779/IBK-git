// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_MARKETMAP.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pWnd, _param* param)
{
	EnableAutomation();

	m_pWnd = pWnd;

	m_szName  = CString(param->name);
	m_key	  = param->key;
	m_rect    = param->rect;

	m_fMaxRate = 6.0f;

	char	szHex[16]{};
// �ְ�(������) RGB(237,28,36)
// ����(Ǫ����) RGB(3,32,250)

	strcpy(szHex ,"0x241CED");
	m_nUpColor = (int)strtol(szHex, NULL,16);

	strcpy(szHex ,"0xFA2003");
	m_nDownColor = (int)strtol(szHex, NULL,16);

	strcpy(szHex ,"0xFDFDFD");
	m_nEqualColor = (int)strtol(szHex, NULL,16);

	m_fgColor = RGB(255,206,214);
	m_ggColor = RGB(198,247,231);
	m_psColor = RGB(255,247,206);

	m_nMarketType = 0;
	m_nViewType = 0;

	m_nSearchType = ctUP;	//0 ���� ����   1 ���� ����
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
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WD_UPDBCLICK, OnUpDBClick)
	ON_MESSAGE(WD_TRIGGERCC, OnRButtonClick)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "MakeMap", MakeMap, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CMainWnd, "LoadMap", LoadMap, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CMainWnd, "LoadUpMap", LoadUpMap, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SearchByUp", SearchByUp, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "redo", redo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetDataArr", GetDataArr, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "Redraw", Redraw, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8201BFF2-5214-424B-81A8-2A53F8B1A0B5}
static const IID IID_IMainWnd =
{ 0x8201bff2, 0x5214, 0x424b, { 0x81, 0xa8, 0x2a, 0x53, 0xf8, 0xb1, 0xa0, 0xb5 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

void CMainWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

}

BSTR CMainWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

// struct jidogrid_mod 
// {
// 	char upcode[3];
// 	char upname[20];
// 	char xcode[12];
// 	char hname[40];
// 	char rate[7];
// 	char pers[8];
// 	char forg[8];
// 	char gign[8];
// };
LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	CString str;

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		if (exth->size == 0)
			break;
		if (exth->key == KEY_2133)
			Parse2133(exth->data, exth->size);
		else
			ParseOub(exth->data, exth->size);
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!
		{
			str = (char*)lParam;
			if (HIWORD(wParam) == 0 || str.IsEmpty())
				break;

			const	int	igubn = HIBYTE(LOWORD(wParam));

			if (igubn == 0)
				Parse2133((char *)lParam, HIWORD(wParam));
			else
				ParseOub((char *)lParam, HIWORD(wParam));
		}
		break;
	case DLL_ALERTx:
	case DLL_NOTICE:
	case DLL_SETPAL:
	case DLL_TRIGGER:
	case DLL_DOMINO:
	case DLL_GUIDE:
	case DLL_SETFONT:
	case DLL_SETFONTx:
	default:
		break;
	}

	return 0;
}

void CMainWnd::Parse2133(char* pData, int nData)
{
	CRect	rc;
	struct pibo2133_mod* mod = (struct pibo2133_mod*)pData;
	const	int	nCnt = atoi(CString(mod->nrec, sizeof(mod->nrec)));
	CString strUp, strUpName, strRate, strAmt;
	CString strSiga, strPers, strForg, strGign;
	std::shared_ptr<CUpGroup> upGroup = nullptr;
	std::shared_ptr<CUpGroup> detail = nullptr;

	for (int ii = 0; ii < nCnt; ii++)
	{
		strUp = CString(mod->grid[ii].upcode,sizeof(mod->grid[ii].upcode));
		strUp.Trim();

		strUpName = CString(mod->grid[ii].upname,sizeof(mod->grid[ii].upname));
		strUpName.Trim();

		strRate = CString(mod->grid[ii].rate,sizeof(mod->grid[ii].rate));
		strRate.Trim();

		strAmt = CString(mod->grid[ii].amt,sizeof(mod->grid[ii].amt));
		strAmt.Trim();

		strSiga = CString(mod->grid[ii].siga,sizeof(mod->grid[ii].siga));
		strSiga.Trim();
		
		strPers = CString(mod->grid[ii].pers,sizeof(mod->grid[ii].pers));	//���μ��ż��� 	
		strPers.Trim();
		
		strForg = CString(mod->grid[ii].forg,sizeof(mod->grid[ii].forg));	//�ܱ��μ��ż���
		strForg.Trim();
		
		strGign = CString(mod->grid[ii].gign,sizeof(mod->grid[ii].gign));	//������ż���
		strGign.Trim();

		if (!m_mapChild.Lookup(strUp, upGroup))
		{
			upGroup = std::make_shared<CUpGroup>();
			upGroup->Create(NULL, "CTRLWND", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, -1);
			upGroup->ShowWindow(SW_SHOW);

			upGroup->SetUPCode(strUp);
			upGroup->SetUpName(strUpName);
			
			upGroup->SetViewType(m_nViewType);
			upGroup->SetScale(m_nScale);
			upGroup->SetStandard(m_nStandard);
			
			upGroup->SetSiga(strSiga);						//�ð��Ѿ� ���
			upGroup->SetAmount(strAmt);						//�ŷ��� 
		
			upGroup->SetForg(strForg);						//�ܱ��μ��ż���
			upGroup->SetGign(strGign);						//������ż���
			upGroup->SetPers(strPers);						//���θż���
		
			upGroup->SetSumSgTotal(strSiga);				//�ش� ���� �ð��Ѿ� �ջ� 
			upGroup->SetSumAmt(strAmt);						//�ش� ���� �ŷ��� �ջ�
		
			upGroup->SetSumQnt(strGign,strForg,strPers);
			upGroup->SetSumAmtFore(strForg);				//�ش� ���� �ܱ��� ���ż��� �ջ�
			upGroup->SetSumAmtGign(strGign);				//�ش� ���� ��� ���ż��� �ջ�
	
			m_dTotalSiga += atof(strSiga);					//��ü �ð��Ѿ� �հ� 
			m_dTotalAmt += atof(strAmt);					//��ü  �ŷ���  �հ�
			m_dTotalFore += atof(strForg);					//��ü  �ܱ��� ���ż���   �հ�
			m_dTotalGi += atof(strGign);					//��ü  ��� ���ż���   �հ�
			
			upGroup->SetParent(this);

			GetClientRect(&rc);
			upGroup->SetParentRect(rc);

			detail = std::make_shared<CUpGroup>();
			detail->SetUPCode(strUp);
			detail->SetUpName(strUpName);
	
			detail->SetRate(strRate);
			detail->SetSgTotal(strSiga);
			detail->SetSumSgTotal(strSiga);
			detail->SetAmount(strAmt);
			detail->SetSumAmt(strAmt);
	
			detail->SetPers(strPers);
			detail->SetForg(strForg);			
			detail->SetGign(strGign);			
			detail->SetSiga(strSiga);
		
			m_mapChild.SetAt(strUp, upGroup);

			upGroup->PutData(detail);
			m_arrChild.Add(upGroup);
		}
		else  //�����ڵ�Ű ���� ���� �Ǿ������
		{
			upGroup->SetSumQnt(strGign,strForg,strPers);
			upGroup->SetSumSgTotal(strSiga);	  //�ش� ���� �ð��Ѿ� �ջ� 
			upGroup->SetSumAmt(strAmt);			  //�ش� ���� �ŷ��� �ջ�
			upGroup->SetSumAmtFore(strForg);	  //�ش� ���� �ܱ��� ���ż��� �ջ�
			upGroup->SetSumAmtGign(strGign);	  //�ش� ���� ��� ���ż��� �ջ�
		
			m_dTotalSiga += atof(strSiga);		  //��ü  �ð��Ѿ� �հ� 
			m_dTotalAmt += atof(strAmt);		  //��ü  �ŷ���  �հ�
			m_dTotalFore += atof(strForg);		  //��ü  �ܱ��� ���ż���   �հ�
			m_dTotalGi += atof(strGign);		  //��ü  ��� ���ż���   �հ�

			detail = std::make_shared<CUpGroup>();
			detail->SetUPCode(strUp);
			detail->SetUpName(strUpName);
			detail->SetRate(strRate);
			detail->SetSgTotal(strSiga);
			detail->SetSumSgTotal(strSiga);
			detail->SetAmount(strAmt);
			detail->SetSumAmt(strAmt);
			detail->SetPers(strPers);
			detail->SetForg(strForg);		
			detail->SetGign(strGign);	
			detail->SetSiga(strSiga);
	
			upGroup->PutData(detail);
		}

		if (m_nScale == 0)
			updateRect();
		else if (m_nScale == 1)  //0 ���� 1 ����
			updateRectByRate();
		
		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnClick)), 
					(LPARAM)m_szName.GetString());
	}
	m_bSending = false;
}

void CMainWnd::ParseOub(char* pData, int nData)
{
	CRect	rc;
	struct pidojid2_mod* mod = (struct pidojid2_mod*)pData;
	const	int nCnt = atoi(CString(mod->rcnt, sizeof(mod->rcnt)));
	CString strUp, strUpName, strCode, strName;
	CString	strRate, strAmt, strMny;
	CString	strCurr, strDiff;
	CString strSiga, strPers, strForg, strGign;

	std::shared_ptr<CUpGroup> upGroup = nullptr;
	std::shared_ptr<CUpGroup> detail = nullptr;

	m_dTotalSiga = 0;
	m_dTotalAmt  = 0;
	m_dTotalMny = 0;
	m_dTotalFore = 0;
	m_dTotalGi = 0;

	for (int ii = 0; ii < nCnt; ii++)
	{
		strUp = CString(mod->grid[ii].upcode,sizeof(mod->grid[ii].upcode));
		strUp.Trim();

		strUpName = CString(mod->grid[ii].upname,sizeof(mod->grid[ii].upname));
		strUpName.Trim();

		strCode = CString(mod->grid[ii].xcode,sizeof(mod->grid[ii].xcode));
		strCode.Trim();
		strCode.Remove('A');

		strName = CString(mod->grid[ii].hname,sizeof(mod->grid[ii].hname));
		strName.Trim();

		strRate = CString(mod->grid[ii].rate,sizeof(mod->grid[ii].rate));
		strRate.Trim();

		strAmt = CString(mod->grid[ii].amt,sizeof(mod->grid[ii].amt));
		strAmt.Trim();

		strMny = CString(mod->grid[ii].mny,sizeof(mod->grid[ii].mny));
		strMny.Trim();

		strSiga = CString(mod->grid[ii].siga,sizeof(mod->grid[ii].siga));
		strSiga.Trim();

		strCurr = CString(mod->grid[ii].curr,sizeof(mod->grid[ii].curr));
		strCurr.Trim();

		strDiff = CString(mod->grid[ii].diff,sizeof(mod->grid[ii].diff));
		strDiff.Trim();
		
		strPers = CString(mod->grid[ii].pers,sizeof(mod->grid[ii].pers));	//���μ��ż��� 	 1000
		strPers.Format("%.0f", atof(strPers)/1000);
		strPers.Trim();
		
		strForg = CString(mod->grid[ii].forg,sizeof(mod->grid[ii].forg));	//�ܱ��μ��ż���
		strForg.Format("%.0f", atof(strForg)/1000);
		strForg.Trim();

		strGign = CString(mod->grid[ii].gign,sizeof(mod->grid[ii].gign));	//������ż���
		strGign.Format("%.0f", atof(strGign)/1000);
		strGign.Trim();
	
		if (!m_mapChild.Lookup(strUp, upGroup))
		{
			upGroup = std::make_shared<CUpGroup>();
			upGroup->Create(NULL, "CTRLWND", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, -1);
			upGroup->ShowWindow(SW_SHOW);

			upGroup->SetUPCode(strUp);
			upGroup->SetUpName(strUpName);
			
			upGroup->SetViewType(m_nViewType);
			upGroup->SetScale(m_nScale);
			upGroup->SetStandard(m_nStandard);
			
			upGroup->SetSiga(strSiga);					//�ð��Ѿ� ���
			upGroup->SetAmount(strAmt);					//�ŷ��� 
			upGroup->SetMoney(strMny);					//�ŷ����
			
			upGroup->SetForg(strForg);					//�ܱ��μ��ż���
			upGroup->SetGign(strGign);					//������ż���
			upGroup->SetPers(strPers);					//���θż���
		
			
			upGroup->SetSumSgTotal(strSiga);				//�ش� ���� �ð��Ѿ� �ջ� 
			upGroup->SetSumAmt(strAmt);						//�ش� ���� �ŷ��� �ջ�
			upGroup->SetSumMoney(strMny);					//�ش� ���� �ŷ���� �ջ�

			upGroup->SetSumQnt(strGign,strForg,strPers);
			upGroup->SetSumAmtFore(strForg);				//�ش� ���� �ܱ��� ���ż��� �ջ�
			upGroup->SetSumAmtGign(strGign);				//�ش� ���� ��� ���ż��� �ջ�
		
			upGroup->SetCurr(strCurr);
			upGroup->SetDiff(strDiff);

			m_dTotalSiga += atof(strSiga);					//��ü �ð��Ѿ� �հ� 
			m_dTotalAmt += atof(strAmt);					//��ü  �ŷ���  �հ�
			m_dTotalMny += atof(strMny);					//��ü  �ŷ����  �հ�
			m_dTotalFore += atof(strForg);					//��ü  �ܱ��� ���ż���   �հ�
			m_dTotalGi += atof(strGign);					//��ü  ��� ���ż���   �հ�
			
			upGroup->SetParent(this);

			GetClientRect(&rc);
			upGroup->SetParentRect(rc);

			detail = std::make_shared<CUpGroup>();
			detail->SetUPCode(strUp);
			detail->SetUpName(strUpName);
			detail->SetCode(strCode);
			detail->SetName(strName);
			detail->SetRate(strRate);
			detail->SetSgTotal(strSiga);
			detail->SetSumSgTotal(strSiga);
			detail->SetAmount(strAmt);
			detail->SetSumAmt(strAmt);
			detail->SetMoney(strMny);
			detail->SetSumMoney(strMny);
			detail->SetPers(strPers);
			detail->SetForg(strForg);			
			detail->SetGign(strGign);			
			detail->SetSiga(strSiga);
			detail->SetCurr(strCurr);
			detail->SetDiff(strDiff);	
		
			m_mapChild.SetAt(strUp,upGroup);

			upGroup->PutData(detail);
			m_arrChild.Add(upGroup);
		}
		else  //�����ڵ�Ű ���� ���� �Ǿ������
		{
			upGroup->SetSumQnt(strGign,strForg,strPers);
			upGroup->SetSumSgTotal(strSiga);	  //�ش� ���� �ð��Ѿ� �ջ� 
			upGroup->SetSumAmt(strAmt);			  //�ش� ���� �ŷ��� �ջ�
			upGroup->SetSumMoney(strMny);		  //�ش� ���� �ŷ���� �ջ�
			upGroup->SetSumAmtFore(strForg);	  //�ش� ���� �ܱ��� ���ż��� �ջ�
			upGroup->SetSumAmtGign(strGign);	  //�ش� ���� ��� ���ż��� �ջ�
		
		
			m_dTotalSiga += atof(strSiga);		  //��ü  �ð��Ѿ� �հ� 
			m_dTotalAmt += atof(strAmt);		  //��ü  �ŷ���  �հ�
			m_dTotalMny += atof(strMny);		  //��ü ���� �ŷ����  �հ�
			m_dTotalFore += atof(strForg);		  //��ü  �ܱ��� ���ż���   �հ�
			m_dTotalGi += atof(strGign);		  //��ü  ��� ���ż���   �հ�

			detail = std::make_shared<CUpGroup>();
			detail->SetUPCode(strUp);
			detail->SetUpName(strUpName);
			detail->SetCode(strCode);
			detail->SetName(strName);
			detail->SetRate(strRate);
			detail->SetSgTotal(strSiga);
			detail->SetSumSgTotal(strSiga);
			detail->SetAmount(strAmt);
			detail->SetSumAmt(strAmt);
			detail->SetMoney(strMny);
			detail->SetSumMoney(strMny);
			detail->SetPers(strPers);
			detail->SetForg(strForg);		
			detail->SetGign(strGign);	
			detail->SetSiga(strSiga);
			detail->SetCurr(strCurr);
			detail->SetDiff(strDiff);
		
			upGroup->PutData(detail);
		}
	}

	if (m_nSearchType != ctSTOCK)
	{
		if (m_nScale == 0)
			updateRect();
		else if (m_nScale == 1)  //0 ���� 1 ����
			updateRectByRate();

		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnClick)),
				(LPARAM)m_szName.GetString());
	}
	else if (m_nSearchType == ctSTOCK && !m_strUp.IsEmpty())
	{
		CString key;
		const	int	itmpType = m_nViewType;		//0 ��� 1 ����
		const	int	itmpScale = m_nScale;		//0 ���� 1 ����
		const	int	itmpStandard = m_nStandard;	//0 �ð��Ѿ�, 1 �ŷ���, 2 �ŷ���� 3 �ܱ��� ���ż���  4 �ܱ��� ���ż����

		m_nScale = 1;
		for (POSITION pos = m_mapChild.GetStartPosition(); pos;)	
		{
			m_mapChild.GetNextAssoc(pos, key, upGroup);
			upGroup->SetScale(m_nScale);
			upGroup->SetViewType(m_nViewType);
			upGroup->SetStandard(m_nStandard);
		}
		
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			upGroup = m_arrChild.GetAt(ii);
			upGroup->SetScale(m_nScale);
			upGroup->SetViewType(m_nViewType);
			upGroup->SetStandard(m_nStandard);
		}
		
		updateRectByRate();
		updateColor();
		
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			upGroup = m_arrChild.GetAt(ii);
			if (upGroup->GetUPCode() == m_strUp)
			{
				upGroup->SendMessage(WM_LBUTTONDBLCLK, 0, MAKELONG(0, 0));
			}
		}	

		m_strUp.Empty();
		m_nViewType = itmpType;		// 0 ��� 1 ����  
		m_nScale = itmpScale;		// 0 ���� 1 ����
		m_nStandard = itmpStandard;	// 0 �ð��Ѿ�, 1 �ŷ���, 2 �ŷ���� 3 �ܱ��� ���ż���  4 �ܱ��� ���ż����

		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnChange)),
					(LPARAM)m_szName.GetString());
	}

	m_bSending = false;
}

afx_msg LONG CMainWnd::OnUpDBClick(WPARAM wParam, LPARAM lParam)
{
	CString strUp = (char*)lParam;
	const	int	nSelected = (int)wParam;

	if (nSelected == 1)
	{  //�����ڵ� ���� ��Ȳ���� ����Ŭ����
		std::shared_ptr<CUpGroup> item = nullptr;
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			item = m_arrChild.GetAt(ii);
			if (item->GetUPCode() != strUp)
				item->ShowWindow(SW_HIDE);
			else 
				item->ShowWindow(SW_SHOW);
		}
	}
	else if (nSelected == 2)
	{
		if (m_nViewType == 0)
		{
			CString string;
			string.Format("%s/t/S/d%s\t%s\n%s\t%s\n", "IB200100", "1301", strUp, "", "");
			openView(typeVIEW, string);
		}	
	}
	else if (nSelected == 0)
	{  // �����ڵ� ���� ��Ȳ���� ����Ŭ����
	
	}

	return 0;
}

void CMainWnd::Redraw(short type1, short type2, short type3, short type4, LPCTSTR code) 
{
	CString key, stmp;
	
	m_nViewType = type1;	//0 ��� 1 ����
	m_nScale    = type2;	//0 ���� 1 ����
	m_nStandard = type3;	//0 �ð��Ѿ�, 1 �ŷ���, 2 �ŷ���� 3 �ܱ��� ���ż���  4 �ܱ��� ���ż����

	std::shared_ptr<CUpGroup> upGroup = nullptr;
	for (POSITION pos = m_mapChild.GetStartPosition(); pos;)	
	{
		m_mapChild.GetNextAssoc(pos, key, upGroup);
		upGroup->SetScale(m_nScale);
		upGroup->SetViewType(m_nViewType);
		upGroup->SetStandard(m_nStandard);
	}
	
	for (int ii = 0;ii < m_arrChild.GetSize();ii++)
	{
		upGroup = m_arrChild.GetAt(ii);
		upGroup->SetScale(m_nScale);
		upGroup->SetViewType(m_nViewType);
		upGroup->SetStandard(m_nStandard);
	}
	
	if (m_nScale == 0)
		updateRect();
	else if (m_nScale == 1)  
		updateRectByRate();
	
	if (type4 == 1)
	{
		stmp = CString(code);
		stmp.Trim();
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			upGroup = m_arrChild.GetAt(ii);
			if (upGroup->GetUPCode() == stmp)
			{	
				upGroup->m_bSelected = false;
				upGroup->SendMessage(WM_LBUTTONDBLCLK, 0, MAKELONG(0, 0));
			}
		}	
	}

	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnDblClk)), 
				(LPARAM)m_szName.GetString());
}

void CMainWnd::SendTR(CString name, CString data, BYTE type, int key)
{
	const	int	datL = data.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datL + L_userTH + 128);
	struct _userTH udat {};
	int	idx = m_szName.GetLength();

	// control name
	CopyMemory(pWb.get(), m_szName, idx);
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, name, min(sizeof(udat.trc), name.GetLength()));
	udat.key  = key;
	udat.stat = type;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], data, datL);
	
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)pWb.get());
	pWb.reset();
}

void CMainWnd::sendTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey)
{ 	
	const	int	datL = data.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datL + L_userTH + 128);
	struct _userTH udat {};
	int	idx = 0;

	// key & control name
	pWb[idx++] = uniqueKey;
	CopyMemory(&pWb[idx], m_szName, m_szName.GetLength());
	idx += m_szName.GetLength();
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, name, min(sizeof(udat.trc), name.GetLength()));
	udat.key  = key; 
	udat.stat = type;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], data, datL);
	
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)pWb.get());
	pWb.reset();
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());
	
	if ((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

void CMainWnd::updateColor()
{
	if (m_nViewType == 0)  //���
	{
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			m_arrChild.GetAt(ii)->updateChildColorByRate(m_nUpColor,m_nDownColor,m_nEqualColor,m_fMaxRate);
		}
	}
	else    //����
	{
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			m_arrChild.GetAt(ii)->updateChildColorBySum(m_ggColor,m_fgColor,m_psColor);
		}
	}
}

void CMainWnd::updateRect()
{
	CRect	rect;

	GetClientRect(rect);
	updateRect(m_arrChild,rect);
	if (m_nViewType == 0)  //0 ���, 1 ����
	{
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			m_arrChild.GetAt(ii)->updateRect();
		}
	}

	updateColor();
	draw();
}

void CMainWnd::updateRectByRate()
{
	CRect	rect;
	
	GetClientRect(rect);
	Quick_Sort(m_arrChild,m_arrChild.GetSize(), m_nStandard);	//0 �ð��Ѿ� 1 �ŷ��� 2 �ŷ���� 3 �ܱ��� ���ż� 4 �ܱ��� ���ż����
	updateRectByRate(m_arrChild, rect);
	
	if (m_nViewType == 0)	//0 ���  1 ����
	{
		for (int ii = 0;ii < m_arrChild.GetSize();ii++)
		{
			m_arrChild.GetAt(ii)->updateRectByRate();
		}
	}
	
	updateColor();
	draw();
}

void CMainWnd::updateRectByRate(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect)
{
	if (arrInfo.GetSize() < 1)
		return;
	
	if (arrInfo.GetSize() == 1)
	{
		arrInfo.GetAt(0)->SetRect(rect);
		return;
	}
	
	double	totalData =0.0f;
	for (int ii = 0; ii < arrInfo.GetSize() ; ii++)
	{
		totalData += arrInfo.GetAt(ii)->GetSortData(m_nStandard);
	}

	double upData = 0.0f;
	double downData = 0.0f;
	std::shared_ptr<CUpGroup> info = nullptr;
	CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> arrInfoup;
	CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> arrInfodown;

	if (arrInfo.GetSize() == 2)
	{
		info = arrInfo.GetAt(0);
		upData += info->GetSortData(m_nStandard);
		arrInfoup.Add(info);
		
		info = arrInfo.GetAt(1);
		downData += info->GetSortData(m_nStandard);
		arrInfodown.Add(info);
	}
	else
	{
		for (int ii = 0; ii < arrInfo.GetSize();ii++)
		{
			info = arrInfo.GetAt(ii);
			if ((upData * 2) < totalData)
			{
				upData += info->GetSortData(m_nStandard);
				arrInfoup.Add(info);
			}
			else
			{
				downData += info->GetSortData(m_nStandard);
				arrInfodown.Add(info);
			}
		}
	}
/*  
	if (upData < 0)
		upData = 0;
	if (downData < 0)
		downData = 0;
*/
	CRect	upRect,downRect;
	
	CopyRect(upRect,rect);
	CopyRect(downRect,rect);
	if (rect.Width() < rect.Height())
	{
		upRect.bottom = upRect.top + (int)(rect.Height() * (upData/totalData));
		downRect.top = upRect.bottom;
		downRect.bottom = rect.bottom;
	}
	else
	{
		upRect.right = upRect.left + (int)(rect.Width() * (upData/totalData));
		downRect.left = upRect.right;
		downRect.right = rect.right;
	}
	
	if (upData == 0)
	{
		updateRect(arrInfoup,upRect);
	}
	else
	{
		updateRectByRate(arrInfoup, upRect);
	}

	if (downData == 0)
	{
		updateRect(arrInfodown,downRect);
	}
	else
	{
		updateRectByRate(arrInfodown, downRect);
	}
}

void CMainWnd::updateRect(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect)
{
	if (arrInfo.GetSize() < 1)
		return;

	if (arrInfo.GetSize() == 1)
	{
		arrInfo.GetAt(0)->SetRect(rect);
		return;
	}

	const	double	totalCnt = arrInfo.GetSize();
	double upData = 0.0f;
	double downData = 0.0f;
	std::shared_ptr<CUpGroup> info = nullptr;
	CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> arrInfoup;
	CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> arrInfodown;

	if (arrInfo.GetSize() == 2)
	{
		info = arrInfo.GetAt(0);
		upData += 1;
		arrInfoup.Add(info);

		info = arrInfo.GetAt(1);
		downData += 1;
		arrInfodown.Add(info);
	}
	else
	{
		for (int ii = 0; ii < arrInfo.GetSize();ii++)
		{
			info = arrInfo.GetAt(ii);
			if ((upData*2) < totalCnt)
			{
				upData += 1;
				arrInfoup.Add(info);
			}
			else
			{
				downData += 1;
				arrInfodown.Add(info);
			}
		}
	}

	CRect	upRect,downRect;

 	CopyRect(upRect,rect);
	CopyRect(downRect,rect);

	if (rect.Width() < rect.Height())
	{
		upRect.bottom = upRect.top + (int)(rect.Height() * (upData/totalCnt));

		downRect.top = upRect.bottom;
		downRect.bottom = rect.bottom;
	}
	else
	{
		upRect.right = upRect.left + (int)(rect.Width() * (upData/totalCnt));

		downRect.left = upRect.right;
		downRect.right = rect.right;
	}

	updateRect(arrInfoup, upRect);
	updateRect(arrInfodown, downRect);
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC*	pDC = &dc;
	CRect	cRc;
	CBrush	brush;

	GetWindowRect(cRc);
	//cRc -= cRc.TopLeft();
	brush.CreateSolidBrush(RGB(250,250,251)); 
	pDC->FillRect(&cRc, &brush);
	brush.DeleteObject();

// 	for (int ii = 0;ii < m_arrChild.GetSize();ii++)
// 	{
// 		CUpGroup* item = (CUpGroup*)m_arrChild.GetAt(ii);
// 		
// 		CPen	cPen(PS_SOLID, 1, RGB(0,0,0));
// 		
// 		CPen *sPen = pDC->SelectObject(&cPen);
// 		// 	pDC->MoveTo(iRc.left - 2, iRc.top);
// 		// 	pDC->LineTo(iRc.left - 2 + m_buttonW - 4, iRc.top);
// 		// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top);
// 		// 	pDC->LineTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 		// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 		// 	pDC->LineTo(iRc.left - 2 ,iRc.top + m_buttonH -7);
// 		// 	pDC->MoveTo(iRc.left - 2 ,iRc.top + m_buttonH - 7);
// 		// 	pDC->LineTo(iRc.left - 2 , iRc.top);
// 		POINT pt;
// 		pt.x = 1;
// 		pt.y = 1;
// 		
// 		pDC->RoundRect(item->GetRect(),pt);
// 	}
	// Do not call CWnd::OnPaint() for painting messages
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CMainWnd::MakeMap(short type) 
{
	//SendTR("PIDOJIDO","00",US_KEY,m_key);
	//sendTR("PIDOJIDO","00",US_KEY,m_key,m_szName,m_key);

}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

void CMainWnd::draw()
{
	CRect	rc;
	std::shared_ptr<CUpGroup> item = nullptr;

	for (int ii = 0;ii < m_arrChild.GetSize();ii++)
	{
		item = m_arrChild.GetAt(ii);
		rc = item->GetRect();
		item->MoveWindow(rc, TRUE);
	}
}

void CMainWnd::OnClose() 
{
	CWnd::OnClose();
}

void CMainWnd::LoadMap(short market, short type, short type2, short scale) 
{
	if (m_bSending == true)
		return;

	//market 0 �ڽ���,	1 �ڽ���
	//type   0 ���,	1 ����
	//type2  0 �ð��Ѿ�, 1 �ŷ���, 2 �ŷ���� 3 �ܱ��� ���ż���  4 �ܱ���
	//scale  0 ����  ,   1 ����
	m_nMarketType = market;
	m_nViewType = type;
	m_nStandard = type2;
	m_nScale    = scale;

	clear();

	m_nSearchType = ctUP;
	m_bSending = true;
	if (type == 0)
	{
		if (market == 0)
			sendTR("PIDOJID2","00", US_KEY, m_key, m_szName, KEY_JID2);
		else if (market == 1)	                       		 
			sendTR("PIDOJID2","10", US_KEY, m_key, m_szName, KEY_JID2);
	}				                       		 
	else				                       		 
	{				                       		 
		if (market == 0)	                       		 
			sendTR("pibo2133","00", US_KEY, m_key, m_szName, KEY_2133);
		else if (market == 1)	                       		 
			sendTR("pibo2133","10", US_KEY, m_key, m_szName, KEY_2133);
	}
}

void CMainWnd::LoadUpMap(LPCTSTR sUpcode) 
{
	clear();

	const	int	datL = strlen(sUpcode);
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datL + L_userTH + 128);
	struct	_userTH	udat{};
	int	idx = 0;

	// key & control name
	pWb[idx++] = KEY_LOADUPMAP;
	CopyMemory(&pWb[idx], m_szName, m_szName.GetLength());
	idx += m_szName.GetLength();
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, TR_PIBO2911, sizeof(udat.trc));
	udat.key  = m_key;
	udat.stat = US_KEY;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], sUpcode, datL);
	
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)pWb.get());
	pWb.reset();
}

void CMainWnd::OnDestroy()
{
	clear();
	CWnd::OnDestroy();
}

LRESULT CMainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		break;
	default:
		break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CMainWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnMouseMove(nFlags, point);
}

void CMainWnd::Quick_Sort(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int count, int type)
{
	Quick_Recursive(arrInfo, 0, count-1, type);
}

void CMainWnd::Quick_Recursive(CArray<std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int left, int right , int type)
{
	const	int	left_hold = left;
	const	int	right_hold = right;
	std::shared_ptr<CUpGroup> upGroup = arrInfo.GetAt(left);
	double	pivot = upGroup->GetSortData(type);

	while (left < right)
	{
		while ((arrInfo.GetAt(right)->GetSortData(type) <= pivot) && (left < right))
			right--;

		if (left != right)
		{
			arrInfo.SetAt(left,arrInfo.GetAt(right));
		}

		while ((arrInfo.GetAt(left)->GetSortData(type) >= pivot) && (left < right))
			left++;

		if (left != right)
		{
			arrInfo.SetAt(right,arrInfo.GetAt(left));
			right--;
		}
	}

	arrInfo.SetAt(left,upGroup);
	pivot = left;
	left  = left_hold;
	right = right_hold;

	if (left < pivot) 
		Quick_Recursive(arrInfo,left,(int)pivot-1,type);

	if (right > pivot) 
		Quick_Recursive(arrInfo,(int)pivot+1,right,type);
}

LONG CMainWnd::OnRButtonClick(WPARAM wParam,LPARAM lParam)
{
	//Variant(triggerCC,(char*)lParam);
	/*
	CString str,tmp;
	tmp = (char*)lParam;
	tmp.Replace('\t',',');
	str.Format("MarketEvent\t%s",tmp);

	//AfxMessageBox((char*)lParam);
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)str);
	*/

	Variant(triggerCC, (char *)lParam);
	return 0;
}


void CMainWnd::SearchByUp(LPCTSTR sUpCode, LPCTSTR sGubn) 
{
	if (m_bSending == true)
		return;

	CString strUp, strtmp ;
	strUp.Format("%s", sUpCode);
	strUp.Trim();

	strtmp.Format("%s", sGubn);
	m_nStandard = atoi(strtmp);


	if (!CheckUpcode(strUp))    //PIDOJID2�� ��ȸ���� ���� �����ڵ�� ��û�� ����
	{
	//	strtmp.Format("�����ڵ� Ȯ�� [%s]", strUp);
	//	AfxMessageBox(strtmp);
		m_nMarketType = 0;
		redo();
		return;
	}
	
	m_nSearchType = ctSTOCK;
	m_strUp = strUp;

	clear();

	m_bSending = true;
	if (m_nMarketType == 0)
		sendTR("PIDOJID2","00", US_KEY, m_key, m_szName, KEY_JID2);
	else if (m_nMarketType == 1)
		sendTR("PIDOJID2","10", US_KEY, m_key, m_szName, KEY_JID2);
}

bool CMainWnd::CheckUpcode(CString strup)
{
	bool	bfind = false;
	std::shared_ptr<CUpGroup> item = nullptr;

	for (int ii = 0;ii < m_arrChild.GetSize();ii++)
	{
		item = m_arrChild.GetAt(ii);
		if (item->GetUPCode() == strup)
			bfind = true;
	}	

	return bfind;
}

void CMainWnd::redo() 
{
	clear();

	m_nSearchType = ctUP;
	if (m_nMarketType == 0)
		sendTR("PIDOJID2","00", US_KEY, m_key, m_szName, KEY_JID2);
	else if (m_nMarketType == 1)
		sendTR("PIDOJID2","10", US_KEY, m_key, m_szName, KEY_JID2);
}

int CMainWnd::openView(int type, CString param)
{
	return m_pWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)param.operator LPCTSTR());
}

CString CMainWnd::Parser(CString &strSrc, CString strSub)
{
	CString strTemp;
	
	if (strSrc.Find(strSub) == -1)
	{
		strTemp = strSrc;
		strSrc.Empty();
		return strTemp;
	}

	strTemp = strSrc.Left(strSrc.Find(strSub));
	strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
	return strTemp;
}

BSTR CMainWnd::GetDataArr(LPCTSTR sArr, LPCTSTR sGubn) 
{
	CString strResult, strUp, strtmp, strgubn;
	CStringArray arr;

	strResult.Format("%s", sArr);
	strUp = Parser(strResult, ":");
	strUp.Trim();
	strgubn.Format("%s", sGubn);
	strgubn.Trim();

	while (!strResult.IsEmpty()) 
	{
		strtmp = Parser(strResult, ":");
		strtmp.Trim();
		strtmp.Remove('A');
		arr.Add(strtmp);
	}
	strResult.Empty();
	
	std::shared_ptr<CUpGroup> upGroup = nullptr;

	if (strUp == "00" || strUp == "10")							//00 �ڽ��� 10 �ڽ���    ��ü ���������� ���� or �ܱ��� , ����� ������ 
	{
		for (int ii = 0 ; ii < m_arrChild.GetSize() ; ii++)
		{
			upGroup = m_arrChild.GetAt(ii);
			for (int jj = 0 ; jj < arr.GetSize(); jj++)
			{
				if (arr.GetAt(jj) == upGroup->GetUPCode())
				{
					if (strgubn == "0")					//�ð��Ѿ�  (�ʿ��� ��ȸ �ؿ��� ������ ȣ���� �ȵ�)
						strResult +=  arr.GetAt(jj) + "!" + upGroup->GetSiga() + ":";
					else if (strgubn == "2")				//�ŷ����  (�ʿ��� ��ȸ �ؿ��� ������ ȣ���� �ȵ�)
						strResult +=  arr.GetAt(jj) + "!" + upGroup->GetMoney() + ":";
					
					else if (strgubn == "3")             //�ܱ���, �����  ���ż���
					{		
						/*	
						CString strfore, strgign;
						if (upGroup->GetSumAmtFore() > 0)
							strfore.Format("+%.0f", upGroup->GetSumAmtFore());
						else
							strfore.Format("%.0f", upGroup->GetSumAmtFore());

						if (upGroup->GetSumAmtGign() > 0)
							strgign.Format("+%.0f", upGroup->GetSumAmtGign());
						else
							strgign.Format("%.0f", upGroup->GetSumAmtGign());
						
					//	strtmp.Format("%.0f@%.0f", upGroup->GetSumAmtFore(), upGroup->GetSumAmtGign());
						strtmp.Format("%s@%s", strfore, strgign);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
						*/
						strResult = "";
					}
					
					else if (strgubn == "4")             //�ܱ���, �����  ���ż����
					{
						/*
						CString strfore, strgign;
						if (upGroup->GetSumMnFore() > 0)
							strfore.Format("+%.0lf", upGroup->GetSumMnFore());
						else
							strfore.Format("%.0lf", upGroup->GetSumMnFore());
						
						if (upGroup->GetSumMnGign() > 0)
							strgign.Format("+%.0lf", upGroup->GetSumMnGign());
						else
							strgign.Format("%.0lf", upGroup->GetSumMnGign());

				//		strtmp.Format("%.0f@%.0f", upGroup->GetSumMnFore(), upGroup->GetSumMnGign());
						strtmp.Format("%s@%s", strfore, strgign);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
						*/
						strResult = "";
					}

					else if (strgubn == "5")             //����(�ð��Ѿ� ����)
					{
						strtmp.Format("%.2lf", upGroup->GetSumSgTotal()/m_dTotalSiga * 100);  
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
					}
					else if (strgubn == "6")             //����(�ŷ��� ����)
					{
						strtmp.Format("%.2lf", upGroup->GetSumAmt()/m_dTotalAmt * 100);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
					}
					else if (strgubn == "7")             //����(�ŷ���� ����)
					{
						strtmp.Format("%.2lf", upGroup->GetSumMoney()/m_dTotalMny * 100);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
					}
				}
			}
		}
	}
	else if (m_mapChild.Lookup(strUp, upGroup))			//Ư�� ������ ���� ���� or �ܱ��� , ����� ������
	{
		std::shared_ptr<CUpGroup> upChildGroup = nullptr;

		for (int ii = 0 ; ii < upGroup->m_arrChild.GetSize() ; ii++)
		{
			upChildGroup = upGroup->m_arrChild.GetAt(ii);
			for (int jj = 0 ; jj < arr.GetSize(); jj++)
			{
				if (arr.GetAt(jj) == upChildGroup->GetCode())
				{
					if (strgubn == "0")					//�ð��Ѿ�   //�ð��Ѿ�  (�ʿ��� ��ȸ �ؿ��� ������ ȣ���� �ȵ�)
						strResult +=  arr.GetAt(jj) + "!" + upChildGroup->GetSiga() + ":";
					else if (strgubn == "2")				//�ŷ����  (�ʿ��� ��ȸ �ؿ��� ������ ȣ���� �ȵ�)
						strResult +=  arr.GetAt(jj) + "!" + upChildGroup->GetMoney() + ":";

					else if (strgubn == "3")             //�ܱ���, ����� ���ż���
					{
						/*
						CString strfore, strgign;
						if (upChildGroup->GetForg() > 0)
							strfore.Format("+%.0f", upChildGroup->GetForg());
						else
							strfore.Format("%.0f", upChildGroup->GetForg());
						
						if (upChildGroup->GetGign() > 0)
							strgign.Format("+%.0f", upChildGroup->GetGign());
						else
							strgign.Format("%.0f", upChildGroup->GetGign());

				//		strtmp.Format("%s@%s", upChildGroup->GetForg(), upChildGroup->GetGign());
						strtmp.Format("%s@%s", strfore, strgign);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
						*/
						strResult = "";
					}
					
					else if (strgubn == "4")             //�ܱ��� �����  ���ż����
					{
						/*
						CString strfore, strgign;
						if (upChildGroup->GetMnForg() > 0)
							strfore.Format("+%.0f", upChildGroup->GetMnForg());
						else
							strfore.Format("%.0f", upChildGroup->GetMnForg());
						
						if (upChildGroup->GetMnGign() > 0)
							strgign.Format("+%.0f", upChildGroup->GetMnGign());
						else
							strgign.Format("%.0f", upChildGroup->GetMnGign());

				//		strtmp.Format("%s@%s", upChildGroup->GetMnForg(), upChildGroup->GetMnGign());
						strtmp.Format("%s@%s", strfore, strgign);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
						*/
						strResult = "";
					}
					else if (strgubn == "5")             //����(�ð��Ѿ� ����)
					{
						strtmp.Format("%.02f", atof(upChildGroup->GetSiga())/ GetUpdata(strUp, "2") * 100);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
					}
					else if (strgubn == "6")             //����(�ŷ��� ����)
					{
						strtmp.Format("%.02f", atof(upChildGroup->GetAmount())/GetUpdata(strUp, "3") * 100);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
					}
					else if (strgubn == "7")             //����(�ŷ���� ����)
					{
						strtmp.Format("%.02f", atof(upChildGroup->GetMoney())/GetUpdata(strUp, "4") * 100);
						strResult +=  arr.GetAt(jj) + "!" + strtmp + ":";
					}
				}
			}
		}	
	}

	return strResult.AllocSysString();
}

//�ش� ������ ���� �ð��Ѿ� , �ŷ��� , �ŷ������ ���ջ���� ��ȯ
double CMainWnd::GetUpdata(CString strUp, CString strgubn)
{
	double	dReturn = 0;
	std::shared_ptr<CUpGroup> upGroup = nullptr;

	if (m_mapChild.Lookup(strUp, upGroup))
	{
		if (strgubn == "2")		//�ð��Ѿ� 
		{
			dReturn = upGroup->GetSumSgTotal();
		}
		else if (strgubn == "3")	//�ŷ��� 
		{
			dReturn = upGroup->GetSumAmt();
		}
		else if (strgubn == "4")  //�ŷ����
		{
			dReturn = upGroup->GetSumMoney();
		}
	}
	return dReturn;
}

CString CMainWnd::GetForedata(CString strUp)
{
	std::shared_ptr<CUpGroup> upGroup = nullptr;

	if (m_mapChild.Lookup(strUp, upGroup))
	{
		return upGroup->GetForg();
	}
	return "";
}

CString CMainWnd::GetGigndata(CString strUp)
{
	std::shared_ptr<CUpGroup> upGroup = nullptr;

	if (m_mapChild.Lookup(strUp, upGroup))
	{
		return upGroup->GetGign();
	}
	return "";
}

void CMainWnd::clear()
{
	std::shared_ptr<CUpGroup> item = nullptr;

	for (int ii = 0; ii < m_arrChild.GetSize(); ii++)
	{
		item = m_arrChild.GetAt(ii);
		item->Reset();
		item.reset();
	}
	m_arrChild.RemoveAll();
	m_mapChild.RemoveAll();
}