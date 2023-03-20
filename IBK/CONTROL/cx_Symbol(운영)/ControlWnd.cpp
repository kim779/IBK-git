// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Symbol.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"
#include "sk_pc_identity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int indicatorW = 11;
const int bitmapW = 30;

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

#define OOP_REQ_17414		245

CControlWnd::CControlWnd()
	: m_pTBmp(nullptr)
	, m_pBBmp(nullptr)
	, m_pRBmp(nullptr)
	, m_pTipCtrl(nullptr)
{
	EnableAutomation();

	m_capture = false;
	m_bCheck = false;
	m_pPopup = nullptr;
	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bMouseOver = false;
	m_sData = "";

	m_sCode = _T("");
	m_ujongsise = false;
	m_memoexist = false;
//	m_bitmap = NULL;

	m_bFromDLL = FALSE;
	m_bSideTime = FALSE;

	m_sMkMsg.Empty();
}

CControlWnd::~CControlWnd()
{
	if (m_pPopup)
	{
		if (IsWindow(m_pPopup->GetSafeHwnd()))
			m_pPopup->SendMessage(WM_CLOSE);
		m_pPopup->Detach();
		m_pPopup.reset();
	}
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
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "visible", m_visible, OnVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CControlWnd, "sInfo", m_sInfo, OnSInfoChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "sMkMsg", m_sMkMsg, OnSMkMsgChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetSideTime", SetSideTime, VT_EMPTY, VTS_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {AA69DFD8-D09A-4295-8B0B-0AC78A214F10}
static const IID IID_IControlWnd =
{ 0xaa69dfd8, 0xd09a, 0x4295, { 0x8b, 0xb, 0xa, 0xc7, 0x8a, 0x21, 0x4f, 0x10 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

void CControlWnd::load_jinfo(char* pData)
{
	CString strTemp;
	FILE	*fp = fopen("jinfo.txt", "w");

	fprintf(fp, "%s", pData);
	fclose(fp);

	//2013.06.14 KSJ �ŸŴ���, �����ڹ��� �ɺ��߰�
	strTemp.Format("%s", pData);
	strTemp = strTemp.Right(strTemp.GetLength() - L_jinfo);
	strTemp.Replace("\t", "");
	m_jinfo.str1377.Format("%d", atoi(strTemp));
	m_jinfo.str1971 = strTemp.Mid(m_jinfo.str1377.GetLength());
	//KSJ

	struct	_jinfo	jinfo{};
	CopyMemory(&jinfo, pData, L_jinfo);	
	m_jinfo.codx = CString(jinfo.codx, sizeof(jinfo.codx));
	m_jinfo.hnam = CString(jinfo.hnam, sizeof(jinfo.hnam));
	m_jinfo.jgub = CString(jinfo.jgub, sizeof(jinfo.jgub));
	m_jinfo.size = CString(jinfo.size, sizeof(jinfo.size));
	m_jinfo.k2gb = CString(jinfo.k2gb, sizeof(jinfo.k2gb));
	m_jinfo.usun = CString(jinfo.usun, sizeof(jinfo.usun));
	m_jinfo.ksmm = CString(jinfo.ksmm, sizeof(jinfo.ksmm));
	m_jinfo.amga = CString(jinfo.amga, sizeof(jinfo.amga));
	m_jinfo.jbkm = CString(jinfo.jbkm, sizeof(jinfo.jbkm));
	m_jinfo.siga = CString(jinfo.siga, sizeof(jinfo.siga));
	m_jinfo.sjjs = CString(jinfo.sjjs, sizeof(jinfo.sjjs));
	m_jinfo.jrat = CString(jinfo.jrat, sizeof(jinfo.jrat));
	m_jinfo.nrat = CString(jinfo.nrat, sizeof(jinfo.nrat));	// 20070718
	

	// extended infomation
	m_jinfo.krgb = CString(jinfo.krgb, sizeof(jinfo.krgb));
	m_jinfo.jggb = CString(jinfo.jggb, sizeof(jinfo.jggb));
	m_jinfo.lock = CString(jinfo.lock, sizeof(jinfo.lock));
	m_jinfo.dyga = CString(jinfo.dyga, sizeof(jinfo.dyga));
	m_jinfo.ujgb = CString(jinfo.ujgb, sizeof(jinfo.ujgb));
	m_jinfo.ujcd = CString(jinfo.ujcd, sizeof(jinfo.ujcd));
	m_jinfo.jisu = CString(jinfo.jisu, sizeof(jinfo.jisu));
	m_jinfo.diff = CString(jinfo.diff, sizeof(jinfo.diff));
	m_jinfo.rate = CString(jinfo.rate, sizeof(jinfo.rate));
	m_jinfo.gvol = CString(jinfo.gvol, sizeof(jinfo.gvol));
	m_jinfo.unio = CString(jinfo.unio, sizeof(jinfo.unio));
	m_jinfo.frgb = CString(jinfo.frgb, sizeof(jinfo.frgb));
	m_jinfo.rlnk = CString(jinfo.rlnk, sizeof(jinfo.rlnk));
	m_jinfo.rned = CString(jinfo.rned, sizeof(jinfo.rned));
	m_jinfo.dist = CString(jinfo.dist, sizeof(jinfo.dist));
	m_jinfo.vist = CString(jinfo.vist, sizeof(jinfo.vist));
	m_jinfo.vitime = CString(jinfo.vitime, sizeof(jinfo.vitime));
	m_jinfo.vitype = CString(jinfo.vitype, sizeof(jinfo.vitype));
	m_jinfo.sijang = CString(jinfo.sijang, sizeof(jinfo.sijang));

	CString strCode = m_jinfo.codx;
	strCode.TrimRight();
	m_sInfo = m_jinfo.krgb;
	m_sInfo.TrimRight();

	m_sCode = m_jinfo.codx; m_sCode.Trim();
	m_memoexist = checkMemo(m_sCode);

	loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);

	m_jinfo.hnam.TrimRight();

	m_sData = m_jinfo.hnam;

	m_jinfo.jisu.Replace("+", ""); m_jinfo.jisu.Replace("-", "");
	Invalidate();
}

void CControlWnd::load_jinfo3(char* pData)
{
	CString  stmp;
	struct	_jinfo3 jinfo {};

	CopyMemory(&jinfo, pData, L_jinfo3);	

	m_jinfo.codx = CString(jinfo.codx, sizeof(jinfo.codx));
	m_jinfo.hnam = CString(jinfo.hnam, sizeof(jinfo.hnam));
	m_jinfo.jgub = CString(jinfo.jgub, sizeof(jinfo.jgub));
	m_jinfo.jrat = CString(jinfo.jrat, sizeof(jinfo.jrat));
	m_jinfo.nrat = CString(jinfo.nrat, sizeof(jinfo.nrat));	// 20070718

	// extended infomation
	m_jinfo.krgb = CString(jinfo.krgb, sizeof(jinfo.krgb));	
	m_jinfo.rned = CString(jinfo.rned, sizeof(jinfo.rned));
	m_jinfo.dist = CString(jinfo.dist, sizeof(jinfo.dist));
	m_jinfo.vist = CString(jinfo.vist, sizeof(jinfo.vist));
	m_jinfo.vitime = CString(jinfo.vitime, sizeof(jinfo.vitime));
	m_jinfo.vitype = CString(jinfo.vitype, sizeof(jinfo.vitype));
	m_sMkMsg = CString(jinfo.fill, 1);

	m_sCode = m_jinfo.codx; m_sCode.Trim();
	m_memoexist = checkMemo(m_sCode);
	m_sInfo = m_jinfo.krgb;
	m_sInfo.TrimRight();
	loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);

	m_jinfo.hnam.TrimRight();
	
	m_sData = m_jinfo.hnam;
	Invalidate();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		return (long)m_rtnStr.GetString();

	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		if (exth->size >= L_jinfo)
		{
			load_jinfo(exth->data);

			CRect	rc;

			GetClientRect(&rc);
			if (!m_sData.IsEmpty() && rc.PtInRect(m_ptUp))
			{
				GetWindowRect(&rc);
				if (m_pPopup)
					m_pPopup.reset();

				if (!m_pPopup)
				{
					int	cx = 225, cy = 272;//m_ujongsise ? 190 : 150;	//KSJ 2012.05.04 �ŸŴ��� �߰��Ǿ� 250-> 272�� ������
					if (m_jinfo.jgub.GetAt(0) == 'A')//2013.06.11 KSJ �ڳؽ� �߰�
					{
						cx = 225;
						cy = 294;
					}

					CString sClassName = AfxRegisterWndClass(0);

					m_pPopup = std::make_unique<CInfoPopup>();
					m_pPopup->m_pWizard = m_pParent;
					if (!m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, sClassName, NULL,
						WS_POPUP | WS_BORDER, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy),
						NULL, NULL, NULL))
					{
						m_pPopup = NULL;
					}
					else
					{
						m_pPopup->setJinfo(m_jinfo, m_ujongsise);
						m_pPopup->setPathInfo(m_sRoot, m_sUserID);
						//m_pPopup->m_bShow = FALSE;
						m_pPopup->ShowWindow(SW_SHOWNORMAL);
						//m_pPopup->m_bShow = TRUE;
					}
				}
			}

			m_memoexist = checkMemo(m_sCode); Invalidate();
		}
		else if (exth->size >= L_jinfo3)
		{
			load_jinfo3(exth->data);
		}
		else
		{
			m_sData.Empty();
			m_pTBmp = NULL;
			m_pBBmp = NULL;
			m_pRBmp = NULL;
			Invalidate();
		}
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		if (HIWORD(wParam) >= L_jinfo )
		{
			load_jinfo((char *)lParam);

			CRect	rc;

			GetClientRect(&rc);
			if (!m_sData.IsEmpty() && rc.PtInRect(m_ptUp))
			{
				GetWindowRect(&rc);
				if (m_pPopup)
					m_pPopup.reset();
			
				if (!m_pPopup)
				{
					int	cx = 225, cy = 272;//m_ujongsise ? 190 : 150;	//KSJ 2012.05.04 �ŸŴ��� �߰��Ǿ� 250-> 272�� ������
					if (m_jinfo.jgub.GetAt(0) == 'A')//2013.06.11 KSJ �ڳؽ� �߰�
					{
						cx = 225;
						cy = 294;
					}

					CString sClassName  = AfxRegisterWndClass(0);

					m_pPopup = std::make_unique<CInfoPopup>();
					m_pPopup->m_pWizard = m_pParent;
					if (!m_pPopup->CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_DLGMODALFRAME, sClassName, NULL,
						WS_POPUP|WS_BORDER, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy),
						NULL, NULL, NULL))
					{
						m_pPopup = NULL;							
					}
					else
					{
						m_pPopup->setJinfo(m_jinfo, m_ujongsise);
						m_pPopup->setPathInfo(m_sRoot, m_sUserID);
						//m_pPopup->m_bShow = FALSE;
						m_pPopup->ShowWindow(SW_SHOWNORMAL);
						//m_pPopup->m_bShow = TRUE;
					}						
				}
			}

			m_memoexist = checkMemo(m_sCode); Invalidate();
		}
		else if (HIWORD(wParam) >= L_jinfo3)
		{
			load_jinfo3((char *)lParam);  
		}
		else
		{
			m_sData.Empty(); 
			m_pTBmp = NULL;
			m_pBBmp = NULL;
			m_pRBmp = NULL;
			Invalidate();
		}
		break; 

//	case DLL_ALERT:
//		WriteAlert((char*)lParam);
//		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		
		alertR = (struct _alertR*)lParam;
		parseAlert(alertR);
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		Invalidate();
		break;
	
	case DLL_SETFONT:
		{
			CRect	rc;
			CString tmp = CString((char*)lParam);
			if (!tmp.IsEmpty())
				m_Param.fonts = tmp;
			Invalidate();
			
			GetClientRect(&rc);
			m_Param.point = HIWORD(wParam);
			if (rc.Height() == 20)
				m_Param.point = 9;
		}
		break;
	}

	return 0;
}

void CControlWnd::parseAlert(struct _alertR* alert)
{
	CString sym, val;
	bool	bChanged = false;
	CString rned,dist,vist,vitime, vitype;

	if (alert->code == m_sCode)
	{
		DWORD*	data = nullptr;

		for (int ii = alert->size - 1; ii >= 0; ii--)
		{
			data = (DWORD *)alert->ptr[ii];
			
			if (data[950])		//�������
			{
				dist = (char*)data[950];

				bChanged = true;
			}

			if (data[951])       //��������
			{
				rned = (char*)data[951];

				bChanged = true;
			}

			if (data[701])      //������
			{
				vist   = (char*)data[701];
				vitime = (char*)data[702];
				vitype = (char*)data[703];

				bChanged = true;
			}

			if (data[191])   //��������
			{ 
				val.Format("%s", (char *)data[191]);  //19�� ��� ����
				val.Trim();
			
				if (val == "19")
				{
					if (m_jinfo.krgb.Find("����") < 0)
					{
						m_jinfo.krgb = "����";
						m_sInfo = m_jinfo.krgb;
						loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);
						Invalidate();	
					}
					
				}
				else if (val == "0")
				{
					if (m_jinfo.krgb.Find("����") < 0)
					{
						m_jinfo.krgb = "����";
						m_sInfo = m_jinfo.krgb;
						loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);
						Invalidate();	
					}
				}
			}

			if (bChanged)
			{
				loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb,rned,dist,vist,vitime,vitype);

				Invalidate();
			}
		}
	}
}

void CControlWnd::LoadLoanGrade( void )
{
	CStdioFile file;
	CString strPath;

	strPath.Format("%s/tab/loangrade.mst", m_sRoot);
	if (!file.Open(strPath, CFile::modeRead | CFile::shareDenyNone))
		return;	

	CString str;
	CString strCode, strGrade, strName;
	int	iFind = 0;
	int	iOld = 0;
	sGrade	info;

	while (file.ReadString(str))
	{
		iOld = str.Find( ';', iFind+1 );
		if (iOld < 0 )
			continue;

		strCode = str.Mid(iFind,iOld-iFind);
		iFind = iOld;

		iOld = str.Find( ';', iFind+1 );
		if (iOld < 0 )
			continue;

		strName = str.Mid(iFind,iOld-iFind);
		iFind = iOld;

		strGrade = str.Right( 2 );
 
		strCode.TrimRight();
		info.strCode  = strCode.Left(7);
		info.strName  = strName;
		info.strGrade = strGrade;

		m_arrGrade.Add( info );
	}

	file.Close();
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CString txt;
	CPen	Pen;
	CRect	rc, rect;
	COLORREF bxColor = 0, bgColor = 0, fgColor = 0;

	GetClientRect(&rc);
	// set font
	CFont	*pFont = getAxFont(m_Param.fonts, m_Param.point, m_Param.style);
	CFont	*pOldFont = (CFont*)dc.SelectObject(pFont);

	if (!m_bHover)
	{
		bxColor = m_boxColor;
		bgColor = GetIndexColor(m_Param.pRGB);
		fgColor = GetIndexColor(130);//m_Param.tRGB);
	}
	else
	{
		bxColor = m_boxColorHover;
		bgColor = m_pRgbHover;
		fgColor = m_tRgbHover;
	}

	Pen.CreatePen(PS_SOLID, 0, bxColor);
	CPen	*pOldPen = (CPen*)dc.SelectObject(&Pen);
	dc.Rectangle(rc);
	dc.SelectObject(pOldPen); Pen.DeleteObject();

	rc.DeflateRect(1, 1);
	if (GetState() & 0x0004)
		dc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	else
		dc.FillSolidRect(rc, bgColor);

	rc.InflateRect(1, 1);

//	drawTriangle(&dc, rc, fgColor);

	rect = rc; rect.left = rect.right - indicatorW;
	drawIndicator1(&dc, rect, RGB(128, 128, 128));	// ��������
	if (m_memoexist)
		drawIndicator2(&dc, rect, RGB( 68, 225, 115));	// �޸𿩺�
	rc.right -= indicatorW;

	// bitmap draw
	rect = rc; rect.right = rect.left + bitmapW;
//	drawBitmap(&dc, rect, m_bitmap);
	DrawBitmap(&dc, rect);
	
	rc.left += bitmapW + 1;

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(fgColor);
	dc.DrawText(m_sData, rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		
	dc.SelectObject(pOldFont);
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_capture)
	{
		SetCapture();
		m_capture = true;
		m_bMouseOver = true;
		Invalidate();
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CString strINI;

	strINI.Format("%s\\User\\%s\\userconf.ini", m_sRoot, m_sUserID);

	CRect rc;

	GetClientRect(&rc);
	
	rc.left = 11;
	rc.right = 27;

	if (m_chjgub == '8' || m_chjgub == '9' || m_chjgub == '0')
	{
		if (rc.PtInRect(point))
		{
			m_capture = false;
			ReleaseCapture();
			
			CString tmp;
			
			tmp = m_jinfo.codx;
			
			tmp = tmp.Mid(1);
			
			WritePrivateProfileString("IB202600", "POPUP_FLAG", "1", strINI);
			WritePrivateProfileString("IB202600", "POPUP_CODE", tmp, strINI);
			
			CString string;
			
			string.Format("IB202600 /S/t0/d%s\t%s", "1301", m_jinfo.codx);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)string.GetString());
			
			Invalidate();
		}
		else
		{
			if (m_capture)
			{
				char	szBuf[64]{};
				int	datL = 0;

				m_capture = false;
				ReleaseCapture();
				datL = sprintf(szBuf, "1301%c%s\t17414\t1377\t1971\t",0x7f, (char *)m_sCode.GetString());	//2013.06.14 KSJ �ŸŴ���, �����ڹ��� �ɺ��߰�
				if (m_bFromDLL)
					SendTR_Dll(szBuf, datL);
				else
					SendTR_Control("POOPPOOP", szBuf, datL, US_OOP|US_PASS);

				m_ptUp = point;
				Invalidate();
			}
		}
	}
	else
	{
		if (m_capture)
		{
			char	szBuf[64]{};
			int	datL = 0;

			m_capture = false;
			ReleaseCapture();
			
			datL = sprintf(szBuf,"1301%c%s\t17414\t1377\t1971\t",0x7f, (char *)m_sCode.GetString());		//2013.06.14 KSJ �ŸŴ���, �����ڹ��� �ɺ��߰�
			if (m_bFromDLL)
				SendTR_Dll(szBuf, datL);
			else
				SendTR_Control("POOPPOOP", szBuf, datL, US_OOP|US_PASS);

			m_ptUp = point;
			Invalidate();
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	/*if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CRect rc; GetClientRect(&rc);
	if (rc.PtInRect(point))
	{
		if (!m_bMouseOver)
			Invalidate();
		m_bMouseOver = true;
	}
	else
	{
		if (m_bMouseOver)
			Invalidate();
		m_bMouseOver = false;
	}*/

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CControlWnd::GetState()
{
	if (m_capture && m_bMouseOver)
		return 0x0004;
	return 0;
}

void CControlWnd::SetCheck(bool bCheck)
{
	m_bCheck = bCheck; Invalidate();
}

bool CControlWnd::GetCheck()
{
	return m_bCheck;
}

void CControlWnd::OnDestroy() 
{
	if (m_pTipCtrl)
		m_pTipCtrl.reset();

	CWnd::OnDestroy();	
}

COLORREF CControlWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

LOGFONT CControlWnd::setFont(CString fName, int fsize, int style)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, fName);  
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	style = 3;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		lf.lfItalic = TRUE;
		break;
	case 3: // bold
		lf.lfWeight = FW_BOLD;
		break;
	case 4: // both
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_BOLD;
		break;
	}

	return lf;
}

void CControlWnd::SetParam(_param *pParam)
{
	CString	string, text, tmps, keys;

	m_sUserID = Variant(nameCC, "");
	m_sRoot   = Variant(homeCC, "");

	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	tmps	= _T("/ ");
	keys	= _T("abcdei");
	string	= pParam->options;
	for (int ii = 0, idx = 0, pos = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = string.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = string.Find('/', idx);

		text = (pos < 0) ? string.Mid(idx) : string.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'a':	// box color
			if (text.IsEmpty())
				m_boxColor = GetIndexColor(89);
			else
				m_boxColor = GetIndexColor(atoi(text));
			break;
		case 'b':	// hover box color
			if (text.IsEmpty())
				m_boxColorHover = GetIndexColor(91);
			else
				m_boxColorHover = GetIndexColor(atoi(text));
			break;
		case 'c':	// hover background color
			if (text.IsEmpty())
				m_pRgbHover = GetIndexColor(92);
			else
				m_pRgbHover = GetIndexColor(atoi(text));
			break;
		case 'd':
			if (text.IsEmpty())
				m_tRgbHover = GetIndexColor(130);//69);
			else
				m_tRgbHover = GetIndexColor(130);//atoi(text));
			break;
		case 'e':
			m_ujongsise = true;
			break;
		case 'i':
			m_bFromDLL = TRUE;
			break;
		}
	}
}

void CControlWnd::drawTriangle(CDC *pDC, CRect rc, COLORREF clr)
{
	if (rc.Width() < 3)
		return;

	const	CSize	sz(10, 8);

	pDC->FillSolidRect(rc.right - sz.cx, rc.bottom - sz.cy, 5, 1, clr);
	pDC->FillSolidRect(rc.right - sz.cx + 1, rc.bottom - sz.cy + 1, 3, 1, clr);
	pDC->FillSolidRect(rc.right - sz.cx + 2, rc.bottom - sz.cy + 2, 1, 1, clr);
}

// updateX_20060110
void CControlWnd::drawIndicator1(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	int	gap = 0, height = 0;

	CPen*   oldPen   = dc->SelectObject(getAxPen(clr, 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(getAxBrush(clr));

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	gap    = rect.Height() / 6;
	height = (rect.Height() - gap) / 2;

	rect.top  = rect.bottom - height;
	rect.left = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.bottom;
	pts[1].x = rect.right;
	pts[1].y = rect.top;
	pts[2].x = rect.right;
	pts[2].y = rect.bottom;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

// updateX_20060110, �޸�ǥ�ÿ���
void CControlWnd::drawIndicator2(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	int	gap = 0, height = 0;

	CPen*   oldPen   = dc->SelectObject(getAxPen(clr, 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(getAxBrush(clr));

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	gap    = rect.Height() / 5;
	height = (rect.Height() - gap) / 2;

	rect.bottom = rect.top + height;
	rect.left   = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.top;
	pts[1].x = rect.right;
	pts[1].y = rect.bottom;
	pts[2].x = rect.right;
	pts[2].y = rect.top;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

void CControlWnd::DrawBitmap(CDC* pDC, CRect rect)
{
	rect.DeflateRect(2, 2, 1, 2);

	if (m_pRBmp == NULL)
		return;

	CDC memDC;
	CBitmap* pOldBmp = nullptr;
	BITMAP bmpInfo;
	memDC.CreateCompatibleDC(pDC);
	m_pRBmp->GetBitmap(&bmpInfo);

	if (memDC.m_hDC != NULL)
		pOldBmp = (CBitmap*)memDC.SelectObject(m_pRBmp);
    //������ ���ø� �̹��� ũ��� 12 ,������ ���ø� �̹��� ũ��� 16
	//���ø� �̹����� �ƴҰ��� �� �ΰ����� ũ�⸦ ���ļ� �׸��� ��
	//m_bBmpSlice�� �̹� �ϳ��� �̹����� �׸��� �Ǿ�����
	if (m_bBmpSlice)
	{
		pDC->StretchBlt(rect.left + 12, rect.top, rect.Width() - 12, rect.Height(),
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

		if (m_pTBmp != NULL)
		{
			m_pTBmp->GetBitmap(&bmpInfo);
			memDC.SelectObject(m_pTBmp);
			pDC->StretchBlt(rect.left, rect.top, rect.Width() - 16, rect.Height() / 2,
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		}

		if (m_pBBmp != NULL)
		{
			m_pBBmp->GetBitmap(&bmpInfo);
			memDC.SelectObject(m_pBBmp);
			pDC->StretchBlt(rect.left, rect.top + rect.Height() / 2, rect.Width() - 16, rect.Height() / 2,
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		}
	}
	else
	{
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	}

	if (pOldBmp)
		memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
}

void CControlWnd::drawBitmap(CDC *pDC, CRect rect, CBitmap *pBitmap)
{
	rect.DeflateRect(2, 2, 1, 2);

	if (pBitmap)
	{
		CDC	memDC;
		CBitmap	*pSBitmap = nullptr;
		BITMAP stBitmapInfo{};

		pBitmap->GetBitmap(&stBitmapInfo);
		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
				&memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
		if (pSBitmap)
			memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CControlWnd::WriteAlert(CString str)
{
	CString sym, dat;
	bool bChanged = false;
	while (!str.IsEmpty())
	{
		sym = Parser(str, "\t");
		dat = Parser(str, "\t");

		switch (atoi(sym))
		{
		case 23:	// ���簡
			m_jinfo.jisu = dat;
			m_jinfo.jisu.Replace("+", "");
			m_jinfo.jisu.Replace("-", "");
			bChanged = true;
			break;
		case 24:	// ���ϴ��
			m_jinfo.diff = dat;
			bChanged = true;
			break;
		case 33:	// �����
			m_jinfo.rate = dat;
			bChanged = true;
			break;
		case 27:	// �ŷ���
			m_jinfo.gvol = dat;
			bChanged = true;
			break;
		}
	}
	if (bChanged && m_pPopup)
	{
		m_pPopup->setJinfo(m_jinfo, m_ujongsise);
		m_pPopup->Invalidate();
	}
}

LRESULT CControlWnd::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover = TRUE;
	Invalidate();
	return 0;
}


LRESULT CControlWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;
	Invalidate();
	return 0;
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CControlWnd::SetProperties(LPCTSTR sParam) 
{
	CString	string, text, tmps, keys;
	
	tmps	= _T("/ ");
	keys	= _T("abcd");
	m_Param.options = sParam;
	string	= m_Param.options;

	for (int ii = 0, idx = 0, pos = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = string.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = string.Find('/', idx);

		text = (pos < 0) ? string.Mid(idx) : string.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'a':	// box color
			if (text.IsEmpty())
				m_boxColor = GetIndexColor(89);
			else
				m_boxColor = GetIndexColor(atoi(text));
			break;
		case 'b':	// hover box color
			if (text.IsEmpty())
				m_boxColorHover = GetIndexColor(91);
			else
				m_boxColorHover = GetIndexColor(atoi(text));
			break;
		case 'c':	// hover background color
			if (text.IsEmpty())
				m_pRgbHover = GetIndexColor(92);
			else
				m_pRgbHover = GetIndexColor(atoi(text));
			break;
		case 'd':	// hover foreground color
			if (text.IsEmpty())
				m_tRgbHover = GetIndexColor(69);
			else
				m_tRgbHover = GetIndexColor(atoi(text));
			break;
		}
	}
}

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;

	return strResult.AllocSysString();
}

CPen* CControlWnd::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CBrush* CControlWnd::getAxBrush(COLORREF clr)
{
	return (CBrush*) m_pParent->SendMessage(WM_USER, getBRUSH, (LPARAM)clr);
}

CFont* CControlWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	style = 3;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

CBitmap* CControlWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

// updateX_20060110
bool CControlWnd::checkMemo(CString sKey)
{
	if (sKey.IsEmpty())
		return false;

	CString dir, dat;
	char	key[12]{}, lBytes[4]{};

	dir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, "user", m_sUserID);
	CFileFind	finder;
	if (!finder.FindFile(dir))
		return false;

	TRY
	{ 
		CFile file(dir, CFile::modeRead | CFile::shareDenyNone);
		UINT	nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);

					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}

					if (!sKey.Compare(key) && lSize > 0)
					{
						dat.ReleaseBuffer();
						return true;
					}

					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
				
		}while ((int)nBytesRead);
		file.Close();
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	return false;
}

void CControlWnd::loadBitmap(CString jrab, CString nrat, CString jgub, CString kind, CString rned, CString dist, CString vist, CString vitime, CString vitype)
{
	CString strImgPath;
	CString strTemp;

	kind.Trim();
	m_strCaseTip.Empty();

	const	int	nRat = atoi(m_jinfo.nrat);
	if (vist == _T("1"))
	{

		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/btn_vi.bmp", m_sRoot);
		m_pRBmp = getBitmap(strImgPath);
		
		vitype.Trim();

		strTemp = vitime;
		if (vitype == _T("1")){
			m_strCaseTip = "[���� ������ ��ȭ��ġ �ߵ� "+ strTemp.Mid(0,2) + ":" + strTemp.Mid(2,2) + ":" + strTemp.Mid(4,2) + "]";
		}
		else if (vitype == _T("2")){
			m_strCaseTip = "[���� ������ ��ȭ��ġ �ߵ� "+ strTemp.Mid(0,2) + ":" + strTemp.Mid(2,2) + ":" + strTemp.Mid(4,2) + "]";
		}
		else{
			m_strCaseTip = "[����/���� ������ ��ȭ��ġ �ߵ� "+ strTemp.Mid(0,2) + ":" + strTemp.Mid(2,2) + ":" + strTemp.Mid(4,2) + "]";
		} 

		
		m_chjgub = -1;			// �� �ʱ�ȭ
		
		return;
	}

	else 
	{
		if (m_bSideTime == FALSE)
		{
			if (rned == _T("12") || rned == _T("14"))
			{
				m_bBmpSlice = FALSE;
				strImgPath.Format("%s/image/����.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);

				m_strCaseTip = "���ϰ��Ÿ� ���ǿ���";
				
				m_chjgub = -1;			// �� �ʱ�ȭ

				return;
			}
			else if (dist != "")
			{
				if (dist.GetLength() > 1)
				{
					if (dist.GetAt(0) == '1')
					{
						m_bBmpSlice = FALSE;
						strImgPath.Format("%s/image/���.bmp", m_sRoot);
						m_pRBmp = getBitmap(strImgPath);

						m_strCaseTip = "����ȣ�� �������";
						
						m_chjgub = -1;			// �� �ʱ�ȭ

						return;
					}
				}
			}
		}
		else
		{
			if (rned == _T("16"))
			{
				m_bBmpSlice = FALSE;
				strImgPath.Format("%s/image/����.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				
				m_strCaseTip = "���ϰ��Ÿ� ���ǿ���";
				
				m_chjgub = -1;			// �� �ʱ�ȭ
				
				return;
			}
			else if (dist != "")
			{
				if (dist.GetLength() > 1)
				{
					if (dist.GetAt(0) == '1')
					{
						m_bBmpSlice = FALSE;
						strImgPath.Format("%s/image/���.bmp", m_sRoot);
						m_pRBmp = getBitmap(strImgPath);
						
						m_strCaseTip = "����ȣ�� �������";
						
						m_chjgub = -1;			// �� �ʱ�ȭ
						
						return;
					}
				}
			}
		}
	}
	if (kind == _T("����") || kind == _T("���") || kind == _T("����") || kind == _T("����") || kind == _T("����") || 
		kind == _T("����") || kind == _T("����") || kind == _T("����") || kind == _T("�Ǹ�") || kind == _T("���") ||
		kind == _T("�׸�") || kind == _T("�Ҽ�") || kind == _T("����") || kind == _T("ȯ��") || kind == _T("����") ||
		kind == _T("����") || kind == _T("�պ�") || kind == _T("�޵�") || kind == _T("����"))
	{
		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/%s.bmp", m_sRoot, kind);
		m_pRBmp = getBitmap(strImgPath);
	
		const	int	nJrat = atoi(m_jinfo.jrat);
		m_jinfo.hnam.Trim();
		//m_strCaseTip.Format("%s, �������ű��� : %d%%, �ſ�Ұ�", m_jinfo.hnam, nJrat);
		if (nRat == 0)
			m_strCaseTip.Format("%s, �������ű��� : %d%%, �ſ�Ұ�", m_jinfo.hnam, nJrat);
		else
			m_strCaseTip.Format("%s, �������ű��� : %d%%, �ſ����ű��� : %d%%", m_jinfo.hnam, nJrat, nRat);
     
		m_chjgub = -1;			// �� �ʱ�ȭ
	}
	else if (kind == _T("�ܱ�"))	//2012.10.31 KSJ �ܱ�������� �߰�
	{
		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/����.bmp", m_sRoot);
		m_pRBmp = getBitmap(strImgPath);
		
		m_strCaseTip = "�ܱ��������";

		m_chjgub = -1;			// �� �ʱ�ȭ
	}
	else if (kind == _T("����������"))
	{
		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/����.bmp", m_sRoot);
		m_pRBmp = getBitmap(strImgPath);
		
		m_strCaseTip = "���������� ���� 10�д��� ü��";
		
		m_chjgub = -1;			// �� �ʱ�ȭ
	}
	else
	{
		m_bBmpSlice = TRUE;
		
		//���������� ��쿡�� �̹����� �ϳ��̱� ������ ������ ó���� �ʿ����
		if (jgub.GetAt(0) != '7')
		{
			strImgPath.Format("%s/image/%d.bmp", m_sRoot, atoi((LPCTSTR)jrab));
			m_pTBmp = getBitmap(strImgPath);

			strImgPath.Format("%s/image/%d.bmp", m_sRoot, atoi((LPCTSTR)nrat));
			m_pBBmp = getBitmap(strImgPath);
		}

		switch (jgub.GetAt(0))
		{
			//case '7'://�и��� �̹��� ��ü
			case '6':
			case '1':	// �ŷ���
				strImgPath.Format("%s/image/�ŷ���.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '2':	// �ڽ���
				strImgPath.Format("%s/image/�ڽ���.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '3':	// K-OTC
				strImgPath.Format("%s/image/btn_jang.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);	
				break;
			case '4':
				strImgPath.Format("%s/image/�ŷ���.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '7':
				m_bBmpSlice = FALSE;

				strImgPath.Format("%s/image/����.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '8' :
				strImgPath.Format("%s/image/��õ����.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '9' :
				strImgPath.Format("%s/image/��õ����.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '0' :
				strImgPath.Format("%s/image/��õ����.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case 'R' :
				strImgPath.Format("%s/image/REPORT.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case 'A' :	//2013.06.11 KSJ �ڳؽ� �߰�
			case 'C' :	//2013.06.11 KSJ �ڳؽ� �߰�
			case 'D' :	//2013.06.11 KSJ �ڳؽ� �߰�
				strImgPath.Format("%s/image/�ڳؽ�.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case 'B':
				strImgPath.Format("%s/image/�ŷ���.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
		}

		m_chjgub = jgub.GetAt(0);
	}



/*	kind.Trim();

	CString	imagePath; imagePath.Format("%s/%s/", m_sRoot, "image");

	if (kind.CompareNoCase("����"))
	{
		m_bitmap = getBitmap(imagePath + kind + _T(".bmp"));
	}
	else if (jgub.GetLength() >= 1)
	{
		switch (jgub.GetAt(0))
		{
		case '1':	// �ŷ���
			m_bitmap = getBitmap(imagePath + _T("�ŷ���.bmp"));
			break;
		case '2':	// �ڽ���
			m_bitmap = getBitmap(imagePath + _T("�ڽ���.bmp"));
			break;
		case '3':	// ��������
			m_bitmap = getBitmap(imagePath + _T("��������.bmp"));
			break;
		}
	}
	else
	{
		m_bitmap = NULL;
	}
*/
}

void CControlWnd::OnVisibleChanged() 
{
	ShowWindow(m_visible);
}

LRESULT CControlWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		CRect rc;
		GetClientRect(&rc);

		if (m_pTipCtrl == nullptr)
		{
			m_pTipCtrl = std::make_unique<CToolTipCtrl>();
			m_pTipCtrl->Create(this);
		}

		if (m_bBmpSlice && !m_sData.IsEmpty())
		{
			CString strTip;
			const	int	nJrat = atoi(m_jinfo.jrat);
			const	int	nRat = atoi(m_jinfo.nrat);
			/*CString codx, jgub, jrat, ujgb, frgb, sjjs, amga, ksmm, jbkm, dyga, lock, nrate;
			codx = ""; jgub = "";  jrat = "";  ujgb = "";  frgb = "";  sjjs = "";  amga = "";  ksmm = "";  jbkm = "";  dyga = "";  lock = "";  nrate;
			CString tmp; tmp = m_jinfo.codx;
			if (tmp.GetLength() > 1 ) tmp.Delete(0);
			codx.Format("�� �� �� ��   : %s", tmp);
			switch (m_jinfo.jgub[0] )
			{
				case '1': jgub = "�� �� �� ��   : �ŷ���";
					break;
				case '2': jgub = "�� �� �� ��   : �ڽ���";
					break;
				case '3': jgub = "�� �� �� ��   : ��������";
					break;
				case '4': jgub = "�� �� �� ��   : ETF";
					break;
				default : jgub = m_jinfo.jgub;
			}
			jrat.Format("�� �� �� ��   : %d %%", atoi((LPCTSTR)m_jinfo.jrat));
			ujgb.Format("��         ��   : %s", m_jinfo.ujgb);
			frgb.Format("���κ�������: %s", (m_jinfo.frgb));

			tmp.Format("%d", atoi(m_jinfo.sjjs));
			tmp = addComma(tmp);
			sjjs.Format("���� �ֽļ�  : %s õ��", tmp);
			
			amga = m_jinfo.amga;
			amga.Remove(_T(','));
			tmp.Format("%d", atoi(amga));
			tmp = addComma(tmp);

			amga.Format("��   ��   ��   : %s ��", tmp);
			ksmm.Format("��   ��   ��   : %d ��", atoi(m_jinfo.ksmm));
			//jbkm = m_jinfo.jbkm;
			//jbkm.Remove(_T(','));
			jbkm.Format("��   ��   ��   : %s", m_jinfo.jbkm);
			dyga.Format("%d", atoi(m_jinfo.dyga));
			
			tmp.Format("%d", atoi(m_jinfo.dyga));
			tmp = addComma(tmp);
			dyga.Format("��   ��   ��   : %s ��", tmp);
			lock.Format("��   ��   ��   : %s", m_jinfo.lock);
			nrate.Format("�ſ뺸���ݷ�: %d %%", atoi((LPCTSTR)m_jinfo.nrat));*/
			CPoint	point;

			if (m_chjgub == '8' || m_chjgub == '9' || m_chjgub == '0')
			{
				CRect	rect;

				::GetCursorPos(&point);
				ScreenToClient(&point);

				rect = rc;
				rect.left = 11;
				rect.right = 27;

				if (rect.PtInRect(point))
				{
					strTip = "IBKS ��õ����";
				}
				else
				{
					if (nRat == 0)
						strTip.Format("%s, �������ű��� : %d%%, �ſ�Ұ�", m_jinfo.hnam, nJrat);
					else
						strTip.Format("%s, �������ű��� : %d%%, �ſ����ű��� : %d%%", m_jinfo.hnam, nJrat, nRat);
				}

				CString tmp;

				m_pTipCtrl->GetText(tmp,this,GetDlgCtrlID());

				if (tmp != "")
				{
					if (tmp != strTip)
					{
						m_pTipCtrl->DelTool(this);
						m_pTipCtrl->Activate(FALSE);
						m_pTipCtrl.reset();
						 
						m_pTipCtrl = std::make_unique<CToolTipCtrl>();
						m_pTipCtrl->Create(this);
					}
				}
			}
			else
			{
				if (nRat == 0)
					strTip.Format("%s, �������ű��� : %d%%, �ſ�Ұ�", m_jinfo.hnam, nJrat);
				else
					strTip.Format("%s, �������ű��� : %d%%, �ſ����ű��� : %d%%", m_jinfo.hnam, nJrat, nRat);
			}
			
			//strTip.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",codx, jgub, jrat, ujgb, frgb, sjjs, amga, ksmm, jbkm, dyga, lock, nrate);
			m_pTipCtrl->AddTool(this, (LPCTSTR)strTip, &rc, GetDlgCtrlID());
			m_pTipCtrl->SendMessage(TTM_SETMAXTIPWIDTH, 0, 1000);
			m_pTipCtrl->Activate(TRUE);
			m_pTipCtrl->RelayEvent(&msg);
		}
		else if (m_strCaseTip != "")
		{
			m_pTipCtrl->AddTool(this, (LPCTSTR)m_strCaseTip, &rc, GetDlgCtrlID());
			m_pTipCtrl->SendMessage(TTM_SETMAXTIPWIDTH, 0, 1000);
			m_pTipCtrl->Activate(TRUE);
			m_pTipCtrl->RelayEvent(&msg);
		}
		else
		{
			m_pTipCtrl->DelTool(this);
			m_pTipCtrl->Activate(FALSE);
		}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}
 
void CControlWnd::SendTR_Control(CString name, char* datb, int datl, char stat)
{
	char	szBuf[1024]{};
	struct _userTH udat{};
	int	idx = m_Param.name.GetLength();

	CopyMemory(udat.trc, (char *)name.GetString(), name.GetLength());
	udat.key = m_Param.key;
	udat.stat = stat;

	// set buffer
	CopyMemory(szBuf, (char*)m_Param.name.GetString(), idx);
	szBuf[idx++] = '\t';

	CopyMemory(&szBuf[idx], &udat, L_userTH);
	idx += L_userTH;

	CopyMemory(&szBuf[idx], datb, datl);

	if (m_pParent->GetSafeHwnd())
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf);
}

BOOL CControlWnd::SendTR_Dll(char* datb, int datl)
{
	char	szBuf[1024]{};
	struct	_userTH* info = (struct _userTH*)szBuf;;

	CopyMemory(info->trc, "pooppoop", sizeof(info->trc)); 
	info->stat = US_KEY|US_PASS|US_OOP;
	info->key = m_Param.key;		
 
	memcpy(&szBuf[L_userTH], datb, datl);				

	if (m_pParent->GetSafeHwnd())
		m_pParent->SendMessage( WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf );

	return TRUE;
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadLoanGrade();
	
	return 0;
}

CString CControlWnd::addComma(CString data)
{
	CString	tmps;
	int	pos = 0, length = 0;

	tmps = data;
	length = tmps.GetLength();
	pos = tmps.Find('.') + 1;
	if (pos > 0)
		return data;

	length -= pos; 
	if (length < 4)
		return data;

	data.Empty();
	for (int ii = 0; ii < length; ii++)
	{
		if ((length-ii) % 3 == 0 && ii != 0)
			data += ',';
		data += tmps.GetAt(ii);
	}
	return data;

}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	GetWindowRect(&rc);

	if (m_pPopup && m_pPopup->IsWindowVisible())
	{		
		const	int	cx = 225, cy = 250;

		rc.SetRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy);
		m_pPopup->MoveWindow(&rc);		
	}

	//m_pPopup->MoveWindow();
}

void CControlWnd::SetSideTime(BOOL flag) 
{
	m_bSideTime = flag;
}

void CControlWnd::OnSInfoChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnSMkMsgChanged() 
{
	// TODO: Add notification handler code

}
