// ToolWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "ToolWnd.h"
#include "../../control/fx_misc/misctype.h"
#include "IHCheck.h"
#include "../../h/interst.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SIZE_TABITEMWIDTH	80
#define SIZE_TABITEMHEIGHT	19
/////////////////////////////////////////////////////////////////////////////
// CToolWnd

CToolWnd::CToolWnd(CWnd* pMainWnd) : CBaseWnd(pMainWnd)
{
	// ADD PSH 20070918
	m_bMoveCfg  = FALSE;
	m_nMoveSave = 0;
	// END ADD
}

CToolWnd::~CToolWnd()
{
}


BEGIN_MESSAGE_MAP(CToolWnd, CBaseWnd)
	//{{AFX_MSG_MAP(CToolWnd)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolWnd message handlers


void CToolWnd::OperInit()
{
	loadcfg();
}

void CToolWnd::OperDestory()
{
	savecfg();
}

void CToolWnd::OperResize(int cx, int cy)
{
	m_rect = CRect(0, 0, cx, cy);
	if (cx <= 0 || cy <= 0)
		return;
}

void CToolWnd::SetViewType()
{
	int	ncnt = -1;

	if(GetMainViewType() == 1)
		ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GROUP, 0), (LPARAM)FALSE);
	else
 		ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GROUP, 1), (LPARAM)FALSE);
}

void CToolWnd::OperDraw(CDC* pDC)
{
	CBaseWnd::OperDraw(pDC);

}

void CToolWnd::OperPallette()
{

}

LONG CToolWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	bool bchk = FALSE;

	switch (LOWORD(wParam))
	{
	case MK_EXPECT:
			ret = m_bExpectCheck;
		break;
	case MK_SETEXPECT:
		if ((int)lParam == 0)
			m_bExpectCheck = FALSE;
		else
			m_bExpectCheck = TRUE;
		break;
	case MK_AUTOEXPECT:
		bchk = m_bAutoCheck;
		
		if (bchk)
			ret = 1;
		break;
	case MK_SETAUTOCHECK:
		m_bAutoCheck = (BOOL)lParam;
		if (m_bAutoCheck == TRUE)
			m_bExpectCheck = FALSE;
		ret = 1;
	case MK_GETAFIELD:	// get active field
		{

		}
		break;
	case MK_GETMOVECFG:
		ret = m_bMoveCfg;
		break;
	case MK_SETMOVECFG:
		m_bMoveCfg = lParam;
		break;
	case MK_GETMOVESAVE:
		ret = m_nMoveSave;
		break;
	case MK_SETMOVESAVE:
		m_nMoveSave = lParam;
		break;
	case MK_SETARRANGE:
		{
		const int nIndex = 0;
			m_pMainWnd->SendMessage(WM_MANAGE, MK_ARRANGE, nIndex);
		}
		break;

	case MK_SETVIEWTYPE:
		{
			SetViewType();
		}
		break;
	case MK_CHANGEVIEWTYPE:
		{
			loadcfg();
			const int nView = HIWORD(wParam);
			CString strTemp, strRemain, strIndex;
			strTemp.Format("%s", (char*)lParam);

			AfxExtractSubString(strIndex, strTemp, 0, (char)0x09);
			AfxExtractSubString(strRemain, strTemp, 1, (char)0x09);

			const int nIndex = atoi(strIndex); //- 1 ;	//받은 것에서 -1 해야 본래 인덱스나옴
			const BOOL bRemain = (BOOL)atoi(strRemain);

			((CMainWnd*)m_pMainWnd)->m_iViewType = nView;

			SetViewType();

			if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_SMALLCONFIG); 
			else
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_LARGECONFIG); 
			
			if(bRemain != TRUE)
				SendTree(nIndex);
		}
		break;
	case MK_SETGROUP:
		{

		}
		break;
	case MK_VIEWREFRESH:
		{	
			if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_SMALLCONFIG); 
			else
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_LARGECONFIG); 
		}
		break;
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}

	return ret;
}

void CToolWnd::savecfg()
{
	CString	sztmp;

	sztmp.Format("%d", m_bAutoCheck);
	WritePrivateProfileString(SEC_TOOL, KEY_AUTO, sztmp, m_fileCFG);

	int	nmode = MO_SELECT;

	if(GetMainViewType() == 1)
		nmode = MO_SELECT;
	else
		nmode = MO_VISIBLE;

	sztmp.Format("%d", nmode);
	WritePrivateProfileString(SEC_TOOL, KEY_FILLMODE, sztmp, m_fileCFG);

	int nIndex = 0;
	sztmp.Format("%d", (int)nIndex);
	WritePrivateProfileString(SEC_TOOL, KEY_CBARRANGE, sztmp, m_fileCFG);

	nIndex = GetMainViewType();
	sztmp.Format("%d", (int)nIndex);
	WritePrivateProfileString(SEC_TOOL, KEY_VIEWTYPE, sztmp, m_fileCFG);
}

void CToolWnd::loadcfg()
{
	BOOL	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EXPAND, 1, m_fileCFG);

	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_AUTO, 1, m_fileCFG);

	m_bMoveCfg  = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_MOVECFG, 0, m_fileCFG);
	m_nMoveSave = GetPrivateProfileInt(SEC_TOOL, KEY_MOVESAVE, 0, m_fileCFG);

	const int	nmode = (int)GetPrivateProfileInt(SEC_TOOL, KEY_FILLMODE, MO_SELECT, m_fileCFG);
	
	((CMainWnd*)m_pMainWnd)->m_iViewType = nmode;

	int	ncount = (int)GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);
	
	if (ncount <= 0)
		ncount = 1;
	
	if (ncount > MAX_GROUP)
		ncount = MAX_GROUP;

	const int	nval = GetPrivateProfileInt(SEC_TOOL, KEY_TIME, 3, m_fileCFG);

	if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
		m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_SMALLFIELD);
	else
		m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_LARGEFIELD);
 	
	CFileFind finder;
	if (!finder.FindFile(m_fileFIELD))
	{
		 CString	string;

		 if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		 {
			 if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
				 string = DEF_FIELD_CONTENT1;
			 else
				 string = DEF_FIELD_CONTENT;

			 WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
		 }
		 else
		 {
			 if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
				 string = ((CMainWnd*)m_pMainWnd)->m_Smallfields;
			 else
				 string = ((CMainWnd*)m_pMainWnd)->m_Largefields;
			 
			 WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
		 }
	}
	else
	{
		CString	string;
		
		if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		{
			int	readL = 0;
			char	readB[256];
			
			readL = GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", "", readB, sizeof(readB), m_fileFIELD);
			
			CString tmp = CString(readB, readL);
			
			if(tmp == "")
			{
				if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
					string = DEF_FIELD_CONTENT1;
				else
					string = DEF_FIELD_CONTENT;
				
				WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
			}
		}
		else
		{	
			int	readL = 0;
			char	readB[256];
			
			readL = GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", "", readB, sizeof(readB), m_fileFIELD);
			
			CString tmp = CString(readB, readL);

			if(tmp == "")
			{
				if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
					string = ((CMainWnd*)m_pMainWnd)->m_Smallfields;
				else
					string = ((CMainWnd*)m_pMainWnd)->m_Largefields;
				
				WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
			}
		 }
	}
 


	ncount = (int)GetPrivateProfileInt(SEC_TOOL, KEY_VIEWTYPE, 0, m_fileCFG);

	((CMainWnd*)m_pMainWnd)->m_iViewType = ncount;

}

void CToolWnd::SendTree(int nIndex)
{
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	pWnd->SendMessage(WM_MANAGE, MK_CALLINTEREST, nIndex);
}

int CToolWnd::GetMainViewType()
{
	return ((CMainWnd*)m_pMainWnd)->m_iViewType;
}

