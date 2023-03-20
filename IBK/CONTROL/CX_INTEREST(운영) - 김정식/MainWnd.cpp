// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "MainWnd.h"
#include "toolwnd.h"
#include "groupwnd.h"
#include "basketwnd.h"
#include "TreeWnd.h"
#include "BaseWnd.h"
#include "optdlg.h"
#include "GroupWnd.h"
#include "UsefulSetupDlg.h"
#include "../../h/userdecrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TRKEY_INTER	100

struct MarketTime
{
	char row1[30];	//�����ð���
	char row2[30];	//����ȣ��
	char row3[30];	//�峻�ð�
	char row4[30];	//����ȣ��
	char row5[30];	//���Ľð���
	char row6[30];	//�ð��� ���ϰ�
};

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pWnd, _param* pInfo)
{
	EnableAutomation();

	m_pWnd = pWnd;	
	m_nResize = MO_NONE;
	m_clrKospi	  = RGB(0, 0, 0);
	m_clrKosdaq	  = RGB(128, 64, 64);
	m_nFileType = LARGETYPE_FILE;
	m_nID = -1;
	m_mapName = "2000";
	SetParam(pInfo);
	m_iTrkey = -1;
	
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
ON_WM_DESTROY()
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_SETCURSOR()
ON_WM_LBUTTONDBLCLK()
ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
ON_MESSAGE(WM_MANAGE, OnManage)
ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_PROPERTY_NOTIFY(CMainWnd, "cbGroupData", m_cbGroupData, OnCbGroupDataChanged, VT_BSTR)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Reload", Reload, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetUseToolBox", SetUseToolBox, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CMainWnd, "SetViewType", SetViewType, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CMainWnd, "ChangeGroup", ChangeGroup, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CMainWnd, "ChangeArrange", ChangeArrange, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CMainWnd, "ChangeView", ChangeView, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION(CMainWnd, "SetAutoCheck", SetAutoCheck, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CMainWnd, "GetCode", GetCode, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "OnPortfolio", OnPortfolio, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "RefreshCX", RefreshCX, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION(CMainWnd, "SetGroup", SetGroup, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CMainWnd, "Save", Save, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "CheckSaveStatus", CheckSaveStatus, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Recover", Recover, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "DeleteOrgFile", DeleteOrgFile, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "ClosingSave", ClosingSave, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CMainWnd, "IsWindow", IsWindow, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "ClearGrid", ClearGrid, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetExpect", SetExpect, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CMainWnd, "SetAccount", SetAccount, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SetOthersMap", SetOthersMap, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CMainWnd, "SetFieldData", SetFieldData, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SetPass", SetPass, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "CanClose", CanClose, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetMapHandle", SetMapHandle, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetMapHandle", GetMapHandle, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetRemainCode", GetRemainCode, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SearchGroupList", SearchGroupList, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SearchGroupCode", SearchGroupCode, VT_EMPTY, VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {AC991DBA-2B69-4AE5-8183-BAB58DCDB5E0}
static const IID IID_IMainWnd =
{ 0xac991dba, 0x2b69, 0x4ae5, { 0x81, 0x83, 0xba, 0xb5, 0x8d, 0xcd, 0xb5, 0xe0 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers


/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	Variant(titleCC, "��������");
	init();
	loadinfo();
	CreateChild();
	SetPallette();
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)"Message");

	return 0;
}

void CMainWnd::GuideMessage(CString msg)
{
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axGUIDE, 1), (LONG)(char*)msg.operator LPCTSTR());
}

void CMainWnd::SetParam(_param *pParam)
{
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
// 	m_clrForeColor = m_Param.tRGB = pParam->tRGB;
// 	m_clrBackColor = m_Param.pRGB = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());	
	m_param.options.Delete(0,2);

	CString tmpOptions;

	tmpOptions = m_param.options;

	CStringArray arOpt;

	SplitString(tmpOptions,":",arOpt);

	for(int i=0;i<arOpt.GetSize();i++)
	{
		switch (i)
		{
			case 0:
				{
					m_param.options = arOpt.GetAt(i);
				}
				break;
			case 1:
				{
					m_mapName = arOpt.GetAt(i);
				}
				break;
			case 2:
				{
					m_Smallfields = arOpt.GetAt(i);
				}
				break;
			case 3:
				{
					m_SmallDefWidth = arOpt.GetAt(i);
				}
				break;
			case 4:
				{
					m_Largefields = arOpt.GetAt(i);
				}
				break;
			case 5:
				{
					m_LargeDefWidth = arOpt.GetAt(i);
				}
				break;
		}
	}

	if(arOpt.GetSize() > 1)
	{
		m_bOthers = TRUE;
	}

	if(m_param.options == "SMALL")
	{
		m_nFileType = SMALLTYPE_FILE;
	}
	else if(m_param.options == "WIDE")
	{
		m_nFileType = LARGETYPE_FILE;
	}
}

void CMainWnd::SplitString(CString &strData, CString strToken,CStringArray &ar) //
{	
	CString sResult = strData;
	if(strToken == "") return;
	int iFind = strData.Find(strToken); 
	
	if(iFind < 0)
	{
		ar.Add(sResult);
		return;
	}
	
	while(iFind > -1)
	{
		if(iFind > -1 ) 
		{
			sResult = strData.Left(iFind);
			strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
			
			ar.Add(sResult);
			
			iFind = strData.Find(strToken);
		}
	}
	
	sResult = strData;
	
	ar.Add(sResult);
}

void CMainWnd::OnDestroy() 
{
	m_bDestroy = true;

	CWnd::OnDestroy();		

	saveinfo();
	DestroyChild();	

	::CloseHandle(m_hEventRTM);
}

void CMainWnd::loadinfo()
{
	CString	filepath;

	if(GetFileType() == SMALLTYPE_FILE)
		filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_SMALLCONFIG); 
	else
		filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_LARGECONFIG);
	
	char	buf[1024]{};
	CString	sztmp, szUnit;
	CString	section, key;
	memset(buf, 0x00, sizeof(buf));
	
	// height
	m_size.cx = m_size.cy = 0;
	m_size.cy = GetPrivateProfileInt(SEC_MAIN, KEY_HEIGHT, SIZE_HEIGHT, filepath);

	// Ʈ�� ������
	m_nTree = GetPrivateProfileInt(SEC_MAIN, KEY_TREESIZE, SIZE_TREE, filepath);
	//m_bTree = (BOOL)GetPrivateProfileInt(SEC_MAIN, KEY_TREEUSE, 0, filepath);
	m_bTree = 0;

	// ��ġ �ɼ�
	m_nOption = GetPrivateProfileInt(SEC_MAIN, KEY_SOPTION, OPT_DEFAULT, filepath);
	
	if (m_bTree)
	{
		m_size.cx += m_nTree;
		m_size.cx += SIZE_DIVIDE;
	}

	if (m_bRTS)
		m_size.cx += SIZE_RTS;

	// ��ٱ��� ������
	m_nBasket = GetPrivateProfileInt(SEC_MAIN, KEY_BASKETSIZE, SIZE_BASKET, filepath);
//	m_bBasket = (BOOL)GetPrivateProfileInt(SEC_MAIN, KEY_BASKETUSE, 0, filepath);
	m_bBasket = 0;

	if (m_bBasket)
	{
		m_size.cx += m_nBasket;
		m_size.cx += SIZE_DIVIDE;
	}
	
	// group
//	m_nGroup = GetPrivateProfileInt(SEC_MAIN, KEY_GROUPSIZE, SIZE_GROUP, filepath);
	m_nGroup = m_param.rect.Width();


	m_size.cx += m_nGroup;

	// Wave File
	m_bWaveApply = (BOOL)GetPrivateProfileInt(SEC_MAIN, "APPWAVE", 0, filepath);

	CString strWaveFile;
	strWaveFile.Format("%s\\%s\\%s", m_home, IMAGEDIR, "000.WAV");
	
	memset(buf, 0x00, sizeof(buf));
	if (0 == GetPrivateProfileString(SEC_MAIN, "WAVE00", "", buf, sizeof(buf), filepath))
	{
		m_strWaveFile = strWaveFile;
	}
	else
	{
		m_strWaveFile.Format("%s", buf);
	}

	m_bMKClrApply = (BOOL)GetPrivateProfileInt(SEC_MAIN, "APPMKCLR", 0, filepath);

	m_clrKospi = GetPrivateProfileInt(SEC_MAIN, "KOSPI", 0, filepath);

	m_clrKosdaq = GetPrivateProfileInt(SEC_MAIN, "KOSDAQ", 0, filepath);

	if (0 == GetPrivateProfileString(SEC_MAIN, "POPUP1", "", buf, sizeof(buf), filepath))
	{
		m_bPopupDlg = FALSE;
	}
	else
	{
		m_bPopupDlg = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, "DISPCODE", "", buf, sizeof(buf), filepath))
	{
		m_bDispCode = FALSE;
	}
	else
	{
		m_bDispCode = atoi(buf);
	}
}

void CMainWnd::saveinfo()
{
	CString	smallpath,largepath;
	CString	sztmp = _T("");

	smallpath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_SMALLCONFIG); 
	largepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_LARGECONFIG);

	WritePrivateProfileString(SEC_MAIN, KEY_VERSION, "1", smallpath);
	WritePrivateProfileString(SEC_MAIN, KEY_VERSION, "1", largepath);

	// Ʈ��������
// 	sztmp.Format("%d", m_nTree);
// 	WritePrivateProfileString(SEC_MAIN, KEY_TREESIZE, sztmp, filepath);
// 	WritePrivateProfileString(SEC_MAIN, KEY_TREEUSE, (m_bTree) ? "1" : "0", filepath);

	sztmp.Format("%d", m_nOption);
	WritePrivateProfileString(SEC_MAIN, KEY_SOPTION, sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, KEY_SOPTION, sztmp, largepath);
	
	// ��ٱ��� ������
// 	sztmp.Format("%d", m_nBasket);
// 	WritePrivateProfileString(SEC_MAIN, KEY_BASKETSIZE, sztmp, filepath);
// 	WritePrivateProfileString(SEC_MAIN, KEY_BASKETUSE, (m_bBasket) ? "1" : "0", filepath);

	// group
//	sztmp.Format("%d", m_size.cx - ((m_bTree) ? m_nTree+SIZE_DIVIDE : 0));//BUFFET ADD FOR SIZE 
	
// 	CString dd;
// 	dd.Format("cx : %d Tree %d RTs : %d basket : %d nGroup : %d", m_size.cx, m_nTree+SIZE_DIVIDE, SIZE_RTS, m_nBasket+SIZE_DIVIDE, m_nGroup);
// 	MessageBox(dd);
	//sztmp.Format("%d", m_nGroup);
	WritePrivateProfileString(SEC_MAIN, KEY_GROUPSIZE, sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, KEY_GROUPSIZE, sztmp, largepath);
	
	sztmp.Format("%d", m_size.cy);
	WritePrivateProfileString(SEC_MAIN, KEY_HEIGHT, sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, KEY_HEIGHT, sztmp, largepath);

	sztmp.Format("%d", m_bWaveApply);
	WritePrivateProfileString(SEC_MAIN, "APPWAVE", sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, "APPWAVE", sztmp, largepath);

	WritePrivateProfileString(SEC_MAIN, "WAVE00", m_strWaveFile.GetBuffer(0), smallpath);
	WritePrivateProfileString(SEC_MAIN, "WAVE00", m_strWaveFile.GetBuffer(0), largepath);

	sztmp.Format("%d", m_bMKClrApply);
	WritePrivateProfileString(SEC_MAIN, "APPMKCLR", sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, "APPMKCLR", sztmp, largepath);

	sztmp.Format("%d", m_clrKospi);
	WritePrivateProfileString(SEC_MAIN, "KOSPI", sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, "KOSPI", sztmp, largepath);

	sztmp.Format("%d", m_clrKosdaq);
	WritePrivateProfileString(SEC_MAIN, "KOSDAQ", sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, "KOSDAQ", sztmp, largepath);

	sztmp.Format("%d", m_bPopupDlg);
	WritePrivateProfileString(SEC_MAIN, "POPUP1", sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, "POPUP1", sztmp, largepath);

	sztmp.Format("%d", m_bDispCode);
	WritePrivateProfileString(SEC_MAIN, "DISPCODE", sztmp, smallpath);
	WritePrivateProfileString(SEC_MAIN, "DISPCODE", sztmp, largepath);
}

HBITMAP CMainWnd::getBitmap(CString path)
{
	const CBitmap*	pBitmap = (CBitmap*)m_pWnd->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

CFont* CMainWnd::GetAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)m_pWnd->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMainWnd::CreateChild()
{
	const COLORREF	color = getAxColor(64);
	// �� ���ϵ� �����찡 ������ �� �ʱ�ȭ������ ������� ���� ������ ����� �����Ƿ� ���ϵ� ���������� ���� ���� �� �ִ�. 
	// �� m_pGroupWnd ������ m_pBasketWnd�� ���� �����͸� �䱸�ϰų� �ϸ� ���� �� �����Ƿ� ���� ���� �Ͽ� �����Ͽ��� �Ѵ�.
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_home, IMAGEDIR);
	
	HBITMAP	hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};
	hBITMAP = getBitmap(fileIMG + "TROPT.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "TROPT_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "TROPT_en.bmp");
	m_pFont = GetAxFont();

	m_btOption.Create("�˻��ɼ�\n����������", CRect(0, 0, 0, SIZE_BUTTONS), this, IDC_BT_CONFIG, TRUE, TRUE);
	m_btOption.SetFont(m_pFont, false);  
	m_btOption.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btOption.SetCheck(false);
	m_nOptHeight = 0;
	
	if (m_pOptDlg == nullptr)
	{
		m_pOptDlg = std::make_unique<COptDlg>(this);
		m_pOptDlg->Create(COptDlg::IDD, this);
		CRect	rect;
		SearchOption(MO_SET, m_nOption);
		m_pOptDlg->GetWindowRect(rect);
		m_nOptHeight = rect.Height();
	}

	if (m_pTreeWnd == nullptr)
	{
		m_pTreeWnd = std::make_unique<CTreeWnd>(this);
		m_pTreeWnd->Create(WS_CHILD|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_TREE);			
	}	

	if (m_pToolWnd == nullptr)
	{
		m_pToolWnd = std::make_unique<CToolWnd>(this);
		m_pToolWnd->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_TOOL);
		m_pToolWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);
	}
	
	if (m_pGroupWnd == nullptr)
	{
		m_pGroupWnd = std::make_unique<CGroupWnd>(this);
		m_pGroupWnd->m_strWaveFile = m_strWaveFile;
		m_pGroupWnd->m_bWaveApply  = m_bWaveApply;
		m_pGroupWnd->m_bMKClrApply = m_bMKClrApply;
		m_pGroupWnd->m_clrKospi	  = m_clrKospi;
		m_pGroupWnd->m_clrKosdaq   = m_clrKosdaq;
		m_pGroupWnd->m_bPopupDlg	  = m_bPopupDlg;
		m_pGroupWnd->m_bDispCode	  = m_bDispCode;
		m_pGroupWnd->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GROUP);	
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);
	}
}

void CMainWnd::DestroyChild()
{
	//if (LIB_IsWndAlive(m_pToolWnd))
	//	LIB_DeleteWnd(m_pToolWnd);
	//if (LIB_IsWndAlive(m_pTreeWnd))
	//	LIB_DeleteWnd(m_pTreeWnd);
// 	if (LIB_IsWndAlive(m_pGroupWnd))
// 		LIB_DeleteWnd(m_pGroupWnd);
// 	if (LIB_IsWndAlive(m_pBasketWnd))
// 		LIB_DeleteWnd(m_pBasketWnd);

	m_btOption.DestroyWindow();
}

LONG CMainWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	CString slog;


	LONG	ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_ISTREE:
		ret = m_bTree;
		break;

	case MK_GROUPHEIGHT:
		{
			int	nHeight		= m_size.cy;
			if(m_bUseToolWnd)
			{
				if (!m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE))
					nHeight -= (SIZE_TOOL / 2 + 1);
				else
					nHeight -= SIZE_TOOL;
			}

			ret = nHeight;
		}
		break;
	case MK_PROCDLL:
		m_bProc = true;

		break;
	case MK_VARIANT:
		{
			m_szRET = _T("");
			m_szRET.Format("%s", Variant(HIWORD(wParam), (char*)lParam));
			ret = (LONG)(char*)m_szRET.operator LPCTSTR();
		}
		break;
	case MK_SHOWTOOLTIP:
		Variant(guideCC, (char*)lParam);
		break;

	case MK_TOOLSIZE:
		ResizeOper(m_size.cx, m_size.cy);
		break;

	case MK_SYNC:
		switch (HIWORD(wParam))
		{
		case MO_LOCK:
			m_sync.Lock();
			break;
		case MO_UNLOCK:
			m_sync.Unlock();
			break;
		}
		break;
	case MK_GETID:
		ret = (LONG)(char*)m_id.operator LPCTSTR();
		break;
	case MK_GETROOT:
		ret = (LONG)(char*)m_home.operator LPCTSTR();
		break;
	case MK_GETUSER:
		ret = (LONG)(char*)m_user.operator LPCTSTR();
		break;		
	case MK_GETBITMAP:
		ret = (LONG)m_pWnd->SendMessage(WM_USER, getBITMAP, lParam);
		break;
	case MK_TREEVISIBLE:
		ret = OperTreeVisible(HIWORD(wParam));
		break;
// 	case MK_BASKETVISIBLE:
// 		ret = OperBasketVisible(HIWORD(wParam));
// 		break;
	case MK_SIZE:
		OperSize(HIWORD(wParam), CSize(LOWORD(lParam), HIWORD(lParam)));
		break;
	case MK_OPENSCR:
		OpenScreen(HIWORD(wParam), (char*)lParam);
		break;
	case MK_SEARCHOPT:
		ret = SearchOption(HIWORD(wParam), (DWORD)lParam);
		break;
	case MK_FILETYPE:
		ret = m_nFileType;
		break;
	case MK_GETARRANGE:
		ret = m_pToolWnd->SendMessage(WM_MANAGE,MK_GETARRANGE);
		break;
	case MK_SETARRANGE:
		ret = m_pToolWnd->SendMessage(WM_MANAGE,MK_SETARRANGE);
		break;
	case MK_GETWND:
		switch (HIWORD(wParam))
		{
		case MO_VIEW: 
			ret = (LONG)m_pWnd;
			break;
		case MO_GROUP:
			ret = (LONG)m_pGroupWnd.get();
			break;
		case MO_TOOL:
			ret = (LONG)m_pToolWnd.get();
			break;
		case MO_TREE:
			ret = (LONG)m_pTreeWnd.get();
			break;
// 		case MO_BASKET:
// 			ret = (LONG)m_pBasketWnd;
// 			break;
		case MO_GRID:
			ret = m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GRID));
			break;
		}
		break;
	case MK_SELGROUP:
		m_slog.Format("[cx_interest][CMainWnd][OnManage][remove] MK_SELGROUP  [%d]\r\n", (int)lParam);
		OutputDebugString(m_slog);
		if (HIWORD(wParam) == MO_SET)
			SelectGroup((int)lParam);
		break;
	// by pass message
	case MK_SENDTR:
		SendOper((CSendData*)lParam);
		break;
	case MK_GROUP:
	case MK_GETGROUP:
	case MK_CHANGEFIELD:
	case MK_EXCEL:
	case MK_EXPECT:
	case MK_CHANGEDIFFFIELD:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_AUTOEXPECT:
		ret = (LONG)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT, lParam);
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_SETAUTOCHECK:
		ret = (LONG)m_pToolWnd->SendMessage(WM_MANAGE, MK_SETAUTOCHECK, lParam);
		break;
	case MK_TREEDATA:
		{
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);		//4
		}
		break;
	case MK_UPJONGDATA:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);		//4
		break;
	case MK_CLOSE:
		{
			OnDestroy();
		}
		break;
	case MK_MARKETCODE:
		{
			m_bDispCode = (BOOL)lParam;
		}
		break;
	case MK_ARRANGE:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);	
		break;
	case MK_SAVEFOROUB:
		{
			m_pGroupWnd->SendMessage(WM_MANAGE,wParam,lParam);
		}
		break;
	case MK_ACCOUNT:
		{
			m_strAccount = CString((char*)lParam);
		}
		break;
	case MK_BOUCODE:
		{
			m_strTotalCode = (LPCTSTR)lParam;
		}
		break;
	case MK_GetSearchType:
		{
			if(m_iTrkey == TRKEY_GROUPCODE)
				ret = 1;
			else 
				ret = 0;
			
			m_iTrkey = -1;
		}	
		break;
	case MK_PTRGROUPCODE:
		{
			//const int key = (int)lParam;
			//
			//m_pcodeArr = nullptr;
			//if(m_mapGroupCode.Lookup(key, m_pcodeArr))
			//	ret = (LONG)(char*)m_pcodeArr;
			//else
			//	ret =  0;
		}
		break;
	}

	return ret;
}

CTreeWnd* CMainWnd::GetTreeWnd()
{
	return m_pTreeWnd.get();
}

void CMainWnd::SetPallette()
{
	const COLORREF	color = getAxColor(64);
	if (m_pToolWnd)
		m_pToolWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);

	if (m_pGroupWnd)
		m_pGroupWnd->SetGridBkColor(color);

	m_strPal = GetPalette();
	
	Invalidate();
}

LONG CMainWnd::OperDLLOUB(WPARAM wParam, LPARAM lParam)
{
	struct _extTHx* ex = (struct _extTHx*)lParam;
	CString slog;
	const LONG ret = 0;
	const WORD	kind = HIBYTE(LOWORD(wParam));
	const struct _trkey* trkey = (struct _trkey*)&kind;
	
	CRecvData	rdata;
	CSendData	sdata;
	rdata.SetData(sdata.GetUserData(), wParam, lParam);
	
	char* data = ex->data; 
	const int len = ex->size;

	slog.Format("[cx_interest] OperDLLOUB key=[%d] len=[%d]", ex->key, len);
	OutputDebugString(slog);

	CString strRem = CString(data,len);
	if (ex->key == TRKEY_INTER)
	{
		const CString skey = strRem.Left(2);
		m_iSendTr--;   //test 20230223
		m_slog.Format("[cx_interest][CMainWnd][OperDLLOUB][%x] m_iSendTr=[%d] [%d] strRem=[%26s]", this, m_iSendTr, len,strRem);
		OutputDebugString(m_slog);
		if (atoi(skey) < 0)
			return ret;

		_groupKey = atoi(skey);
		if (_groupKey < 0)
			return ret;

		m_pGroupWnd->m_iInterCnt = atoi(CString(data, len).Mid(22, 4));
		m_pTreeWnd->receiveOub(CString(data, len), _groupKey);

		
	
		//if(m_bChangeGroup == 0)
		//	m_bChangeGroup = false;  //test 20230223

		return ret;
	}

	if (len	== sizeof(struct MarketTime))
	{
		SetMarketTime(data);
	}
	else if(m_strAccount != "" && strRem.Find(m_id) > -1)
	{
		_groupKey = 0;
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
	}
	else
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
	}
	return ret;
}


//�׽�Ʈ��
void CMainWnd::testSaveFile(CString dt)
{
	CString strPath(_T("")), m_root, m_user;
	strPath.Format("c:\\test.ini");
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	CString titletime;
	
	titletime.Format("[%s]MainWnd::OnUser", strCurTime);
	
	WritePrivateProfileString("IB202200", strCurTime, (LPCTSTR)dt, (LPCTSTR)strPath);
}

LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	CString s = "";
	LONG	ret = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		if(m_nSendCount > 0)
		{
			m_nSendCount--;
		}

		m_sync.Lock();
		ret = OperDLLOUB(wParam, lParam);
		this->EnableWindow(TRUE);
		m_sync.Unlock();
		SendMode(false);

		if(m_nID != -1 && m_nID < 101)
		{
			CString send;			
			if(m_nID == 100)
			{
				send.Format("%s", "change_extend");
				m_pWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)send);
			}
			m_nID = -1;
			return 0;
		}
		break;
		//2012.01.19 KSJ RTM ó������� ���� ���˺���
	case DLL_ALERTx:
		if (!m_bDestroy)		
			doRTMx(lParam);
		break;
		//KSJ
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		SetPallette();
		break;
	case DLL_TRIGGER:
		if (!m_bDestroy)
			parsingTrigger((char*)lParam);
		break;
	case DLL_DOMINO:
		parsingDomino((char*)lParam);	//����ȭ�鿡�� 2022���鼭 �׷����� ���� ��
		break;
	case DLL_GUIDE:
		return 1;
		break;
	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			m_param.point = HIWORD(wParam);

			if(m_pGroupWnd)
				m_pGroupWnd->setFontSize(m_param.point);

			Invalidate(FALSE);
		}
		break;
	}

	return ret;
}

void CMainWnd::parsingDomino(CString datB)
{
	CString symbol, entry;

	while(!datB.IsEmpty())
	{
		symbol = IH::Parser(datB, "\t");
		entry = IH::Parser(datB, "\n");

		if(symbol == _T("selectGroup") && !entry.IsEmpty())
		{
			m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELECTGROUP, (LPARAM)atoi(entry)));
		}
	}
}

void CMainWnd::parsingTrigger(CString datB)
{
	CString	symbol;
	symbol = IH::Parser(datB, "\t");

	if (symbol == _T("OnPortfolio") && datB == _T("ok"))
	{
		if (!m_bProc)
		{

			const int	ret = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
			m_pTreeWnd->SendMessage(WM_MANAGE, MK_SETUPOK);

			// ������ ���Ѿ��Ѵ�. m_pToolWnd�� m_pTreeWnd�� ���� setting���Ŀ� �װ��� �������� m_pGroupWnd�� ���Ѵ�.
			m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SETUPOK, ret));	
			m_pToolWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
		}
		m_bProc = false;
	}	
	else if (symbol == _T("OnCatchChange") && datB == _T("ok"))	// ���Ǻ���
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SISECATCH, MO_INIT));
	}
	else if (symbol == _T("Alarm"))					// �ش�����
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SISECATCH, MO_CATCH), (LPARAM)(char*)datB.operator LPCTSTR());
	}	
}


COLORREF CMainWnd::getAxColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_pWnd->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}


void CMainWnd::sendTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey)
{ 
	if(m_bMainClose)
		return;

	std::string trData; 	

	struct _userTH udat;
	memset((void*)&udat, 0, sizeof(udat));

	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;
	
	trData = uniqueKey;
	trData += keyName;
	trData += '\t';
	trData += std::string((char *)&udat, L_userTH);
	data.Replace("emptyrow", "        ");  //test 20230208
	trData += data.GetString();
	
	const LRESULT result = m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.size() - L_userTH - m_param.name.GetLength() - 1), (LPARAM)trData.c_str());
	if (result)
	{
		m_nSendCount++;
	}
}	

int CMainWnd::searchTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey)
{ 
	if(m_bMainClose)
		return -1;
	
	CString trData = ""; 
	CString tempdata = "";
	CString pass;
	
	struct _userTH udat;
	memset((void*)&udat, 0, sizeof(udat));
	
	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;
	
	trData = CString((char)uniqueKey);	
	trData += keyName;
	trData += '\t';
	trData += CString((char *)&udat, L_userTH);
	trData += data;
	
	//	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strlen(data)), (LPARAM)(LPCTSTR)trData);
	const int RESULT = m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.GetLength() - L_userTH - m_param.name.GetLength() - 1), (LPARAM)trData.operator LPCTSTR());
	
	m_nSendCount++;

	m_iTrkey = uniqueKey;
	return RESULT;
}	

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	const char* dta = (char*)m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
	DrawDivide(&dc);
}

void CMainWnd::init()
{
	m_home = Variant(homeCC);
	m_user = Variant(nameCC);
	m_id = Variant(userCC);
	m_pBrush = GetAxBrush(RGB(100, 100, 100));
}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 || cy == 0)
		return;
	
	ResizeOper(cx, cy);
}

CBrush* CMainWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pWnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}

void CMainWnd::ResizeOper(int cx, int cy)
{
	if (m_pGroupWnd == nullptr || m_pToolWnd == nullptr || m_pTreeWnd == nullptr || m_pOptDlg == nullptr)
		return;

	int	minx = ((m_bTree) ? m_nTree : 0);
	const int	miny = SIZE_TOOL + SIZE_GROUPMINY + ((m_bBasket) ? SIZE_BASKETMINY : 0);
	const int	ncnt = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ALL));
	
	minx += (ncnt * SIZE_GROUPMINX);

	if (cx < minx)
		cx = minx;
	if (cy < miny)
		cy = miny;

	m_size = CSize(cx, cy);
	const CRect	rect(0, GAP2, m_size.cx, m_size.cy);
	CRect	rcTree(GAP1, SIZE_DIVIDE*GAP2, 0, m_size.cy-2*SIZE_DIVIDE);//BUFFET
	CRect	rcBasket(0, SIZE_DIVIDE, m_size.cx, m_size.cy-2*SIZE_DIVIDE);//BUFFET

	const BOOL	bBig = m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);

	const CRect rcRTS(0, (bBig) ? SIZE_TOOL + SIZE_DIVIDE*3 : SIZE_TOOL/2 + SIZE_DIVIDE*4, 0, m_size.cy-2*SIZE_DIVIDE);

	CRect	rcTool((m_bTree) ? SIZE_DIVIDE+GAP1:0, 0, m_size.cx-GAP1, SIZE_TOOL/2), rcTemp(0, 0, 0, 0);

	m_rcDivide[0] = CRect(0, 0, 0, 0);
	m_rcDivide[1] = CRect(0, 0, 0, 0);

	m_rcPanel[0] = CRect(0, 0, 0, 0);
	m_rcPanel[1] = CRect(0, 0, 0, 0);
	m_rcPanel[2] = CRect(0, 0, 0, 0);

	if (m_bTree)
	{
		m_rcDivide[0].left = rcTree.right = m_nTree;
		m_rcDivide[0].right = m_rcDivide[0].left + SIZE_DIVIDE;
		m_rcDivide[0].top = 0;
		m_rcDivide[0].bottom = m_size.cy;
		
		//for rounding
		m_rcPanel[0].left = GAP1;
		m_rcPanel[0].right = rcTree.right + SIZE_DIVIDE;
		m_rcPanel[0].top = GAP2;
		m_rcPanel[0].bottom = m_size.cy - 1;
		//for rounding 

		//rcBasket.left = rcTool.left = m_rcDivide[0].right + GAP1;	
		rcBasket.left = m_rcDivide[0].right + GAP1;		
		rcTool.left = m_rcDivide[0].right + SIZE_DIVIDE + GAP1;	
				
		rcTemp = rect;
		rcTemp.left = rcTemp.left + SIZE_DIVIDE;//BUFFET
		rcTemp.right = rcTree.right - SIZE_DIVIDE; 
		rcTemp.bottom = m_size.cy - SIZE_DIVIDE;
					
		if (m_btOption.IsChecked())
		{
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - m_nOptHeight;
			m_pOptDlg->MoveWindow(rcTemp, TRUE);
			m_pOptDlg->ShowWindow(SW_SHOW);
			rcTemp.bottom = rcTemp.top;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
			
		}
		else
		{
			m_pOptDlg->ShowWindow(SW_HIDE);
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
		}

		m_btOption.MoveWindow(rcTemp, TRUE);
		rcTree.bottom = rcTemp.top;
		m_btOption.ShowWindow(SW_SHOW);
		m_pTreeWnd->MoveWindow(rcTree, TRUE);
		m_pTreeWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pTreeWnd->ShowWindow(SW_HIDE);		
		m_btOption.ShowWindow(SW_HIDE);
		m_pOptDlg->ShowWindow(SW_HIDE);
	}

	if(m_bUseToolWnd)
	{
		m_pToolWnd->MoveWindow(rcTool);
		m_pToolWnd->ShowWindow(m_bUseToolWnd);
	}

	//GRID BACKGROUND OUTBOX
	m_rcPanel[2].right = rcTool.right+GAP1;
	m_rcPanel[2].bottom = m_size.cy;
	
	//GRID OUTBOX
	
	m_rcPanel[1].left = rcTree.right;
	m_rcPanel[1].top = rcTool.bottom;
	m_rcPanel[1].right = rcTool.right;
	m_rcPanel[1].bottom = m_size.cy;

	if(m_bUseToolWnd)
 		rcTool.top = rcTool.bottom;// + SIZE_DIVIDE*2;//BUFFET ADD SIZE_DIVIDE

	rcTool.bottom = m_size.cy - SIZE_DIVIDE;//buffet

	m_pGroupWnd->MoveWindow(rcTool);
	m_nGroup = rcTool.Width();
	m_pGroupWnd->ShowWindow(SW_SHOW);

	Invalidate();//BUFFET 	
}

void CMainWnd::DrawDivide(CDC* pDC)
{
	pDC->FillSolidRect(&m_rcPanel[2], getAxColor(64));
	pDC->FillSolidRect(&m_rcPanel[0], getAxColor(192));
	pDC->FillSolidRect(&m_rcPanel[1], getAxColor(192));
}

LONG CMainWnd::OperTreeVisible(int opt)
{
	LONG	ret = 0;

	if (opt)	// set visible
	{
		switch (opt)
		{
		case IDC_BT_PREV:
			m_size.cx += (m_nTree + SIZE_DIVIDE);
			m_bTree = TRUE;
			break;
		case IDC_BT_NEXT:
			m_size.cx -= (m_nTree + SIZE_DIVIDE);			
			m_bTree = FALSE;
			break;
		}

		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));
		//SendMinMaxInfo();//BUFFET BLOCK
	}
	else			// get visible state
	{
		ret = m_bTree;
	}

	return ret;
}


void CMainWnd::OperSize(int opt, CSize size)
{
	CSize szTemp = m_size;
	
	switch (opt)
	{
	case MO_MINUS:
		szTemp = szTemp - size;
		szTemp.cx -= 2;
		break;
	case MO_PLUS:
		szTemp = szTemp + size;
		break;
	}
	
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(szTemp.cx, szTemp.cy));
	
	if (m_size.cx != szTemp.cx || m_size.cy != szTemp.cy)
		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));
	
	MoveWindow(0, 0, m_size.cx, m_size.cy);
}

void CMainWnd::OpenScreen(int type, CString data)
{
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.operator LPCTSTR());
}

LRESULT CMainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
		return 1;
	else if ( message == WM_DESTROY )
	{
 		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INTSAVE, 0));

		Sleep(200);			//2016.08.18 KSJ G������ ���Ϲ��� �ð��� ���� ����... �׷��� V�� ������ �ִ�.
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CMainWnd::SelectGroup(int kind)
{
	m_slog.Format("[cx_interest][CMainWnd][SelectGroup][remove]  kind=[%d]\r\n", kind);
	OutputDebugString(m_slog);
	if (m_pTreeWnd)
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);

	if (m_pToolWnd)
		m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);	
}

void CMainWnd::SendOper(class CSendData* sdata)
{
	SendMode(true);

	if(sdata->GetTRCODE().Find("PIBOSJG2") > -1 || sdata->GetTRCODE().Find("PIBOFJGO") > -1)  //20200423 �ܰ���ȸTr����
	{
		sendTR(sdata->GetTRCODE(), CString(sdata->GetData(), sdata->GetDataLen()), US_ENC|US_KEY, m_param.key, m_param.name, sdata->GetDataKey());
		//2013.01.29 KSJ ENd
	}
	else if(sdata->GetTRCODE().Find("bo221199") > -1)	//2012.11.28 KSJ ���ð� ��ȸ
	{
		sendTR(sdata->GetTRCODE(), CString(sdata->GetData(), sdata->GetDataLen()), US_PASS|US_KEY, m_param.key, m_param.name, sdata->GetDataKey());
	}
	else
	{
		sendTR(sdata->GetTRCODE(), CString(sdata->GetData(), sdata->GetDataLen()), US_OOP|US_KEY, m_param.key, m_param.name, sdata->GetDataKey());
	}
}

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (LOWORD(wParam) == IDC_BT_CONFIG)
	{
		ResizeOper(m_size.cx, m_size.cy);
	}
	return CWnd::OnCommand(wParam, lParam);
}

DWORD CMainWnd::SearchOption(WORD kind, DWORD option)
{
	DWORD	ret = 0;
	switch (kind)
	{
	case MO_SET:
		m_pOptDlg->SetOption((int)option);
		break;
	case MO_GET:
		ret = m_pOptDlg->GetOption();
		break;
	case MO_ADD:
		m_pOptDlg->AddOption((int)option);
		break;
	case MO_DEL:
		m_pOptDlg->DelOption((int)option);
		break;
	}
	return ret;
}

void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
// 	TRACE("CMainWnd::OnLButtonDown\n");
	if (m_rcDivide[0].PtInRect(point))
	{
		SetCapture();
		m_nResize = MO_LEFTRIGHT;	
		m_xPosMove = m_xPos = point;
		CRect	rect = m_rcDIVIDE = m_rcDivide[0];
		rect.left = SIZE_TREEMINX;
		rect.right = rect.left + 200;
		ClientToScreen(rect);
		ClipCursor(rect);
		DrawMove(m_rcDIVIDE);
	}
	else if (m_rcDivide[2].PtInRect(point))
	{
		SetCapture();
		m_nResize = MO_TOPBOTTOM;
		m_xPosMove = m_xPos = point;
		CRect	rect = m_rcDIVIDE = m_rcDivide[2];
		rect.bottom = m_size.cy - SIZE_BASKETMINY;
		rect.top = rect.bottom - 300;
		ClientToScreen(rect);
		ClipCursor(rect);
		DrawMove(m_rcDIVIDE);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CMainWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	ClipCursor(nullptr);
	
	if ((m_nResize == MO_LEFTRIGHT && point.x != m_xPos.x) || (m_nResize == MO_TOPBOTTOM && point.y != m_xPos.y))
	{
		switch (m_nResize)
		{
		case MO_LEFTRIGHT:
			m_nTree += (point.x - m_xPos.x);
			break;
		case MO_TOPBOTTOM:
			m_nBasket -= (point.y - m_xPos.y);
			break;
		}
		
		ResizeOper(m_size.cx, m_size.cy);
	}

	DrawMove(m_rcDIVIDE);
	m_nResize = MO_NONE;
	m_rcDIVIDE.SetRectEmpty();
	m_xPosMove = m_xPos = CPoint(-1, -1);
	//InvalidateDivide();
	Invalidate();//BUFFET 
	CWnd::OnLButtonUp(nFlags, point);
}

void CMainWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_nResize != MO_NONE)
	{
		DrawMove(m_rcDIVIDE);

		switch (m_nResize)
		{
		case MO_LEFTRIGHT:
			m_rcDIVIDE.OffsetRect(point.x - m_xPosMove.x, 0);
			m_xPosMove.x = point.x;
			break;
		case MO_TOPBOTTOM:
			m_rcDIVIDE.OffsetRect(0, point.y - m_xPosMove.y);
			m_xPosMove.y = point.y;			
			break;
		}
		
		DrawMove(m_rcDIVIDE);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CMainWnd::DrawMove(CRect rect)
{
	CWindowDC	dc(this);
	const int	nMode = dc.SetROP2(R2_XORPEN);
	CBrush	*oldbrush = dc.SelectObject(m_pBrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	dc.SetROP2(nMode);
}

BOOL CMainWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (m_rcDivide[0].PtInRect(pt))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LR));
		return TRUE;
	}
	else if (m_rcDivide[2].PtInRect(pt))
	{
//		MessageBox("IDC_CURSOR_TB");
//		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_TB));
		return TRUE;	
	}
		
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainWnd::InvalidateDivide()
{
	CRect	rect = m_rcDivide[0] + CRect(0, 0, 1, 0);
	InvalidateRect(rect, FALSE);
	InvalidateRect(m_rcDivide[2], FALSE);
}

void CMainWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_rcDivide[0].PtInRect(point))
	{
		m_nTree = SIZE_TREE;
		ResizeOper(m_size.cx, m_size.cy);
	}
	else if (m_rcDivide[2].PtInRect(point))
	{
		m_nBasket = SIZE_BASKET;
		ResizeOper(m_size.cx, m_size.cy);
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}



void CMainWnd::SendMode(bool bSend /* = true */)
{
	if (bSend)
	{
		Enable(MO_ALLWND, !bSend);
		KillTimer(1000);
		SetTimer(1000, 10 * 1000, nullptr);
	}
	else
	{
		Enable(MO_ALLWND, !bSend);
	}
}

void CMainWnd::Enable(int kind, bool bEnable /* = true */)
{
	if (kind & MO_MAIN)
		EnableWindow(bEnable);
	if (kind & MO_TOOL)
		if (m_pToolWnd)
			m_pToolWnd->EnableWindow(bEnable);
	if (kind & MO_GROUP)
		if (m_pGroupWnd)
			m_pGroupWnd->EnableWindow(bEnable);
// 	if (kind & MO_BASKET)
// 		if (m_pBasketWnd)
// 			m_pBasketWnd->EnableWindow(bEnable);
	if (kind & MO_TREE)
	{
		if (m_pTreeWnd)
			m_pTreeWnd->EnableWindow(bEnable);
			
		if (m_pOptDlg)
			m_pOptDlg->EnableWindow(bEnable);
		
		if (m_btOption.m_hWnd)
			m_btOption.EnableWindow(bEnable);
		
	}
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1000)
	{
		KillTimer(1000);
		SendMode(false);		
	}
	else if(nIDEvent == 1001)
	{
		KillTimer(1001);
		CWnd::OnDestroy();
		
		saveinfo();	
		DestroyChild();	
		
		::CloseHandle(m_hEventRTM);
	}
	else if(nIDEvent == 1222)
	{
		KillTimer(1222);
		m_bChangeGroup = FALSE;   //test 20230223
	}

	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//���ð� ��ȸ 
void CMainWnd::RequestMarketTime()
{
	CSendData	sdata;
	char	key{};
	_trkey* trkey = (struct _trkey*)&key;	
	trkey->kind = TRKEY_MARKETTIME;
	
	sdata.SetData("bo221199", key, "1", 1, "");	
	
	SendOper(&sdata);
}

void CMainWnd::uploadBackup()
{
	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};
	//	char	strUinfo[500];
	
	struct _uinfo* uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;
	
	FillMemory((char *) uinfo, sz_uinfo, ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'b';
	
	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));
	
	uinfo->retc[0] = 'O';
	
	sendB[sendL] = 0x00; 
	
	//	SendTR
	CSendData	sdata;
	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
	
	trkey->kind = TRKEY_BACKUP;
	
	sdata.SetData(trUPDOWN, key, sendB, strlen(sendB), "");
	
	SendOper(&sdata);
}

void CMainWnd::SetMarketTime(char* datB)	//2012.11.26 KSJ ������ ��ȸ
{
	const struct MarketTime *mkTime = (struct MarketTime *)datB;
	
	CString strRow1, strRow2, strRow3, strRow4, strRow5, strRow6;
	CString strTemp;
	
	strRow1 = CString(mkTime->row1, 30);	//�����ð���
	strRow2 = CString(mkTime->row2, 30);	//����ȣ��
	strRow3 = CString(mkTime->row3, 30);	//�峻�ð�
	strRow4 = CString(mkTime->row4, 30);	//����ȣ��
	strRow5 = CString(mkTime->row5, 30);	//���Ľð���
	strRow6 = CString(mkTime->row6, 30);	//�ð��� ���ϰ�
	
	m_strMarketTime = strRow2;
}
//////////////////////////////////////////////////////////////////////////

// 2012.02.10 KSJ
void CMainWnd::doRTMx(LPARAM lParam)
{
	m_pGroupWnd->RecvRTSx(lParam);
}

CString CMainWnd::GetDataTitle(int nKind)
{
	CString strTitle = (char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, nKind);
	return strTitle;
}


BOOL CMainWnd::IsValidCode(CString strCode)
{
	if (m_pGroupWnd)
		return m_pGroupWnd->IsValidCode(strCode);

	return FALSE;
}

void CMainWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMainWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;

	memDC.CreateCompatibleDC(pDC);

	if (memDC.m_hDC != nullptr) 
		memDC.SelectObject(BitMap);

	if (memDC.m_hDC)
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CMainWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_home, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}

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

void CMainWnd::Reload() 
{
	//m_pToolWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
	//m_pTreeWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
	m_pGroupWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
	m_pToolWnd->ReloadList();

}

void CMainWnd::SetUseToolBox(BOOL bUseTool) 
{
	m_bUseToolWnd = bUseTool;
}

//TRUE:ALL FALSE:100
void CMainWnd::SetViewType(BOOL bType) 
{
	m_bAllType = bType;
}


void CMainWnd::ChangeGroup(short nIndex) 
{
	if(m_bChangeGroup == TRUE)
		return;
	//2011.12.20 KSJ ��Ʈ���� �������� �ʰ� GridWnd�� m_bSorting�� false�� ����� �ش�.
	if(m_pGroupWnd)
		m_pGroupWnd->SetInitSortingValue();
	// KSJ

	m_bChangeGroup = TRUE;
	SetTimer(1222,1000,nullptr);
	//SetTimer(1222, 500, nullptr);
	m_pGroupWnd->SendMessage(WM_MANAGE, MK_NOSELECT);
	
	if(nIndex == 0)
	{
		m_pGroupWnd->SendMessage(WM_MANAGE,MAKEWPARAM(MK_REMAIN,(LPARAM)nIndex));
		m_pTreeWnd->SendMessage(WM_MANAGE, MK_REMAIN);	

		m_bRemain = TRUE;
	}
	else
	{
		m_bRemain = FALSE;	//2014.09.16 KSJ �����ܰ��� �ٸ��׷����� �̵��ÿ� ��ĭ�� �������� ���� �߻���.
		m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELECTGROUP, (LPARAM)nIndex));
	}

	//OnCommand(MAKEWPARAM(IDC_CB_GROUP, CBN_SELCHANGE), 0);
}

void CMainWnd::ChangeArrange(short nIndex) 
{
	m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CHANGEARRANGE, (LPARAM)nIndex));	
}

void CMainWnd::ChangeView(short nIndex, short ncbIndex) 
{
	CString filepath;

	if(m_param.options != "SMALL")
	{
		if(nIndex == 0)
			m_nFileType = SMALLTYPE_FILE;
		else
			m_nFileType = LARGETYPE_FILE;
	}
	// 2012.04.05 KSJ ���ڰ� �ϳ� �� �޴� ������ ������.
	CString strTemp;
	strTemp.Format("%d%c%d", ncbIndex, (char)0x09,m_bRemain);

	m_pGroupWnd->SendMessage(WM_MANAGE,MK_CHANGEVIEWTYPE,(LPARAM)m_nFileType);
	m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CHANGEVIEWTYPE, (LPARAM)nIndex),(LPARAM)(LPCTSTR)strTemp);	
}

void CMainWnd::SetAutoCheck(BOOL bAuto) 
{
	OnManage(MK_SETAUTOCHECK, bAuto);
}

BSTR CMainWnd::GetCode() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult = m_pGroupWnd->m_strCode;

	return strResult.AllocSysString();
}

void CMainWnd::OnPortfolio(LPCTSTR result) 
{
	CString str(result);

	m_slog.Format("--------[cx_interest][CMainWnd][OnPortfolio][remove] str=[%s] m_bChangeGroup=[%d]\r\n", str, m_bChangeGroup);
	OutputDebugString(m_slog);

	if (m_bChangeGroup == TRUE)
		return;

	if( str == "start")
		return;

	//m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SETUPOK, 0));

	const int	ret = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
	m_pTreeWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
	
	// ������ ���Ѿ��Ѵ�. m_pToolWnd�� m_pTreeWnd�� ���� setting���Ŀ� �װ��� �������� m_pGroupWnd�� ���Ѵ�.
	m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SETUPOK, ret));
}

int CMainWnd::GetFileType()
{
	return m_nFileType;
}

void CMainWnd::RefreshCX(short key, short nIndex) 
{
	if(m_param.key != key)
	{
		if(m_nFileType == nIndex)
		{
			m_pGroupWnd->SendMessage(WM_MANAGE,MK_VIEWREFRESH,(LPARAM)m_nFileType);
		}
	}
}

void CMainWnd::SetGroup(short nIndex) 
{
	m_pToolWnd->SendMessage(WM_MANAGE,MK_SETGROUP,(LPARAM)nIndex);
}

void CMainWnd::Save() 
{
	m_pGroupWnd->SendMessage(WM_MANAGE,MK_SAVE,101);
}     

BOOL CMainWnd::CheckSaveStatus() 
{
	bool isNewDrop = (bool)m_pGroupWnd->SendMessage(WM_MANAGE,MK_ISNEWDROP);

	if(m_bRemain)
		isNewDrop = FALSE;

	return isNewDrop;
}

void CMainWnd::Recover() 
{
	m_pGroupWnd->SendMessage(WM_MANAGE,MK_RECOVER);

}

void CMainWnd::DeleteOrgFile() 
{
	m_pGroupWnd->SendMessage(WM_MANAGE,MK_DELETEORGFILE);
}

void CMainWnd::ClosingSave(short nID) 
{
	m_nID = nID;

	m_pGroupWnd->SendMessage(WM_MANAGE,MK_SAVE,nID);
}

BOOL CMainWnd::IsWindow() 
{
	return ::IsWindow(m_pGroupWnd->GetSafeHwnd());
}


void CMainWnd::ClearGrid() 
{
//	if (!m_bChangeGroup)
//		m_pGroupWnd->SendMessage(WM_MANAGE,MK_CLEAR);
}

void CMainWnd::SetExpect(BOOL bExpect) 
{
	m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_EXPECT, (bExpect ? MO_ON : MO_OFF)));
	m_pToolWnd->SendMessage(WM_MANAGE, MK_SETEXPECT, bExpect);
}

void CMainWnd::SetAccount(LPCTSTR strAccount) 
{
	m_strAccount = strAccount;

	m_pTreeWnd->m_strAccount = strAccount;
}

void CMainWnd::SetOthersMap(BOOL bFlag) 
{
	m_bOthers = bFlag;
}

void CMainWnd::SetFieldData(LPCTSTR mapname, LPCTSTR smallFields, LPCTSTR smallWidth, LPCTSTR largeFields, LPCTSTR largeWidth) 
{
	m_mapName = mapname;
	m_Smallfields = smallFields;
	m_SmallDefWidth = smallWidth;
	m_Largefields = largeFields;
	m_LargeDefWidth = largeWidth;
}

void CMainWnd::SetPass(LPCTSTR sPass) 
{
	m_Pass = sPass;
}

void CMainWnd::OnClose() 
{	
	m_bMainClose = TRUE;
	CWnd::OnClose();
}

BOOL CMainWnd::CanClose() 
{
	if(m_nSendCount != 0)
	{
		return FALSE;
	}
	
	return TRUE;
}

void CMainWnd::SetMapHandle(LPCTSTR sHandle) 
{
	RequestMarketTime();
	uploadBackup();

	m_sMapHandle = sHandle;
}

BSTR CMainWnd::GetMapHandle() 
{
	CString strResult;
	strResult = m_sMapHandle;

	return strResult.AllocSysString();
}

BSTR CMainWnd::GetRemainCode() 
{
	CString strResult;
	strResult = m_strTotalCode;

	return strResult.AllocSysString();
}


void CMainWnd::SearchGroupList() 
{
	// TODO: Add your dispatch handler code here
	CString strSendData;
	struct _updn updn;
	
	FillMemory(&updn, sizeof(_updn), ' ');
	ZeroMemory(&updn, sizeof(_updn));
	
	CopyMemory(&updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'G';
	CopyMemory(updn.uinfo.nblc, _T("00001"), sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'O';
	
	strSendData = CString((char*)&updn, sizeof(_updn));
	
	CSendData	sdata;
	char	key{};
	key = 0;
	_trkey* trkey = (struct _trkey*)&key;
	
	sdata.SetData(trUPDOWN, key, (LPSTR)(LPCTSTR)strSendData, strSendData.GetLength(), "");
	
	m_iTrkey = TRKEY_GROUPARR;
	const int ret = searchTR("pidomyst", strSendData, US_KEY, m_param.key, m_param.name, 18);
	if(ret == 0)
		m_iTrkey = -1;
}

//2020 ���ɱ׷� ����Ʈ ��ȸ 
void CMainWnd::SearchGroupList(bool bInit)  //���� ���� ��ȸ, ����â���� �׷�� ������ ��ȸ �ΰ����� ����
{
	CString strSendData;
	struct _updn updn;
	
	FillMemory(&updn, sizeof(_updn), ' ');
	ZeroMemory(&updn, sizeof(_updn));

	CopyMemory(&updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'G';
	CopyMemory(updn.uinfo.nblc, _T("00001"), sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'O';
	
	strSendData = CString((char*)&updn, sizeof(_updn));
	
	CSendData	sdata;
	char	key{};
	key = 0;
	_trkey* trkey = (struct _trkey*)&key;

	sdata.SetData(trUPDOWN, key, (LPSTR)(LPCTSTR)strSendData, strSendData.GetLength(), "");
	searchTR("pidomyst", strSendData, US_KEY, m_param.key, m_param.name, 18);
}

void CMainWnd::RemoveGroupCodeMap(int key)
{	
	//if(key == -1)
	//{
	//	char* pdata = nullptr;
	//	for (POSITION pos=m_mapGroupCode.GetStartPosition(); pos != nullptr;)
	//	{
	//		m_mapGroupCode.GetNextAssoc(pos, key, pdata);
	//		delete pdata;
	//		pdata = nullptr;
	//	}
	//	m_mapGroupCode.RemoveAll();
	//}
	//else
	//{
	//	char* pdata = nullptr;
	//	if(m_mapGroupCode.Lookup(key, pdata))
	//	{
	//		delete pdata;
	//		pdata = nullptr;
	//		m_mapGroupCode.RemoveKey(key);
	//	}
	//}
}

void CMainWnd::OnCbGroupDataChanged() 
{
	// TODO: Add notification handler code

}


void CMainWnd::SearchGroupCode(short index) 
{
	// TODO: Add your dispatch handler code here
	CString sdata;
	sdata.Format("%d", index);

	if(m_bChangeGroup == TRUE)
		return;

	if(m_pGroupWnd)
		m_pGroupWnd->SetInitSortingValue();

	m_bChangeGroup = TRUE;

	 if (m_pGroupWnd)
		 m_pGroupWnd->SendMessage(WM_MANAGE,MK_NOSELECT);

	 if(index == 0)
	 {
		 m_pGroupWnd->SendMessage(WM_MANAGE,MAKEWPARAM(MK_REMAIN,(LPARAM)index));
		 m_pTreeWnd->SendMessage(WM_MANAGE,MK_REMAIN);	
		 
		 m_bRemain = TRUE;
	 }
	 else
	 {
		 m_bRemain = FALSE;	//2014.09.16 KSJ �����ܰ��� �ٸ��׷����� �̵��ÿ� ��ĭ�� �������� ���� �߻���.
		 
		 const int index = atoi(sdata);
		 int	sendL = 0;
		 CString stmp;
		 char	sendB[16 * 1024] = { 0, };
		 const int isz_updn = sz_uinfo + 2;
		 struct _updn* updn = (struct _updn *)&sendB[sendL];
		 sendL += isz_updn;
		 
		 FillMemory((char *) updn, isz_updn, ' ');
		 
		 CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
		 updn->uinfo.dirt[0] = 'U';
		 updn->uinfo.cont[0] = 'j';
		 CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
		 updn->uinfo.retc[0] = 'O';

		 stmp.Format("%02d", index);
		 memcpy((char*)&sendB[sz_uinfo], stmp, 2);

		 const int ret = searchTR("pidomyst", CString(sendB, sendL) , US_KEY, m_param.key, m_param.name, TRKEY_GROUPCODE);
		 if(ret == 0)
			m_iTrkey = -1;

	 }
}


void CMainWnd::Request_GroupCode(int iseq)
{
	const int index = iseq;
	int sendL = 0;
	CString stmp;
	std::string sendB;
	sendB.resize(16 * 1024);

	const int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	stmp.Format("%02d", index);
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);

	//m_bChangeGroup = TRUE;  //test 20230223
	m_iSendTr++;
	m_slog.Format("[cx_interest][CMainWnd][OperDLLOUB] Request_GroupCode [%x] m_iSendTr=[%d]", this, m_iSendTr);
	OutputDebugString(m_slog);
	sendTR(trUPDOWN, sendB.data(), US_KEY, m_param.key, m_param.name, TRKEY_INTER);
}