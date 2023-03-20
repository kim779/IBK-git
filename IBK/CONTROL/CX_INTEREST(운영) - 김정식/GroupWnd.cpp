// GroupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "GroupWnd.h"
#include "MainWnd.h"
#include "multimon.h"
#include "../../h/grid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "GridWnd.h"
#include "UsefulSetupDlg.h"
#include <math.h>
/////////////////////////////////////////////////////////////////////////////
// CGroupWnd
#define	deliMITER	";"

CGroupWnd::CGroupWnd(CWnd* pMainWnd) : CBaseWnd(pMainWnd)
{	
	m_nGroup = 1;
	m_nIndex = -1;
	m_line = 1;
	m_rtmAction = rtmCELL;
	m_dblAction = dblNONE;
	m_selAction = 1;

	m_fontsize  = 9;
	m_fontname = _T("����ü");
	m_bCurr = TRUE;

	// ADD PSH 20070917
	m_bName = TRUE;

	m_clrKospi	  = RGB(0, 0, 0);
	m_clrKosdaq	  = RGB(128, 64, 64);
	m_clrbookMark = RGB(255,128,255);
	m_clrbookMarkPen = RGB(0, 0, 0);
	

	for(int ii=0 ; ii<2 ; ii++)
	{
		for(int jj=0 ; jj<3 ; jj++)
		{
			m_commInfo[ii][jj] = "";
		}
	}	
}

CGroupWnd::~CGroupWnd()
{
}


BEGIN_MESSAGE_MAP(CGroupWnd, CBaseWnd)
	//{{AFX_MSG_MAP(CGroupWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

void CGroupWnd::SetDragInCount(int data)
{
	m_iDragIn = data;
}

void CGroupWnd::AddDragInCount()
{
	m_iDragIn++;
}

void CGroupWnd::OperDraw(CDC* pDC)
{
	if(m_globalRect.IsRectEmpty())
	{
		GetClientRect(m_globalRect);
	}

	if (m_bMAP)
		DrawTitle(pDC);
}

void CGroupWnd::DrawTitle(CDC* pDC)
{
	pDC->FillSolidRect(m_rcTitle, RGB(255, 255, 0));
	const int	nmode = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_szTITLE, m_rcTitle, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	pDC->SetBkMode(nmode);
}

LONG CGroupWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch(LOWORD(wParam)) 
	{
	case MK_INTSAVE://BUFFET
		OnDestroySave();
		break;
	case MK_GROUPSAVE:
		saveWhenGroupClosed((int)lParam);
		break;
	case MK_GROUPSAVE2:				//ȭ�� ��� ��ȯ�� ���
		saveWhenChangeViewMode((int)lParam);
		break;
	case MK_SISECATCH:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, wParam, lParam);
			}
		}
		break;
	case MK_MEMO:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				if (ii == m_nCurSel)
					continue;
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_MEMO);
			}
		}
		break;
	case MK_DELETEMEMO:
		break;
	case MK_CLOSEGRID:
		CloseGrid(HIWORD(wParam));

		break;

	case MK_SETUPOK:	
		{
			loadfield();

			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->FieldSetup(false);
				m_GridWnd[ii]->Invalidate();
			}

			m_nCurSel = 0;
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
			m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);

			SelectOper();

m_slog.Format("[cx_interest][CGroupWnd][OnManage][remove]");
OutputDebugString(m_slog);

			ReSetupGrid();

			if(((CMainWnd*)m_pMainWnd)->m_bRemain)
			{
				CWnd* wnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
				wnd->SendMessage(WM_MANAGE,MK_REMAIN);
			}			
		}
		break;
	case MK_CHANGEFIELD:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->FieldSetup(false);
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
			}
		}
		break;
	case MK_CHANGE1FIELD:
		{
			if((INT)lParam == 1)
			{
				for ( int ii = 0 ; ii < m_nGroup ; ii++ )
				{
					m_GridWnd[ii]->ChangeField(1);
				}				
			}
			else
			{
				for ( int ii = 0 ; ii < m_nGroup ; ii++ )
				{
					m_GridWnd[ii]->ChangeField(0);
				}
			}
		}
		break;
	case MK_SELGROUP:
		switch (HIWORD(wParam))
		{
		case MO_SET:
			{
				CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
				m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);
				if (m_nCurSel != (int)lParam)
				{
					m_nCurSel = (int)lParam;
					SelectOper();
				}	
				
				const UINT kind = m_GridWnd[m_nCurSel]->GetKind();
				const int indx = CAST_TREEID(kind)->value;
				
 				saveGroupIndex(indx);
				savecfg();
			}
			break;
		case MO_GET:
			ret = (LONG)m_nCurSel;
			InvalidateDivide();
			break;
		}
		break;
	case MK_HAWKEYE:
		{
			for (int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_HAWKEYE);
			}
		}
		break;
	case MK_GETGROUP:
		switch (HIWORD(wParam))
		{
		case MO_ALL:
			ret = m_nGroup;
			break;
		case MO_ACTIVE:
			ret = m_nCurSel;
			break;
		case MO_LEFT:
			ret = m_nGroup - m_nCurSel;
			break;
		}
		
		break;
	case MK_GROUP:
		if (HIWORD(wParam))
			ret = GroupPlus((BOOL)lParam);
		else
			ret = GroupMinus((BOOL)lParam);

		InitGrid();
		break;
	case MK_ENDDRAG:
		{
			for ( int ii = 0 ; ii < m_cntGroup ; ii++ )
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_ENDDRAG);
			}			
		}
		break;
	case MK_GETMAX:
		{
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
			if (nOver == MO_VISIBLE)
			{
				ret = (m_nGroup - m_nCurSel) * MAX_LINE;
			}
			else if (nOver == MO_FULL)
				ret = (m_nGroup - m_nCurSel) * MAX_LINE;
			else
				ret = MAX_LINE;	
		}
		break;
	case MK_EXCEL:
		{
			m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_EXCEL);
		}
		break;
	case MK_EXPECT:
		ExpectOper(HIWORD(wParam));
		break;
	case MK_AUTOEXPECT:
		AutoExpectOper(HIWORD(wParam));
		break;
	case MK_CHANGEDIFFFIELD:
		ChangeFieldOper(HIWORD(wParam));
		break;
	case MK_CHANGERECOMMANDFIELD:
		{

		}
		break;
	case MK_TREEDATA:
		{
			CGridData*	pGridData = (CGridData*)lParam;
			SendGrid(0, pGridData);						
		}
		break;
	case MK_SAVEFOROUB:
		{
			m_GridWnd[0]->SendMessage(WM_MANAGE, MK_SAVEFOROUB);
		}
		break;

	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData*)lParam);
		break;
	case MK_RTSDATA:
		RecvRTS((CRecvData*)lParam);
		break;
	case MK_SELINT:
		SendGrid(m_nCurSel, (int)lParam);
		break;
	// ADD PSH 20070917
	case MK_MKPROP:
		{
			_marker* pMkProp = (_marker*)lParam;

			if (nullptr != pMkProp)
			{
				for ( int ii = 0 ; ii < m_cntGroup ; ii++ )
				{
					m_GridWnd[ii]->SetMarkerProp(pMkProp);
				}
			}
		}
		break;
	case MK_TOOLBUTTON:
		{
			CRect *pRect = (CRect *)lParam;
			m_GridWnd[m_nCurSel]->ShowPopupMenu(pRect->left, pRect->top + 18);
		}
		break;
	case MK_MARKETCODE:
		{
			m_bDispCode = (BOOL)lParam;

			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->SetDispCode(m_bDispCode);
				m_GridWnd[ii]->Invalidate();
			}

			m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKETCODE, lParam);
		}
		break;
	case MK_INITGRID:
		{
			//���õ� �׷쿡 ������ ������
			InitGrid();
//			RefreshGrid();
//			SelectOper();
			//Ʈ���� ���ý��� TR�߻��ϰԲ��Ѵ�.
			//���ɱ׷� ���� ��� ����
//			InitSetGroup();
			break;
 		}
	case MK_SELECTFIRST:
		selectFirst();
		break;
	case MK_ARRANGE:
		{
			arrangeGroup((int)lParam);
		}
		break;
	case MK_CHANGEVIEWTYPE:
		{
			if(((int)lParam) == SMALLTYPE_FILE)
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_SMALLCONFIG); 
			}
			else
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_LARGECONFIG); 
			}

			for(int i=0;i<m_nGroup;i++)
			{
				m_GridWnd[i]->SendMessage(WM_MANAGE,MK_CHANGEVIEWTYPE,lParam);
				m_GridWnd[i]->loadcfg();
				m_GridWnd[i]->FieldSetup(false);
				m_GridWnd[i]->Reset();
			}

			break;
		}
	case MK_VIEWREFRESH:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->ResizeField();
			}
		}
		break;
	case MK_SAVE:
		{
			m_GridWnd[0]->SendMessage(WM_MANAGE,MK_SAVE,lParam);
			for ( int ii = 0 ; ii < 2 ; ii++ )
			{
				if(m_GridWnd[ii])
					m_GridWnd[ii]->m_bEditWork = false;
			}

			DeleteOrg();
		}
		break;
	case MK_EDITWORK:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->m_bEditWork = (BOOL)lParam;
			}
		}
		break;
	case MK_ISNEWDROP:
		{
			ret = IsNewDrop();
		}
		break;
	case MK_RECOVER:
		{
			Recover();

			for ( int ii = 0 ; ii < 2 ; ii++ )
			{
				if(m_GridWnd[ii])
					m_GridWnd[ii]->m_bEditWork = false;
			}
		}
		break;
	case MK_DELETEORGFILE:
		{
			DeleteOrg();
		}
		break;
	case MK_NOSELECT:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->SelectRowClear();
			}
		}
		break;
	case MK_DUPLICATE:
		{
			ret = FALSE;

			for(int i=0;i<m_nGroup;i++)
			{
				const bool flag = m_GridWnd[i]->IsDuplicateCode((char*)lParam,(int)HIWORD(wParam));

				if(flag)
				{
					ret = TRUE;
				}
			}
		}
		break;
	case MK_CLEAR:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->ClearGrid();
			}
		}
		break;
	// END ADD
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}

	return ret;
}


void CGroupWnd::RecvOper(int kind, CRecvData* rdata)
{
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	
	if(nOver == MO_VISIBLE)
	{
		if(m_GridWnd[0]->m_SendKey == TRKEY_GRIDNEW)
		{
			m_GridWnd[0]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA2, 0), (LPARAM)rdata);
			m_GridWnd[1]->m_SendKey = TRKEY_GRIDNEW;
			m_GridWnd[1]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA2, m_GridWnd[0]->GetRowcount() * 100), (LPARAM)rdata);
		}
	}
	else
	{
		m_GridWnd[0]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
	}
	
}

void CGroupWnd::setIsRecommand(bool bFlag)
{
	m_isRecommand = bFlag;
}

bool CGroupWnd::GetIsRecommand()
{
	return m_isRecommand;
}

void CGroupWnd::loadcfg()
{
	// load resource
	m_pBrush = GetAxBrush(RGB(100, 100, 100));
	m_clrBack = GetAxColor(68);
	m_bkColor1 = GetAxColor(68);
	m_bkColor2 = GetAxColor(77);//LINECOLOR BUFFET
	m_rtmColor = GetAxColor(8);
	m_clrbookMark = GetAxColor(199);
	m_clrbookMarkPen = GetAxColor(69);
	m_chUse = 0x00;

	for (int ii = 0; ii < 4; ii++)
		m_fgColor[ii] = GetAxColor(39);

	// load info
	m_nGroup = GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);	
	m_cntGroup = m_nGroup;


	char	readB[1024];
	CString	string, stringx;
	
	memset(readB, 0x00, sizeof(readB));
	GetPrivateProfileString(SEC_GROUP, KEY_RATE, "", readB, sizeof(readB), m_fileCFG);
	string.Format("%s", readB);
	
	if (string.IsEmpty())
	{
		for ( int ii = 0 ; ii < m_nGroup ; ii++ )
		{
			string += "100,";
		}
	}

	SetArray(string);
	m_nCurSel = GetPrivateProfileInt(SEC_GROUP, KEY_SELECT, 0, m_fileCFG);

	loadfield();
}


int CGroupWnd::loadGroupCount()
{
	CString strPath, string;
	char readB[1024]{};
	strPath.Format("%s/%s/%s/portfolio.ini", m_root, USRDIR, m_user);
	int readL = 0, count = 0;
	
	readL = GetPrivateProfileString(SEC_GROUPORDER, "00", "", readB, sizeof(readB), strPath);
	string = CString(readB, readL);
	
	for(int ii=0 ; !string.IsEmpty() ; ii++)
	{
		IH::Parser(string, ";");
		count++;
	}
	
	return count;
}

void CGroupWnd::addGridWnd()
{
	for ( int ii = m_nGroup-1 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii] = std::make_shared<CGridWnd>(m_pMainWnd, ii);
			
		m_GridWnd[ii]->m_bWaveApply  = m_bWaveApply;
		m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
		m_GridWnd[ii]->m_bPopupDlg   = m_bPopupDlg;
		m_GridWnd[ii]->m_bDispCode   = m_bDispCode;
			
		m_GridWnd[ii]->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GRIDWND + ii);
			
		m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);
	}

	m_cntGroup = m_nGroup;
}

void CGroupWnd::loadfield()
{
	CString filePath;

	if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
		filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_SMALLFIELD);
	else
		filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_LARGEFIELD);

	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;
	
	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_BKCOLOR, "1", readB, sizeof(readB), filePath);

	m_chUse = readB[0];//BK ���/�̻��

	if (readB[0] == '2')
	{
		readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_COLOR2, string, readB, sizeof(readB), filePath);
		if (readL > 0)
		{
			string = CString(readB, readL);

			m_line = atoi(IH::Parser(string, deliMITER));
			if (m_line <= 0) m_line = 1;
			m_bkColor1 = _httoi(IH::Parser(string, deliMITER));
			m_bkColor2 = _httoi(IH::Parser(string, deliMITER));
		}
	}
	else
	{
		readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_COLOR1, "", readB, sizeof(readB), filePath);
		if (readL > 0)
		{
			string = CString(readB, readL);

			m_line = 1;
			m_bkColor1 = _httoi(IH::Parser(string, deliMITER));
			m_bkColor2 = m_bkColor1;
		}
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, "BOOKMARKCOLOR", "", readB, sizeof(readB), filePath);
	if (readL > 0) m_clrbookMark = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, "BOOKMARKPENCOLOR", "", readB, sizeof(readB), filePath);
	if (readL > 0) m_clrbookMarkPen = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_REALCOLOR, "", readB, sizeof(readB), filePath);
	if (readL > 0) m_rtmColor = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FRCOLOR, "", readB, sizeof(readB), filePath);

	if (readL > 0)
	{
		string = CString(readB, readL);

		for (int ii = 0; ii < 4; ii++)
			m_fgColor[ii] = _httoi(IH::Parser(string, deliMITER));
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FLASHTYPE, "", readB, sizeof(readB), filePath);
	if (readL > 0)
	{
		switch (readB[0])
		{
		case '1': m_rtmAction = rtmCELL;	break;
		case '2': m_rtmAction = rtmROW;		break;
		default:  m_rtmAction = rtmNONE;	break;
		}
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_LINKMAP, "1", readB, sizeof(readB), filePath);
	if (readL > 0)
	{
		string = CString(readB, readL);

		stringx = IH::Parser(string, deliMITER);
		if (stringx.GetLength() > 0)
		{
			switch (stringx.GetAt(0))
			{
				case '1': m_dblAction = dblCURR;	break;
				case '2': m_dblAction = dblCHART;	break;
				case '4':
						m_dblAction = dblUSER;
						m_linkMap = IH::Parser(string, deliMITER); break;
				default: m_dblAction = dblCURR;	break;
			}
		}
	}

	m_selAction = GetPrivateProfileInt(SEC_INTERFIELD, "SELACTION", 1, filePath);
	m_overlap = GetPrivateProfileInt(SEC_INTERFIELD, "OVERLAP", 0, filePath);
	m_overlapAlert = GetPrivateProfileInt(SEC_INTERFIELD, "OVERLAPALERT", 0, filePath);
	
	m_fontsize = 9;
	
	m_rowH = DEF_ROWH;
	switch (m_fontsize)
	{
	case 8:
		m_rowH = DEF_ROWH - 4;
		break;
	case 11:
		m_rowH = DEF_ROWH + 2;
		break;
	case 12:
		m_rowH = DEF_ROWH + 4;
		break;
	case 13:
		m_rowH = DEF_ROWH + 6;
		break;
	case 14:
		m_rowH = DEF_ROWH + 8;
		break;
	case 15:
		m_rowH = DEF_ROWH + 10;
		break;
}	

	m_fontname = _T("����ü");

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FONTNAME, "����ü", readB, sizeof(readB), filePath);
	
	if (readL > 0) 
		m_fontname.Format("%s", readB);


	m_bCurr = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_CURRBOLD, 0, filePath);
	m_bName = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_NAMEBOLD, 0, filePath);
	m_bAllBold = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_ALLBOLD, 0, filePath);
}

void CGroupWnd::saveOverLap(int overlap)
{
	CString filePath;
	if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
		filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_SMALLFIELD);
	else
		filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_LARGEFIELD);
	
	m_overlap = overlap;

	CString	sztmp = _T("");
	sztmp.Format("%d", overlap);
	WritePrivateProfileString(SEC_INTERFIELD, "OVERLAP", sztmp.operator LPCTSTR(), filePath);
}

void CGroupWnd::savecfg()
{
	// delete resource
	CString	sztmp = _T(""), sztmp2 = _T(""), title = _T("");
	// group count
	sztmp.Format("%d", m_nGroup);	
	WritePrivateProfileString(SEC_GROUP, KEY_COUNT, sztmp.operator LPCTSTR(), m_fileCFG);
	
	// groupwnd size rate
	sztmp = _T("");
	const int	ncnt = m_arRATE.GetSize();

	WritePrivateProfileString(SEC_GROUP, KEY_RATE, sztmp.operator LPCTSTR(), m_fileCFG);

	// save Select group 
	sztmp.Format("%d", m_nCurSel);
	WritePrivateProfileString(SEC_GROUP, KEY_SELECT, sztmp.operator LPCTSTR(), m_fileCFG);
}

void CGroupWnd::OperInit()
{
	((CMainWnd*)m_pMainWnd)->GetParamRect();
	
	loadcfg();
	CreateGrid();

	SelectOper();

	//Ʈ���� ���ý��� TR�߻��ϰԲ��Ѵ�.
	//���ɱ׷� ���� ��� ����
	
	CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));	
	m_nSortCurIndex = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

	SetTimer(MK_SELFIELD, 400, nullptr);
}

void CGroupWnd::InitSetGroup()
{
	const int gIndex = GetPrivateProfileInt(SEC_MAIN, KEY_SELECTGROUP, 0, m_fileCFG);
	const UINT   val = MAKE_TREEID(xINTEREST, 0, 2, gIndex, TD_ITEM);
}

void CGroupWnd::saveGroupIndex(int index)
{

}

//2011.12.20 KSJ ��Ʈ���� �������� �ʰ� GridWnd�� m_bSorting�� false�� ����� �ش�.
void CGroupWnd::SetInitSortingValue()
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->SetInitSortingValue();
	}
}

void CGroupWnd::InitGrid()
{
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	class CGridData	sdata;
	UINT	kind = 0;
	int	max = 0;
	CMapWordToPtr	mapNews;
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	int saveGroupInfo[MAX_GROUP]{};
	bool isFind = false;
	
	int arryIndex = 0;
	for(int i=0 ; i< MAX_GROUP ; i++)
	{
		saveGroupInfo[i] = 0;
	}

	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		sdata.Reset();
		kind = m_GridWnd[0]->GetKind();

		if(nOver == MO_VISIBLE)
		{
			if(ii == 0)
			{
				m_GridWnd[ii]->SetVScroll(GVSC_VERT);
			}
			else
			{
				m_GridWnd[ii]->SetVScroll(GVSC_VERTAUTO);
			}

			m_GridWnd[ii]->m_nGridNumber = ii;
		}
		else
		{
			if(ii == 0)
			{
				m_GridWnd[ii]->SetVScroll(GVSC_VERTAUTO);
			}

			m_GridWnd[ii]->m_nGridNumber = ii;
		}


		if (CAST_TREEID(kind)->kind == xISSUE)
		{
			mapNews.SetAt(ii, nullptr);
			continue;
		}
		else if (CAST_TREEID(kind)->kind == xREMAIN)
		{
			pWnd->SendMessage(WM_MANAGE, MK_SENDREMAIN);
			continue;
		}
		
		if (kind)
		{
			sdata.SetKind(kind);
			
			if (nOver == MO_VISIBLE)
				//				max = (m_nGroup - ii) * GetRowCount();
				max = (m_nGroup - ii) * MAX_LINE;
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;

			int i = 0;
			isFind = false;

			while(i < MAX_GROUP)
			{
				//�迭�ȿ� ������ �ִ��� Ȯ��
				if(saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}
				
				i++;
			}
			
			if(isFind == false && sdata.GetCount() > 0)						
			{
				saveGroupInfo[arryIndex] = kind;
				ii = OverOper(ii, &sdata);
				arryIndex++;
				
			}
			else
			{
				m_GridWnd[ii]->SetKind(0);
				m_GridWnd[ii]->RemoveAll();
			}
		}	
		else
		{
			m_GridWnd[ii]->SetKind(0);
			m_GridWnd[ii]->RemoveAll();
		}
	}
	
	//�����Ͱ� �ִ� �׷츸 ���� �����ش�
	if(nOver == MO_VISIBLE)
	{
		if(m_GridWnd[0]->m_pGridData != nullptr)
		{
			m_GridWnd[0]->m_pGridData = nullptr;
		}
		
		m_GridWnd[0]->m_pGridData = &sdata;
		m_GridWnd[0]->SendWhenVisibleMode(&sdata);		
	}
	else
	{
		for (int ii = 0 ; ii < m_nGroup ; ii++ )
		{
			void*	ptr = nullptr;
			if (mapNews.Lookup(ii, ptr))
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
				continue;
			}
			
			if(m_GridWnd[ii]->GetKind())
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
			}
		}
	}
}

void CGroupWnd::RefreshGrid()
{
	m_slog.Format("[cx_interest][CGroupWnd][RefreshGrid][remove]");
	OutputDebugString(m_slog);

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	UINT	kind = 0;
	CMapWordToPtr	mapNews;
	CGridData sdata;
	int	max = 0;

	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	if(nOver == MO_VISIBLE)
	{
		max = 20;
	}
	else
	{
		max = MAX_LINE;
	}

	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);

	kind = sdata.GetKind();

	if (kind == 0)
		return;

	if (CAST_TREEID(kind)->kind == xINTEREST)
	{
		const UINT datakind = m_GridWnd[0]->GetKind();
		if (datakind == kind)
			m_GridWnd[0]->Reset();
	}

	const int nEnd = OverOper(0, &sdata);
	
	if(nOver == MO_VISIBLE)
	{
		if(m_GridWnd[0]->m_pGridData != nullptr)
			m_GridWnd[0]->m_pGridData = nullptr;	
		m_GridWnd[0]->m_pGridData = &sdata;
		m_GridWnd[0]->SendWhenVisibleMode(&sdata);			
		m_GridWnd[0]->Reset();
	}
	else
	{
		m_GridWnd[0]->SendMessage(WM_MANAGE, MK_SENDTR);
	}
}


void CGroupWnd::ReSetupGrid()
{
m_slog.Format("[cx_interest][CGroupWnd][ReSetupGrid][remove]");
OutputDebugString(m_slog);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	class CGridData	sdata;
	UINT	kind = 0;
	int	max = 0;
	CMapWordToPtr	mapNews;
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	int saveGroupInfo[MAX_GROUP]{};
	bool isFind = false;
	
	int arryIndex = 0;
	for(int i=0 ; i< MAX_GROUP ; i++)
	{
		saveGroupInfo[i] = 0;
	}

	if (nOver == MO_VISIBLE)
	{
		kind = m_GridWnd[m_nCurSel]->GetKind();
		
		if(kind == 0)
		{

		}
	}

	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		sdata.Reset();

		if (nOver == MO_VISIBLE)
		{
			kind = m_GridWnd[m_nCurSel]->GetKind();
		}
		else
		{
			kind = m_GridWnd[ii]->GetKind();
		}

		if (CAST_TREEID(kind)->kind == xISSUE)
		{
			mapNews.SetAt(ii, nullptr);
			continue;
		}
		else if (CAST_TREEID(kind)->kind == xREMAIN)
		{
			pWnd->SendMessage(WM_MANAGE, MK_SENDREMAIN);
			continue;
		}
		
		if (kind)
		{
			sdata.SetKind(kind);
			
			if (nOver == MO_VISIBLE)
			{
				//max = (m_nGroup - ii) * GetRowCount();
				max = (m_nGroup - ii) * MAX_LINE;
			}
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);  //1
			
			int i = 0;
			isFind = false;

			while(i < MAX_GROUP)
			{
				//�迭�ȿ� ������ �ִ��� Ȯ��
				if(saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}
				
				i++;
			}
			
			if(isFind == false && sdata.GetCount() > 0)						
			{
				saveGroupInfo[arryIndex] = kind;
				ii = OverOper(ii, &sdata);
				arryIndex++;
			}
			else
			{
				m_GridWnd[ii]->SetKind(0);
				m_GridWnd[ii]->RemoveAll();
			}
		}	
		else
		{
			m_GridWnd[ii]->SetKind(0);
			m_GridWnd[ii]->RemoveAll();
		}
	}
	
	//�����Ͱ� �ִ� �׷츸 ���� �����ش�
	for (int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		void*	ptr = nullptr;
		if (mapNews.Lookup(ii, ptr))
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
			continue;
		}

		if(m_GridWnd[ii]->GetKind())
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
}

void CGroupWnd::SetArray(CString rates)
{	
	CString	sztmp;
	
	CRect rcRect;
	GetClientRect(&rcRect);
	CRect rcPrev = rcRect;

	m_arRATE.RemoveAll();
	
	for ( int ii = 0 ; ii < MAX_GROUP ; ii++ )
	{
		if (ii < m_nGroup)
			sztmp = IH::Parser(rates, ",");
		m_arRATE.Add(atoi(sztmp));

		if (ii < m_nGroup)
		{
			rcRect.left = rcPrev.left;
			
			if (ii > 0)
				rcRect.left += SIZE_DIVIDE;

			rcRect.right = rcRect.left + atoi(sztmp);
			rcPrev = rcRect;
			m_arRECT.Add(rcRect);
		}
	}
}

int CGroupWnd::getTotalSize()
{
	if (m_arRATE.GetSize() != MAX_GROUP)
		return -1;
	int	total = 0;

	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		total += m_arRATE.GetAt(ii);	
	}

	return total;
}

void CGroupWnd::MakeGridRect(CRect rcFull)
{
	m_arRECT.RemoveAll();
	m_arDIVIDE.RemoveAll();
	m_arUpDIV.RemoveAll();
	m_arDnDIV.RemoveAll();

	const int nTotSize = getTotalSize();
	int nGapSize = 0;
	
	nGapSize = SIZE_DIVIDE * m_nGroup +2 ;

	CRect rcGrid = rcFull;
	rcGrid.right = 0;
	CRect rcDivide = rcFull;

	CRect rcUp = rcFull;
	CRect rcDn = rcFull;
	
	rcUp.bottom = rcGrid.top;// + SIZE_DIVIDE;
	rcDn.top = rcGrid.bottom;// - SIZE_DIVIDE;

	rcGrid.top = rcGrid.top;// + SIZE_DIVIDE;//BUFFET BLOCK
	rcGrid.bottom  = rcGrid.bottom ;//- SIZE_DIVIDE;//BUFFET BLOCK

	int nWidth = rcFull.Width() / m_nGroup;

	for (int i = 0; i < m_nGroup+1; i++)//BUFFET +1
	{
		rcGrid.left = rcGrid.right;
		
		rcUp.left = rcGrid.right;
		rcDn.left = rcGrid.right;

		if (i == m_nGroup - 1 )//BUFFET +1
			nWidth = rcFull.Width() - (nWidth * (m_nGroup - 1));

		rcGrid.right = rcGrid.left + nWidth ;

		rcUp.right = rcGrid.right  ;
		rcDn.right = rcGrid.right  ;
		
		rcDivide.left = rcGrid.left;// - SIZE_DIVIDE;

		rcDivide.right = rcGrid.left;
		
		if (i < m_nGroup+1 )//BUFFET +1
			m_arDIVIDE.Add(rcDivide);// 

		if ( i < m_nGroup)
		{
			m_arRECT.Add(rcGrid);
			m_arUpDIV.Add(rcUp);
			m_arDnDIV.Add(rcDn);

			m_arRATE.SetAt(i, rcGrid.Width());
		}
	}
}

void CGroupWnd::OperResize(int cx, int cy)
{
	const CRect	rect(0, 0, cx, cy);
	CRect	rcGrid;	
	m_rcTitle = rect;
	m_rcTitle.bottom = m_rcTitle.top + 20;
	MakeGridRect(rect);

	if(m_globalRect.IsRectEmpty())
	{
		GetClientRect(m_globalRect);
	}
	else
	{
		m_xRate = (double)((double)cx / (double)m_globalRect.Width());
		m_yRate = (double)((double)cy / (double)m_globalRect.Height());

		//TRACE("m_originRect_x : %d m_originRect_y : %d cx : %d cy : %d m_xRate : %f m_yRate : %f\n", m_globalRect.Width(), m_globalRect.Height(), cx, cy, m_xRate, m_yRate);
		
		m_GridWnd[0]->SetRatio(m_xRate, m_yRate);

		if(m_nGroup > 1)
			m_GridWnd[1]->SetRatio(m_xRate, m_yRate);
	}

	int ii = 0;
	for ( ii = 0 ; ii < m_nGroup ; ii++ )
	{
		rcGrid = m_arRECT.GetAt(ii);
		m_GridWnd[ii]->MoveWindow(rcGrid, FALSE);
		m_arRATE.SetAt(ii, rcGrid.Width());
		if (!m_bMAP)
		{
			if (!m_GridWnd[ii]->IsWindowVisible())
				m_GridWnd[ii]->ShowWindow(SW_SHOW);
		}		
	}

	for ( ; ii < m_cntGroup ; ii++ )
	{
		m_GridWnd[ii]->ShowWindow(SW_HIDE);
	}

	//test 20230203
	for (int ii = 0; ii < m_cntGroup; ii++)
	{
		m_GridWnd[ii]->m_iIndex = ii;
	}

	InvalidateDivide();
}

void CGroupWnd::OperResizeMinus(int cx, int cy)
{
	const CRect	rect(0, 0, cx, cy);
	CRect	rcGrid;	
	m_rcTitle = rect;
	m_rcTitle.bottom = m_rcTitle.top + 20;
	MakeGridRect(rect);
	
	if(m_globalRect.IsRectEmpty())
	{
		GetClientRect(m_globalRect);
	}
	else
	{
		m_xRate = (double)((double)cx / (double)m_globalRect.Width());
		m_yRate = (double)((double)cy / (double)m_globalRect.Height());
		
		//TRACE("m_originRect_x : %d m_originRect_y : %d cx : %d cy : %d m_xRate : %f m_yRate : %f\n", m_globalRect.Width(), m_globalRect.Height(), cx, cy, m_xRate, m_yRate);
		
		m_GridWnd[0]->SetRatio(m_xRate, m_yRate);

		if(m_nGroup > 1)
			m_GridWnd[1]->SetRatio(m_xRate, m_yRate);
	}
	
	int ii = 0;
	for (ii = 0 ; ii < 1 ; ii++ )
	{
		rcGrid = m_arRECT.GetAt(ii);
		m_GridWnd[ii]->MoveWindow(rcGrid, FALSE);
		m_arRATE.SetAt(ii, rcGrid.Width());
		if (!m_bMAP)
		{
			if (!m_GridWnd[ii]->IsWindowVisible())
				m_GridWnd[ii]->ShowWindow(SW_SHOW);
		}		
	}
	
	for ( ; ii < 2 ; ii++ )
	{
		m_GridWnd[ii]->ShowWindow(SW_HIDE);
	}
	
	InvalidateDivide();
}

void CGroupWnd::CreateGrid()
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii] = std::make_shared<CGridWnd>(m_pMainWnd, ii);
	
		m_GridWnd[ii]->m_bWaveApply  = m_bWaveApply;
		m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
		m_GridWnd[ii]->m_bPopupDlg   = m_bPopupDlg;
		m_GridWnd[ii]->m_bDispCode   = m_bDispCode;

		m_GridWnd[ii]->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GRIDWND + ii);

		m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);

		//TRACE("CREATE GRID %d HANDLE %d",ii,m_GridWnd[ii]);
	}
}

void CGroupWnd::DestroyGrid()
{
	m_GridWnd.fill(nullptr);
}

void CGroupWnd::OperDestory()
{
	savecfg();
	DestroyGrid();
}

void CGroupWnd::DrawDivide(CDC* pDC)
{
	int	ncnt = 0;
	CRect	rect;
	
	COLORREF clrLight = GetAxColor(64);

	ncnt = m_arUpDIV.GetSize();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		rect = m_arUpDIV.GetAt(ii);
			
		if ( ncnt > 1 && m_nCurSel == ii)
			clrLight = RGB(255, 70, 70);
		else
			clrLight = GetAxColor(gClrBack);

		DrawHigV(pDC,rect,clrLight,clrLight);

		rect = m_arDnDIV.GetAt(ii);
		
		DrawHigV(pDC,rect,clrLight,clrLight);
	}

	ncnt = m_arDIVIDE.GetSize();

	for (int ii = 0 ; ii < ncnt ; ii++ )
	{
		rect = m_arDIVIDE.GetAt(ii);
			
		if ( ncnt > 2 && 
			( m_nCurSel == ii || (m_nCurSel+1) == ii) )
			clrLight = RGB(255, 70, 70);
		else
			clrLight = GetAxColor(gClrBack);
		
		DrawHighlight(pDC,rect,clrLight,clrLight);
	}
}

void CGroupWnd::DrawHigV(CDC* pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow)
{
	pDC->FillSolidRect(rect, clrShadow);
	CPen	penW(PS_SOLID, 1, clrLight),penB(PS_SOLID, 2, clrShadow);
	CPen*	oldpen = nullptr;

	oldpen = pDC->SelectObject(&penW);
	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.right, rect.top);
	pDC->SelectObject(oldpen);
	penW.DeleteObject();

	oldpen = pDC->SelectObject(&penB);
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.BottomRight());
	pDC->SelectObject(oldpen);
	penB.DeleteObject();
}

void CGroupWnd::DrawHighlight(CDC* pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow)
{
	CPen penLight, penShadow, *pOldPen;

	penLight.CreatePen(PS_SOLID, 1, clrLight);
	penShadow.CreatePen(PS_SOLID, 2, clrShadow);

	pOldPen = pDC->SelectObject(&penLight);

	pDC->SelectObject(&penLight);
		

	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);

	pDC->SelectObject(&penShadow);

	pDC->MoveTo(rect.right-1, rect.top);
	pDC->LineTo(rect.right-1, rect.bottom);

	pDC->SelectObject(pOldPen);

	penLight.DeleteObject();
	penShadow.DeleteObject();
}

int CGroupWnd::GroupPlus(BOOL bExpand)
{
	const int nWidth = m_arRATE.GetAt(m_nGroup - 1);
	const int nScrMax = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	BOOL isReset = TRUE;												//ȭ�� �����Ұ����� ����
	const BOOL bTree = (BOOL)m_pMainWnd->SendMessage(WM_MANAGE, MK_ISTREE);
	
	if(m_nGroup > 1)
		return m_nGroup;

	m_nGroup++;
	
	
	for ( int ii = m_nGroup - 2 ; ii < m_nGroup ; ii++ )
	{
		m_arRATE.SetAt(ii, nWidth);
	}
	
	if (bExpand)
	{
		if (m_arRECT.GetSize())
		{
			CRect	rect = m_arRECT.GetAt(m_arRECT.GetUpperBound());

			const int nTree = bTree ? SIZE_TREE : 0;


			if (  (m_arRECT.GetSize()+1) * rect.Width() + nTree > nScrMax ) //over screen size
			{
				m_nGroup--;
				isReset = FALSE;
				MessageBox("ǥ���Ҽ� �ִ� ������ ���� �����ϴ�.","IBK��������", MB_OK);
			}else
			{
				if(m_cntGroup < m_nGroup)
				{
					addGridWnd();
				}

				rect.OffsetRect(rect.Width() + SIZE_DIVIDE, 0);
				m_arRECT.Add(rect);
				m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SIZE, MO_PLUS), MAKELPARAM(rect.Width(), 0));
				
            }

		}
	}
	else
	{
		if(m_cntGroup < m_nGroup)
		{
			addGridWnd();
		}

		OperResize(m_rect.Width(), m_rect.Height());


	}

	int nLPARAM = 0;

	if(m_fileCFG.Find(FILE_SMALLCONFIG,0) > -1)
	{
		nLPARAM = 1;
	}
	else
	{
		nLPARAM = 2;
	}

	if(isReset == TRUE)
	{
		for(int i=0;i<m_nGroup;i++)
		{
			m_GridWnd[i]->SendMessage(WM_MANAGE,MK_CHANGEVIEWTYPE,nLPARAM);
			m_GridWnd[i]->FieldSetup(false);
			m_GridWnd[i]->Reset();
		}
	}

	return m_nGroup;
}

int CGroupWnd::GroupMinus(BOOL bExpand)
{
	if(m_nGroup>1)
	{
		m_nGroup--;
	}
	else
	{
		return 1;
	}

	int nLPARAM = 0;
	
	if(m_fileCFG.Find(FILE_SMALLCONFIG,0) > -1)
	{
		nLPARAM = 1;
	}
	else
	{
		nLPARAM = 2;
	}

//	m_nGroup--;	

	if (bExpand)
	{
		if (m_arRECT.GetSize())
		{
			const CRect	rect = m_arRECT.GetAt(m_arRECT.GetUpperBound());
			m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SIZE, MO_MINUS), MAKELPARAM(rect.Width(), 0));
		}	
	}
	else
	{
		OperResize(m_rect.Width(), m_rect.Height());
	}

	//m_nGroup++;

	if (m_nCurSel == m_nGroup)
	{
		m_nCurSel = m_nGroup - 1;
		SelectOper();
	}
 	m_GridWnd[m_nGroup]->Reset();
 	m_GridWnd[m_nGroup]->SendMessage(WM_MANAGE, MK_TURNCODE);																																																																

	for(int i=0;i<m_nGroup;i++)
	{
		m_GridWnd[i]->SendMessage(WM_MANAGE,MK_CHANGEVIEWTYPE,nLPARAM);
		m_GridWnd[i]->FieldSetup(false);
		m_GridWnd[i]->Reset();
	}

	return m_nGroup;
}

void CGroupWnd::ExpectOper(int param)
{
	bool	bExpect = false;

	if (param == MO_ON)
		bExpect = true;
	
	if(((CMainWnd*)m_pMainWnd)->m_mapName == "5000")
	{
		for ( int ii = 0 ; ii < m_nGroup ; ii++ )
		{
			m_GridWnd[ii]->toggleAction5000(bExpect);	
		}
	}
	else
	{
		for ( int ii = 0 ; ii < m_nGroup ; ii++ )
		{
			m_GridWnd[ii]->toggleAction(bExpect);	
		}
	}

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	
	if(nOver == MO_VISIBLE)
	{
	
	}
	else
	{
		m_GridWnd[0]->SendMessage(WM_MANAGE, MK_SENDTR);
	}
}

void CGroupWnd::ChangeFieldOper(int param)
{
	bool	bExpect = false;
	
	if (param == MO_ON)
		bExpect = true;
	
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->toggleAction2(bExpect);	
	}

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	
	if(nOver == MO_VISIBLE)
	{
		pWnd->SendMessage(WM_MANAGE, MK_SENDTREE);

	}
	else
	{
		m_GridWnd[0]->SendMessage(WM_MANAGE, MK_SENDTR);
	}
}


void CGroupWnd::AutoExpectOper(int param)
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->toggleAction(false);	
	}
}

void CGroupWnd::SetEqualizerField(int param)
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		if(param == 0)
		{
			m_GridWnd[ii]->FieldSetup(false, param);
		}
		else if(param == 1)
		{
			m_GridWnd[ii]->FieldSetup(false, param);
		}
	}
}


void CGroupWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDivide)
	{
		DrawMove(m_rcDIVIDE);
		DrawMove(m_rcUpDIV);
		DrawMove(m_rcDnDIV);

		m_rcDIVIDE.OffsetRect(point.x - m_xPosMove, 0);
		m_rcUpDIV.OffsetRect(point.x - m_xPosMove, 0);
		m_rcDnDIV.OffsetRect(point.x - m_xPosMove, 0);
		m_xPosMove = point.x;

		DrawMove(m_rcDIVIDE);
		DrawMove(m_rcUpDIV);
		DrawMove(m_rcDnDIV);
	}
	
	CBaseWnd::OnMouseMove(nFlags, point);
}

void CGroupWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bDivide = TRUE;
	m_xPosMove = m_xPos = point.x;
	m_nIndex = GetIndexDivide(point);

	if ( m_nIndex < 0 ) return;

	CRect	rect = GetArea(m_nIndex);
	m_rcDIVIDE = m_arDIVIDE.GetAt(m_nIndex+1);
	DrawMove(m_rcDIVIDE);

	ClientToScreen(rect);
	ClipCursor(rect);
	CBaseWnd::OnLButtonDown(nFlags, point);
}

void CGroupWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	ClipCursor(nullptr);
	if (m_bDivide && point.x != m_xPos)
	{
		if ( m_nIndex < 0 ) return;

		const CRect	rcDivide = GetMovedRect(m_nIndex, point.x - m_xPos);//BUFFET +1
		m_arDIVIDE.SetAt(m_nIndex+1, rcDivide);//BUFFET +1
		
		SetMovedRect(m_nIndex, point.x - m_xPos);

		CRect	rcL, rcR;
	
		rcL = m_arRECT.GetAt(m_nIndex);
		rcR = m_arRECT.GetAt(m_nIndex + 1);
		rcL.right = rcDivide.left;
		rcR.left = rcDivide.right;
		m_arRECT.SetAt(m_nIndex, rcL);
		m_arRECT.SetAt(m_nIndex + 1, rcR);

		m_arRATE.SetAt(m_nIndex, rcL.Width());
		m_arRATE.SetAt(m_nIndex + 1, rcR.Width());

		CRect rcGrid;
		int ii = 0;
		for ( ii = 0 ; ii < m_nGroup ; ii++ )
		{
			rcGrid = m_arRECT.GetAt(ii);
			m_GridWnd[ii]->MoveWindow(rcGrid, FALSE);
			m_arRATE.SetAt(ii, rcGrid.Width());
			if (!m_bMAP)
			{
				if (!m_GridWnd[ii]->IsWindowVisible())
					m_GridWnd[ii]->ShowWindow(SW_SHOW);
			}		
		}
		
		for ( ; ii < m_nGroup ; ii++ )
		{
			m_GridWnd[ii]->ShowWindow(SW_HIDE);
		}
		
		InvalidateDivide();
	}

	DrawMove(m_rcDIVIDE);
	DrawMove(m_rcUpDIV);//BUFFET
	DrawMove(m_rcDnDIV);//BUFFET

	m_bDivide = FALSE;
	m_rcDIVIDE.SetRectEmpty();
	m_rcUpDIV.SetRectEmpty();
	m_rcDnDIV.SetRectEmpty();

	m_xPos = -1;
	m_nIndex = -1;
	CBaseWnd::OnLButtonUp(nFlags, point);
}

int CGroupWnd::GetIndexDivide(CPoint pt)
{
	const int	ncnt = m_arDIVIDE.GetSize();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		if (m_arDIVIDE.GetAt(ii).PtInRect(pt))
		{
			if ( ii == ncnt-1 ) return -1;
			else return (ii-1);
		}
	}

	return -1;
}

CRect CGroupWnd::GetMovedRect(int nIndex, int xPos)
{
	CRect	rect = m_arDIVIDE.GetAt(nIndex+1);
	rect.OffsetRect(xPos, 0);
	return rect;
}

int CGroupWnd::SetMovedRect(int nIndex, int xPos)
{
	if ( nIndex < 0 ) return 0;

	CRect	rect = m_arUpDIV.GetAt(nIndex);
	rect.right = rect.right + xPos;
	m_arUpDIV.SetAt(nIndex, rect);

	rect = m_arDnDIV.GetAt(nIndex);
	rect.right = rect.right + xPos;
	m_arDnDIV.SetAt(nIndex, rect);

	for ( int i = nIndex + 1 ; i < nIndex + 2 ; i++ )
	{
		rect = m_arUpDIV.GetAt(i);
		rect.left = rect.left + xPos;
		m_arUpDIV.SetAt(i, rect);

		rect = m_arDnDIV.GetAt(i);
		rect.left = rect.left + xPos;
		m_arDnDIV.SetAt(i, rect);
	}

	return TRUE;
}

void CGroupWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CString	strBUF = _T("");
	for ( int ii = 0 ; ii < m_nGroup+1 ; ii++ )//BUFFET +1
	{
		strBUF += "100,";
	}
	SetArray(strBUF);
	OperResize(m_rect.Width(), m_rect.Height());
	CBaseWnd::OnLButtonDblClk(nFlags, point);
}

CRect CGroupWnd::GetArea(int nIndex)
{
	CRect	rcL, rcR;
	CRect	rcRet;
	rcL = m_arRECT.GetAt(nIndex);
	rcR = m_arRECT.GetAt(nIndex + 1);
	rcRet = rcL;
	rcRet.right = rcR.right - SIZE_GROUPMINX + 1;
	rcRet.left = rcL.left + SIZE_GROUPMINX;
	if (rcRet.right <= rcRet.left)
		rcRet = m_arDIVIDE.GetAt(nIndex+1);//BUFFET +1
	return rcRet;
}

void CGroupWnd::DrawMove(CRect rect)
{
	CClientDC	dc(this);
	const int	nMode = dc.SetROP2(R2_XORPEN);
	CBrush	*oldbrush = dc.SelectObject(m_pBrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	dc.SetROP2(nMode);
}

void CGroupWnd::SelectOper()
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SELGROUP);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_HIDECODE);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_TURNCODE);
	}

	const UINT	val = m_GridWnd[m_nCurSel]->GetKind();
	m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)val);	
}

//����ȭ�� ������ ����ǥ�õ� ȭ���� ���� �պκп� ���õǵ��� ����
void CGroupWnd::selectFirst()
{
	const UINT	CurrentKind = m_GridWnd[m_nCurSel]->GetKind();
	UINT	beforeKind = 0;

	while(m_nCurSel > 0)
	{
		beforeKind = m_GridWnd[m_nCurSel-1]->GetKind();

		if(CurrentKind != beforeKind)
		{
			break;
		}
		else
		{
			m_nCurSel--;
		}
	}

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
	m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);
	SelectOper();
}


int CGroupWnd::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X') ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		ss++;
		firsttime = false;
	}

	[[gsl::suppress(26408)]]
	free(mstr);
	return result;
}


void CGroupWnd::SendGrid(int nIndex, UINT kind)
{
	const int	max = MAX_LINE;
	CGridData sdata;
	sdata.SetKind(kind);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	m_GridWnd[nIndex]->SetKind(kind);
	m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);

	m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_SENDTR);
}


void CGroupWnd::SendGrid(int nIndex, CGridData* sdata)
{
	m_slog.Format("[cx_interest][CGroupWnd][SendGrid][remove]");
	OutputDebugString(m_slog);

	UINT	kind = sdata->GetKind();

	if(((CMainWnd*)m_pMainWnd)->m_bRemain)
	{
		kind = 4522053;
		sdata->SetKind(kind);
	}

	const int	nEnd = OverOper(nIndex, sdata);
	const  CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	if(nOver == MO_VISIBLE)
	{
		if(m_GridWnd[0]->m_pGridData != nullptr)
		{
			m_GridWnd[0]->m_pGridData = nullptr;
		}
		
		m_GridWnd[0]->m_pGridData = sdata;

		m_GridWnd[0]->SendWhenVisibleMode(sdata);		

		for (int ii = nEnd+1 ; ii < m_nGroup ; ii++ ) 
		{
			m_GridWnd[ii]->Reset();
		}	
	}
	else
	{
		m_GridWnd[0]->SendMessage(WM_MANAGE, MK_SENDTR);
	}
}

int CGroupWnd::OverOper(int nIndex, CGridData* sdata)
{
m_slog.Format("[cx_interest][CGroupWnd][OverOper][remove]");
OutputDebugString(m_slog);

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));

	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	UINT	kind = sdata->GetKind();
	const int	nOld = nIndex;
	const int	nGroup = m_nGroup - nIndex;

	m_slog.Format("------------------[cx_interest] OverOper[%d]   nGroup=[%d] nIndex=[%d] \r\n", nOver, nGroup, nIndex);
	OutputDebugString(m_slog);

	if (sdata->GetCount() == 0)
		nOver = MO_SELECT;
	
	switch (nOver)
	{
	case MO_VISIBLE:
		{		
			//int	max = LINE_SEPERATE;
			int max = MAX_LINE;

			if (nOver == MO_VISIBLE)
			{
				max = GetRowCount(); //test 20230206 Ȯ��
				//max = 24;

				const int gridWidth = m_GridWnd[nIndex]->GetWidth();
				const int intGridWidth = m_GridWnd[nIndex]->GetGridWidth();

m_slog.Format("^^^^^^^^^------------------[cx_interest] OverOper max=[%d]  gridWidth=[%d] intGridWidth=[%d] \r\n", 
max, gridWidth, intGridWidth);
OutputDebugString(m_slog);

				if((gridWidth - intGridWidth) < 0)
				{
					max = max - 1;
				}
			}

			if (max == 0)
				max = MAX_LINE;

			int	ncnt = sdata->GetCount();
			
			if(ncnt > MAX_LINE)
			{
				ncnt = MAX_LINE;	
			}

			m_GridWnd[nIndex]->SetKind(kind);

			for(int ii=nIndex ; ii<m_nGroup ; ii++)
			{
				m_GridWnd[ii]->RemoveAll();
			}
			
			int	nPos = 0;

m_slog.Format("^^^^^^^^^^------------------[cx_interest] OverOper max=[%d] ncnt=[%d] \r\n", max, ncnt);
OutputDebugString(m_slog);
			
			int kk = 0;
			for ( kk = 0 ; kk < ncnt ; kk++ )
			{
				if (kk != 0 && (kk % max) == 0)
				{
					nIndex++;

					if (nIndex == m_nGroup)
						break;

					m_GridWnd[nIndex]->RemoveAll();
					
					nPos = 0;
					//KET_DATA�� ���� nIndex�� �����ߴµ� KEY_DATA�� 0�ϰ�� �����͸� ����
					const int keydata = m_GridWnd[nIndex]->loadcfg_data();
					int beforekeydata = 0;

					if(nIndex-1 >= 0)
					{
						beforekeydata = m_GridWnd[nIndex-1]->loadcfg_data();
					}
					
					CString strKey; 
					if(keydata != beforekeydata)
					{
						strKey.Format("%d", beforekeydata);
						m_GridWnd[nIndex]->savecfg_data(strKey);
					}					
					m_GridWnd[nIndex]->SetKind(kind);					
				}				

				m_GridWnd[nIndex]->AddData(nPos++, sdata->m_arDatas.GetAt(kk));							
			}				
		
			//�׷쿡 �������� ������ ������� ������ â�� append�Ѵ�
			while(kk < ncnt)
			{
				m_GridWnd[nIndex-1]->AddData(nPos++, sdata->m_arDatas.GetAt(kk));	
				kk++;
			}
		}
		break;
	case MO_FULL:
		break;
	case MO_SELECT:
		if(((CMainWnd*)m_pMainWnd)->m_bRemain)
		{
			kind = 4522053;
			sdata->SetKind(kind);
		}

		m_GridWnd[nIndex]->SetKind(kind);
		m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)sdata);	
		break;
	}

	return nIndex;
}

void CGroupWnd::RecvRTS(CRecvData* rdata)
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_RTSDATA, rdata->m_lParam);
	}
}

//2012.02.10 KSJ
void CGroupWnd::RecvRTSx(LPARAM lParam)
{
	for (int ii = 0; ii < m_nGroup; ii++)
		m_GridWnd[ii]->RecvRTSx(lParam);
}
// KSJ

void CGroupWnd::CloseGrid(int nIndex)
{
	if (m_nGroup == 1)
		return;

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const BOOL bExpand = (BOOL)pWnd->SendMessage(WM_MANAGE, MK_ISEXPAND);
	
	if (nIndex == (m_nGroup - 1))
	{
		if (nIndex <= m_nCurSel)
			m_nCurSel--;
		if (m_nCurSel < 0)
			m_nCurSel = 0;

		GroupMinus(bExpand);
		SelectOper();
		return;
	}

	const int	width = m_arRATE.GetAt(m_arRATE.GetUpperBound());
	m_arRATE.Add(width);
	m_arRATE.RemoveAt(nIndex);

	for ( int ii = nIndex ; ii < (m_nGroup - 1) ; ii++ )
	{
		m_GridWnd[ii]->Assign(m_GridWnd[ii + 1].get());
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
	}
	
	if (nIndex <= m_nCurSel)
		m_nCurSel--;
	if (m_nCurSel < 0)
		m_nCurSel = 0;

	SelectOper();
	GroupMinus(bExpand);
}

void CGroupWnd::InvalidateDivide()
{
	int	ncnt = m_arDIVIDE.GetSize();
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		InvalidateRect(m_arDIVIDE.GetAt(ii), FALSE);
	}

	ncnt = m_arUpDIV.GetSize();
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		InvalidateRect(m_arUpDIV.GetAt(ii), FALSE);
		InvalidateRect(m_arDnDIV.GetAt(ii), FALSE);
	}
}

BOOL CGroupWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	const int	ncnt = m_arDIVIDE.GetSize();
	CPoint	pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		if (m_arDIVIDE.GetAt(ii).PtInRect(pt))
		{
			if ( ii == 0 || ii == ncnt -1 ) return TRUE;
			else
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LR));
			return TRUE;
		}
	}

	return CBaseWnd::OnSetCursor(pWnd, nHitTest, message);
}

int CGroupWnd::GetRowCount()
{
	const int	nHeight = (int)m_pMainWnd->SendMessage(WM_MANAGE, MK_GROUPHEIGHT);

	return (int)((double)nHeight / (double)m_GridWnd[0]->GetRowHeight() - 1);
}

void CGroupWnd::SetGridBkColor(COLORREF color)
{
	for (int ii = 0 ; ii < m_nGroup ; ii++ )
		m_GridWnd[ii]->SetGridBkColor(color);
        
      InvalidateDivide();
}

BOOL CGroupWnd::IsValidCode(CString strCode)
{
	if (m_GridWnd[m_nCurSel])
		return m_GridWnd[m_nCurSel]->IsValidCode(strCode);

	return FALSE;
}

void CGroupWnd::GetMinMaxWidth(int &nMinWidth, int &nMaxWidth)
{
	nMinWidth = nMaxWidth = -1;
	CRect rcGrid;
	const int nCount = m_arRECT.GetSize();

	if (nCount <= 0)
		return;

	for (int i = 0; i < nCount; i++)
	{
		rcGrid = m_arRECT.GetAt(i);

		nMinWidth = min(nMinWidth, rcGrid.Width());
		nMaxWidth = max(nMaxWidth, rcGrid.Width());
	}
}

int CGroupWnd::GetGroupCount()
{
	return m_nGroup;
}

void CGroupWnd::Recover()
{
	int gno = 0;
	CString strPath(_T("")), strTemp(_T("")), strBook(_T(""));

	const UINT kind = m_GridWnd[0]->SendMessage(WM_MANAGE,MK_GETDATAKIND);
	
	if (CAST_TREEID(kind)->kind == xINTEREST)
		gno = CAST_TREEID(kind)->value;
	
	strPath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);
	strBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);
	
	CopyFile(strPath+".org",strPath,FALSE);
	CopyFile(strBook+".org",strBook,FALSE);
	
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->RecoverMarker();
	}

	::DeleteFile(strPath+".org");
	::DeleteFile(strBook+".org");
}

void CGroupWnd::DeleteOrg()
{
	int gno = 0;
	CString strPath(_T("")), strTemp(_T("")), strBook(_T(""));

	const UINT kind = m_GridWnd[0]->SendMessage(WM_MANAGE,MK_GETDATAKIND);
	
	if (CAST_TREEID(kind)->kind == xINTEREST)
		gno = CAST_TREEID(kind)->value;
	
	strPath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);
	strBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);

	CFileFind finder;
	if (finder.FindFile(strPath+".org"))
		::DeleteFile(strPath+".org");

	if (finder.FindFile(strBook+".org"))
		::DeleteFile(strBook+".org");
}

void CGroupWnd::OnDestroySave()
{

}

void CGroupWnd::saveWhenGroupClosed(int index)
{
		for ( int ii = 0 ; ii < 2 ; ii++ )
		{
			if(m_GridWnd[ii])
				m_GridWnd[ii]->m_bEditWork = false;
		}

		GetParent()->GetParent()->GetParent()->SetFocus();
}

void CGroupWnd::saveWhenChangeViewMode(int index)
{

}

BOOL CGroupWnd::IsNewDrop()
{ 
	BOOL ret = FALSE;
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		if ( m_GridWnd[ii]->m_bEditWork )
		{
			ret = TRUE;
			break;
		}
	}

	return ret;
}

int CGroupWnd::sumEachGroupCount()
{
	UINT currentGroup = 0;
	UINT sum = 0;

	for(int index=0 ; index< GetGroupCount() ; index++)
	{	
		sum += m_GridWnd[index]->GetRowcount();
	}
	return sum;
}


int CGroupWnd::WriteFileSumtoEachGroup(UINT curruntGroup)			// index : �׷� ������
{
	UINT nextGroup{};
	int curruntGroupCount{}, nextGroupCount{}, gno{};
	CString	code = _T(""), amount = _T(""), price = _T(""), name = _T(""), bookmark = _T("");

	CString strPath(_T("")), strTemp(_T("")), strBook(_T(""));
	CString strSendData(_T(""));


	gno = curruntGroup;
	m_Inters.clear();

	for (int ii = 0; ii < GetGroupCount(); ii++)
	{
		const auto& vInters = m_GridWnd[ii]->m_inters;
		const int count = m_GridWnd[ii]->GetRowcount();	
		std::copy(vInters.begin(), vInters.begin() + count, std::back_insert_iterator(m_Inters));
	}


	if (m_Inters.size() <= 0)
		return 0;

	saveServer(gno);
	strBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);

	char szTemp[10]{};	
	struct _inters* pInters{};
	
	::DeleteFile(strBook);
	CFile	file2(strBook, CFile::modeWrite|CFile::modeCreate);
	
	sprintf(szTemp, "%02d", gno);
	CString strGrouptName = m_GridWnd[0]->GetGroupName(gno);
	curruntGroupCount = m_Inters.size();
	bool bSetBookMark = false;	//2014.06.05 KSJ ���࿡ �ϸ�ũ�� �ϳ��� �����Ǿ� ���� ������ ������ �ʿ䰡 ����.

	for(int ii=0 ; ii< curruntGroupCount ; ii++)
	{
		const auto& pInters = m_Inters.at(ii);
	
		struct _bookmarkinfo binfo;
		FillMemory(&binfo, sizeof(_bookmarkinfo), ' ');

		if(pInters->code.IsEmpty())
		{
			binfo.bookmark[0] = '0';
		}
		else
		{
			CopyMemory(binfo.code, pInters->code, min(sizeof(binfo.code), pInters->code.GetLength()));
			CopyMemory(binfo.name, pInters->name, min(sizeof(binfo.name), pInters->name.GetLength()));
			binfo.bookmark[0] = pInters->bookmark == '1' ? '1':'0';		//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.

			if(pInters->bookmark == '1' || pInters->code[0] == 'm')
				bSetBookMark = true;
		}
		
		file2.Write(&binfo, sizeof(_bookmarkinfo));
	}

	if(!bSetBookMark) ::DeleteFile(strBook);

	return gno;
}

void CGroupWnd::saveServer(int gno)
{
	if (m_Inters.size() <= 0)
		return;
	const int nScnt = m_Inters.size();

	std::string buffer;
	buffer.resize(sz_updn + sz_jinfo * nScnt + 1, ' ');

	struct _updn* updn = (struct _updn*)buffer.data();
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'G';

	CopyMemory(updn->uinfo.nblc, _T("00001"), sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';

	CString sNum = AxStd::FORMAT("%02d", gno);
	CString strGrouptName = m_GridWnd[0]->GetGroupName(gno);

	CopyMemory(updn->ginfo.gnox, sNum, sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, (LPCTSTR)strGrouptName, min(sizeof(updn->ginfo.gnam), strGrouptName.GetLength()));

	sNum.Format("%04d", nScnt);
	CopyMemory(updn->ginfo.jrec, sNum, sizeof(updn->ginfo.jrec));
	const gsl::span<struct _jinfo> spanInter((struct _jinfo*)(buffer.data() + sz_updn), nScnt);

	int ii = 0;
	for_each(spanInter.begin(), spanInter.end(), [&](auto& jinfo) {
		auto& pInters = m_Inters.at(ii++);
		if (!pInters->code.IsEmpty())   //test 20230208
		{
			FillMemory(&jinfo, sz_jinfo, ' ');
			jinfo.gubn[0] = pInters->gubn > 0 ? pInters->gubn : '0';
			CopyMemory(jinfo.code, pInters->code, min(sizeof(jinfo.code), pInters->code.GetLength()));

			if (!pInters->xprc.IsEmpty())
				CopyMemory(jinfo.xprc, pInters->xprc, min(sizeof(jinfo.xprc), pInters->xprc.GetLength()));
			if (!pInters->xnum.IsEmpty())
				CopyMemory(jinfo.xnum, pInters->xnum, min(sizeof(jinfo.xnum), pInters->xnum.GetLength()));
		}
	});

	int isize = sizeof(struct _trkey);
	char key{};
	_trkey* trkey = (struct _trkey*)&key;
	trkey->kind = TRKEY_GRIDSAVE;
	trkey->group = gno;

	CSendData sData;
	sData.SetData(trUPDOWN, key, buffer.data(), buffer.size(), "");
//AxStd::_Msg("%s", buffer.data());
m_slog.Format("************************* [cx_interest] CGroupWnd::saveServer [%s]", buffer.data());
OutputDebugString(m_slog);
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	m_pMainWnd->SendMessage(WM_MANAGE, MK_PROCDLL);
	CString string = "OnPortfolio\tok";
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
}

CString CGroupWnd::MakePacket(CString& code, CString amount, CString price, CString name, CString bookmark)
// END MODIFY
{
	code.TrimLeft(); 
	code.TrimRight();
	if (code.IsEmpty())
		return _T("");

	code += "\t";
	if (!amount.IsEmpty())
		code += amount;
	
	code += "\t";
	if (!price.IsEmpty())
		code += price;

	code += "\t";
	if (!name.IsEmpty())
		code += name;

	code += "\t";
	if (!bookmark.IsEmpty())
		code += bookmark;

	return code;
}

void CGroupWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDEvent == MK_SELFIELD)
	{
		KillTimer(MK_SELFIELD);
		
		InitSetGroup();
	}

	CBaseWnd::OnTimer(nIDEvent);
}

void	CGroupWnd::arrangeGroup(int type)
{
	m_nSortCurIndex = type;

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	if(type == 0)
	{
		m_GridWnd[0]->m_bSort = TRUE;
		m_GridWnd[0]->m_bSort = TRUE;

		pWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
		return;
	}

	CString save1[100][2];
	CString save2[100][2];
	CString save3[100][2];

	CString temp[100][2];

	for(int i=0 ; i< MAX_LINE ; i++)
	{
		temp[i][0] = "";
		temp[i][1] = "";

		m_GridWnd[0]->m_arSortData[i][0] = "";
		m_GridWnd[0]->m_arSortData[i][1] = "";
	}

	int count = 0;

	int maxRow = 0;
	CString strTemp = "";

	if(nOver == MO_VISIBLE)	
	{
		maxRow = MAX_LINE;		

		//�׸��� ������ ��������
		m_GridWnd[0]->ArrangeField(type, &save2[0]);

		if(m_nGroup > 1)
			m_GridWnd[1]->ArrangeField(type, &save3[0]);

		//�� ���� ��Ƴ���
		for(int i = 0 ; i< m_GridWnd[0]->GetRowcount() ; i++)
		{
			temp[count][0] = save2[i][0];
			temp[count][1] = save2[i][1];

			m_GridWnd[0]->m_arSortData[count][0] = save2[i][0];
			m_GridWnd[0]->m_arSortData[count][1] = save2[i][1];
			count++;
		}
		for(int i = 0 ; i< m_GridWnd[1]->GetRowcount() ; i++)
		{
			temp[count][0] = save3[i][0];
			temp[count][1] = save3[i][1];

			m_GridWnd[0]->m_arSortData[count][0] = save3[i][0];
			m_GridWnd[0]->m_arSortData[count][1] = save3[i][1];
			count++;
		}

		//����
		for(int i=0 ; i<maxRow-1 ; i++)
		{
			for(int j=i+1 ; j<maxRow ; j++)
			{
				if(temp[j][1] != "" && temp[i][1].Compare(temp[j][1]) > 0 )
				{
					strTemp = temp[i][1];
					temp[i][1] = temp[j][1];
					m_GridWnd[0]->m_arSortData[i][1] = temp[j][1];
					temp[j][1] = strTemp;
					m_GridWnd[0]->m_arSortData[j][1] = strTemp;
					
					strTemp = temp[i][0];
					temp[i][0] = temp[j][0];
					m_GridWnd[0]->m_arSortData[i][0] = temp[j][0];
					temp[j][0] = strTemp;
					m_GridWnd[0]->m_arSortData[j][0] = strTemp;
				}
			}
		}
	}
	else
	{
		maxRow = 100;
		m_GridWnd[0]->ArrangeField(type, &(m_GridWnd[0]->m_arSortData)[0]);

		for(int i=0 ; i<maxRow-1 ; i++)
		{
			for(int j=i+1 ; j<maxRow ; j++)
			{
				if(m_GridWnd[0]->m_arSortData[j][1] != "" && m_GridWnd[0]->m_arSortData[i][1].Compare(m_GridWnd[0]->m_arSortData[j][1]) > 0 )
				{
					strTemp = m_GridWnd[0]->m_arSortData[i][1];
					m_GridWnd[0]->m_arSortData[i][1] = m_GridWnd[0]->m_arSortData[j][1];
					m_GridWnd[0]->m_arSortData[j][1] = strTemp;
					
					strTemp = m_GridWnd[0]->m_arSortData[i][0];
					m_GridWnd[0]->m_arSortData[i][0] = m_GridWnd[0]->m_arSortData[j][0];
					m_GridWnd[0]->m_arSortData[j][0] = strTemp;
				}
			}
		}
	}
	
	//�����Ͱ� �ִ� �׷츸 ���� �����ش�
	if(nOver == MO_VISIBLE)
	{
		m_GridWnd[0]->SendWhenSortVisibleMode((&(m_GridWnd[0]->m_arSortData))[0]);		
	}
	else
	{
		m_GridWnd[0]->SendWhenSortVisibleMode((&(m_GridWnd[0]->m_arSortData))[0]);
	}
}

void	CGroupWnd::SetKillFocus(int view)
{
	m_GridWnd[view]->killFocus();
}

void	CGroupWnd::setFontSize(int size)
{
	m_GridWnd[0]->SetFontSize(size);

	if(m_nGroup > 1)
		m_GridWnd[1]->SetFontSize(size);
}