// ViewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB201600.h"
#include "ViewWnd.h"
#include "MainWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewWnd

CViewWnd::CViewWnd(CMainWnd* parent, CWnd* mainWnd)
{
	m_pParent = (CMainWnd*)parent;
	m_pMainWnd = mainWnd;
	m_iCols = 0;
	m_iRows = 0;
	m_iIndex = 0;
	uSiseMapID = 0;

	for(int ii = 0 ; ii<MATRIX_MAXCOL ; ii++)
	{
		for(int jj= 0 ; jj<MATRIX_MAXROW ; jj++)
		{
			m_pSiseWnd[ii][jj] = nullptr;
		}
	}

	EnableAutomation();
}

CViewWnd::~CViewWnd()
{
	for (auto& wnd : _vWnd)
		wnd->closeMap();
	_vWnd.clear();

	//for(int ii = 0 ; ii<MATRIX_MAXCOL ; ii++)
	//{
	//	for(int jj= 0 ; jj<MATRIX_MAXROW ; jj++)
	//	{
	//		if(m_pSiseWnd[ii][jj] != nullptr)
	//		{
	//			m_pSiseWnd[ii][jj]->DestroyWindow();
	//			delete m_pSiseWnd[ii][jj];
	//			m_pSiseWnd[ii][jj] = nullptr;
	//		}
	//	}
	//}
}

void CViewWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CViewWnd, CWnd)
	//{{AFX_MSG_MAP(CViewWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CViewWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CViewWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IViewWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {0436CD91-84D3-41BC-977C-DC0C7E8759B6}
static const IID IID_IViewWnd =
{ 0x436cd91, 0x84d3, 0x41bc, { 0x97, 0x7c, 0xdc, 0xc, 0x7e, 0x87, 0x59, 0xb6 } };

BEGIN_INTERFACE_MAP(CViewWnd, CWnd)
	INTERFACE_PART(CViewWnd, IID_IViewWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewWnd message handlers

int CViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	CRect clientRC; 
	GetClientRect(clientRC);

	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");

	m_font = getAxFont(_T("����ü"), 9);
	m_strPal = GetPalette();

	CString	imgPath; 
	imgPath.Format("%s/%s/", m_root, IMAGEDIR);
	
	SetOwner(m_pMainWnd);
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CViewWnd::initSettingSiseWnd(int row, int col, int hoga)
{
	m_iCols = col;
	m_iRows = row;

	for (auto& wnd : _vWnd)
		wnd->closeMap();
	_vWnd.clear();

	//for(int ii = 0 ; ii<MATRIX_MAXCOL ; ii++)
	//{
	//	for(int jj= 0 ; jj<MATRIX_MAXROW ; jj++)
	//	{
	//		if(m_pSiseWnd[ii][jj] != nullptr)
	//		{
	//			m_pSiseWnd[ii][jj]->DestroyWindow();
	//			delete m_pSiseWnd[ii][jj];
	//			m_pSiseWnd[ii][jj] = nullptr;
	//		}
	//	}
	//}
	
	makeSiseWnd(hoga);
	//loadCodeInfo(m_nBtnIndex);
}


void CViewWnd::makeSiseWnd(int hoga)
{
	CRect rcRect;
	
	int width{}, height{};
	width = 0;
	height = 0;
	uSiseMapID = 1000;
	
	rcRect.top = 0;
	rcRect.bottom = rcRect.top + MAP_HEIGHT;
	rcRect.left = 0;
	rcRect.right = rcRect.left + MAP_WIDTH;
		
	for(int ii=0 ; ii<m_iCols ; ii++)
	{
		for(int jj=0 ; jj<m_iRows ; jj++)
		{
			auto& wnd = _vWnd.emplace_back(std::make_unique<CSiseWnd>(this, m_pMainWnd, hoga, ii, jj));
			m_pSiseWnd[ii][jj] = wnd.get();
			m_pSiseWnd[ii][jj]->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rcRect, this, uSiseMapID);
			
			//Send to SiseWnd's Index
			m_pSiseWnd[ii][jj]->setIndexSiseWnd(ii, jj);
			
			//			m_iIndex++;
			uSiseMapID++;
			
			rcRect.left = rcRect.right;
			rcRect.right = rcRect.left + MAP_WIDTH;
		}
		
		rcRect.top = rcRect.top + MAP_HEIGHT;
		rcRect.bottom = rcRect.top + MAP_HEIGHT;
		rcRect.left = 0;
		rcRect.right = rcRect.left + MAP_WIDTH;
	}
}


void CViewWnd::SettingSiseWnd(int row, int col, int hoga)
{
	const int makefieldNum = 0;
	int ii{}, jj{};
	CRect rcRect;

	rcRect.top = 0;
	rcRect.bottom = rcRect.top + MAP_HEIGHT;
	rcRect.left = 0;
	rcRect.right = rcRect.left + MAP_WIDTH;

	//���� ���� �������� ���߰�
	if(row > m_iRows)
	{
		for(ii=0 ; ii<m_iCols ; ii++)
		{
			for(jj=0 ; jj<row ; jj++)
			{
				if(jj>m_iRows-1)
				{
					auto& wnd = _vWnd.emplace_back(std::make_unique<CSiseWnd>(this, m_pMainWnd, hoga, ii, jj));
					m_pSiseWnd[ii][jj] = wnd.get();
					m_pSiseWnd[ii][jj]->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rcRect, this, uSiseMapID);
					
					//Send to SiseWnd's Index
					m_pSiseWnd[ii][jj]->setIndexSiseWnd(ii, jj);
					
					m_iIndex++;
					uSiseMapID++;					
				}
				
				rcRect.left = rcRect.right;
				rcRect.right = rcRect.left + MAP_WIDTH;
			}

			rcRect.top = rcRect.top + MAP_HEIGHT;
			rcRect.bottom = rcRect.top + MAP_HEIGHT;
			rcRect.left = 0;
			rcRect.right = rcRect.left + MAP_WIDTH;
		}

		m_iRows = row;
	}

	rcRect.top = 0;
	rcRect.bottom = rcRect.top + MAP_HEIGHT;
	rcRect.left = 0;
	rcRect.right = rcRect.left + MAP_WIDTH;

	//�ű� �� �߰�
	if(col > m_iCols)
	{
		for(ii=0 ; ii<col ; ii++)
		{
			for(jj=0 ; jj<m_iRows ; jj++)
			{
				if(ii>m_iCols-1)
				{
					auto& wnd = _vWnd.emplace_back(std::make_unique<CSiseWnd>(this, m_pMainWnd, hoga, ii, jj));
					m_pSiseWnd[ii][jj] = wnd.get();
					m_pSiseWnd[ii][jj]->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rcRect, this, uSiseMapID);
					
					//Send to SiseWnd's Index
					m_pSiseWnd[ii][jj]->setIndexSiseWnd(ii, jj);
					
					m_iIndex++;
					uSiseMapID++;
				}

				rcRect.left = rcRect.right;
				rcRect.right = rcRect.left + MAP_WIDTH;
			}

			rcRect.top = rcRect.top + MAP_HEIGHT;
			rcRect.bottom = rcRect.top + MAP_HEIGHT;
			rcRect.left = 0;
			rcRect.right = rcRect.left + MAP_WIDTH;
		}

		m_iCols = col;
	}
}

void CViewWnd::GoanSimSettingSiseWnd(int row, int col, int hoga, int lastLineWndCount,CString* codes,int nCount)
{
	const int makefieldNum = 0;
	int ii{}, jj{};
	CRect rcRect;
	CString srow(_T("")), scol(_T("")), shoga(_T(""));

	rcRect.top = 0;
	rcRect.bottom = rcRect.top + MAP_HEIGHT;
	rcRect.left = 0;
	rcRect.right = rcRect.left + MAP_WIDTH;

	int n = 0;

	//���� ���� �������� ���߰�
	if(row > m_iRows)
	{
		for(ii=0 ; ii<m_iCols ; ii++)
		{
			for(jj=0 ; jj<row ; jj++)
			{
				if(jj>m_iRows-1)
				{
					auto& wnd = _vWnd.emplace_back(std::make_unique<CSiseWnd>(this, m_pMainWnd, hoga, ii, jj));
					m_pSiseWnd[ii][jj] = wnd.get();
					m_pSiseWnd[ii][jj]->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rcRect, this, uSiseMapID);
					
					//Send to SiseWnd's Index
					m_pSiseWnd[ii][jj]->setIndexSiseWnd(ii, jj);
					
					m_iIndex++;
					uSiseMapID++;					
				}
				
				if(n<nCount)
				{
					m_pSiseWnd[ii][jj]->SetCode(codes[n]);
				}
				else
				{
					m_pSiseWnd[ii][jj]->SetCode("");
				}

				n++;
				
				rcRect.left = rcRect.right;
				rcRect.right = rcRect.left + MAP_WIDTH;
			}

			rcRect.top = rcRect.top + MAP_HEIGHT;
			rcRect.bottom = rcRect.top + MAP_HEIGHT;
			rcRect.left = 0;
			rcRect.right = rcRect.left + MAP_WIDTH;
		}

		m_iRows = row;
	}
	else
	{
		for(ii=0 ; ii<m_iCols ; ii++)
		{
			for(jj=0 ; jj<row ; jj++)
			{			
				if(n<nCount)
				{
					m_pSiseWnd[ii][jj]->SetCode(codes[n]);
				}
				else
				{
					m_pSiseWnd[ii][jj]->DestroyWindow();

					m_pSiseWnd[ii][jj] = nullptr;

					auto& wnd = _vWnd.emplace_back(std::make_unique<CSiseWnd>(this, m_pMainWnd, hoga, ii, jj));
					m_pSiseWnd[ii][jj] = wnd.get();
					m_pSiseWnd[ii][jj]->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rcRect, this, uSiseMapID);
					
					//Send to SiseWnd's Index
					m_pSiseWnd[ii][jj]->setIndexSiseWnd(ii, jj);
				}

				n++;
				
				rcRect.left = rcRect.right;
				rcRect.right = rcRect.left + MAP_WIDTH;
			}

			rcRect.top = rcRect.top + MAP_HEIGHT;
			rcRect.bottom = rcRect.top + MAP_HEIGHT;
			rcRect.left = 0;
			rcRect.right = rcRect.left + MAP_WIDTH;
		}
	}

	rcRect.top = 0;
	rcRect.bottom = rcRect.top + MAP_HEIGHT;
	rcRect.left = 0;
	rcRect.right = rcRect.left + MAP_WIDTH;

	//�ű� �� �߰�
	if(col > m_iCols)
	{
		for(ii=0 ; ii<col ; ii++)
		{
			for(jj=0 ; jj<m_iRows ; jj++)
			{
				if(ii>m_iCols-1)
				{
// 					if(col-1 == ii && lastLineWndCount > 0)
// 					{
// 						m_iRows = lastLineWndCount;
// 					}

//					m_pSiseWnd[ii][jj] = new CSiseWnd(this, m_pMainWnd, hoga, ii, jj);
					auto& wnd = _vWnd.emplace_back(std::make_unique<CSiseWnd>(this, m_pMainWnd, hoga, ii, jj));
					m_pSiseWnd[ii][jj] = wnd.get();
					m_pSiseWnd[ii][jj]->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rcRect, this, uSiseMapID);
					
					//Send to SiseWnd's Index
					m_pSiseWnd[ii][jj]->setIndexSiseWnd(ii, jj);

					if(n<nCount)
					{
						m_pSiseWnd[ii][jj]->SetCode(codes[n]);
					}
					else
					{
						m_pSiseWnd[ii][jj]->SetCode("");
					}

					n++;
					
					m_iIndex++;
					uSiseMapID++;
				}

				rcRect.left = rcRect.right;
				rcRect.right = rcRect.left + MAP_WIDTH;
			}

			rcRect.top = rcRect.top + MAP_HEIGHT;
			rcRect.bottom = rcRect.top + MAP_HEIGHT;
			rcRect.left = 0;
			rcRect.right = rcRect.left + MAP_WIDTH;
		}

		m_iCols = col;
	}
}

void CViewWnd::changeSiseWnd(int row, int col, int hoga)
{
	int index = 0;

	for(int ii=0 ; ii<m_iCols ; ii++)
	{
		for(int jj=0 ; jj<m_iRows ; jj++)
		{
			m_pSiseWnd[ii][jj]->showHoga(hoga);
			m_pSiseWnd[ii][jj]->SendHogaWnd();
			index++;
		}
	}
}


void CViewWnd::deleteSiseWnd()
{
	m_iIndex = 0;	
	//saveCodeInfo(m_pParent->m_nBtnIndex,m_pParent->GetCols(), m_pParent->GetRows());

	for (auto& wnd : _vWnd)
		wnd->closeMap();
	_vWnd.clear();

	//for( int ii=0 ; ii < MATRIX_MAXCOL ; ii++ )
	//{
	//	for( int jj=0 ; jj < MATRIX_MAXROW ; jj++)
	//	{
	//		if(m_pSiseWnd[ii][jj] != nullptr)
	//		{
	//			m_pSiseWnd[ii][jj]->DestroyWindow();
	//			delete m_pSiseWnd[ii][jj];
	//			m_pSiseWnd[ii][jj] = nullptr;
	//		}
	//	}
	//}
}

CString CViewWnd::parseX(CString &srcstr, CString substr)
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

void CViewWnd::loadCodeInfo()
{
	CString strPath(_T("")), codeAry(_T("")), code(_T("")), codesStr(_T(""));
	strPath.Format("%s/%s/%s/saveCode.ini", m_root, USRDIR, m_name);
	int index=0;
	int length = 0;
	codeAry = _T("codeary");
	char codes[2048];
	
	length = GetPrivateProfileString(szMatrix, codeAry, "", codes, sizeof(codes) ,strPath);

	if(length != 0)
	{
		codesStr = (CString)codes;
		
		for(int ii=0 ; ii<m_iCols ; ii++)
		{
			for(int jj=0 ; jj<m_iRows ; jj++)
			{
				code = parseX(codesStr, ";");
			
				if(code == "")
				{
					m_pSiseWnd[ii][jj]->SetCode("");
				}
				else
				{
					m_pSiseWnd[ii][jj]->SetCode(code);	
				}
		
				index++;
			}
		}
	}
}

void CViewWnd::loadCodeInfo(int indexbtn)
{
	CString strPath(_T("")), codeAry(_T("")), code(_T("")), codesStr(_T(""));
	strPath.Format("%s/%s/%s/saveCode.ini", m_root, USRDIR, m_name);
	int index=0;
	int length = 0;
	int iType{};

	char codes[2048]{};
	
	iType = GetPrivateProfileInt(szMatrix, "TYPE", 0, strPath);
	
	if(iType == 0)
	{
		codeAry = "codeary";

		length = GetPrivateProfileString(szMatrix, codeAry, "", codes, sizeof(codes) ,strPath);
		
		codeAry.Format("codeary%02d",indexbtn);
		
		//::DeleteFile(strPath);
		codesStr = (CString)codes;
		
		WritePrivateProfileString(szMatrix, codeAry, (LPCTSTR)codesStr, (LPCTSTR)strPath);
		WritePrivateProfileString(szMatrix, "TYPE", "1", (LPCTSTR)strPath);
	}

	m_nBtnIndex = indexbtn;

	//codeAry = _T("codeary");
	codeAry.Format("codeary%02d",indexbtn);
	
	length = GetPrivateProfileString(szMatrix, codeAry, "", codes, sizeof(codes) ,strPath);
	
	if(length != 0)
	{
		codesStr = (CString)codes;
		
		for(int ii=0 ; ii<m_iCols ; ii++)
		{
			for(int jj=0 ; jj<m_iRows ; jj++)
			{
				code = parseX(codesStr, ";");
				
				if(code == "")
				{
					m_pSiseWnd[ii][jj]->SetCode("");
				}
				else
				{
					m_pSiseWnd[ii][jj]->SetCode(code);	
				}
				
				index++;
			}
		}
	}	
}


void CViewWnd::saveCodeInfo()
{
	CString strPath(_T("")), codeAry, strCode, strtemp(_T(""));
	int index = 0;
	codeAry = _T("codeary");
	strCode = _T(""); 

	strPath.Format("%s\\%s\\%s\\saveCode.ini", m_root, USRDIR, m_name);

	::DeleteFile(strPath);
	
	for(int ii=0 ; ii<m_iCols ; ii++)
	{
		for(int jj=0 ; jj<m_iRows ; jj++)
		{
			//m_iIndex
			if(m_pSiseWnd[ii][jj])
			{
				if(m_pSiseWnd[ii][jj]->m_code->GetEditData().Compare("") == 0)
				{
					strCode += "";
				}
				else
				{
					strCode += m_pSiseWnd[ii][jj]->m_code->GetEditData();
				}

				strCode += ";";
				index++;
			}
		}
	}

	WritePrivateProfileString(szMatrix, codeAry, (LPCTSTR)strCode, (LPCTSTR)strPath);
}

void CViewWnd::saveCodeInfo(int indexbtn)
{
	CString strPath(_T("")), codeAry, strCode, strtemp(_T(""));
	int index = 0;
	//codeAry = _T("codeary");

	codeAry.Format("codeary%02d",indexbtn);
	strCode = _T(""); 
	
	strPath.Format("%s\\%s\\%s\\saveCode.ini", m_root, USRDIR, m_name);
	
	//::DeleteFile(strPath);
	
	for(int ii=0 ; ii<m_iCols ; ii++)
	{
		for(int jj=0 ; jj<m_iRows ; jj++)
		{
			//m_iIndex
			if(m_pSiseWnd[ii][jj])
			{
				if(m_pSiseWnd[ii][jj]->m_code->GetEditData().Compare("") == 0)
				{
					strCode += "";
				}
				else
				{
					strCode += m_pSiseWnd[ii][jj]->m_code->GetEditData();
				}
				
				strCode += ";";
				index++;
			}
		}
	}
	
	WritePrivateProfileString(szMatrix, codeAry, (LPCTSTR)strCode, (LPCTSTR)strPath);
}

void CViewWnd::saveCodeInfo(int indexbtn,int col, int row)
{
	CString strPath(_T("")), codeAry, strCode, strtemp(_T(""));
	int index = 0;
	//codeAry = _T("codeary");
	codeAry.Format("codeary%02d",indexbtn);
	strCode = _T(""); 
	
	strPath.Format("%s\\%s\\%s\\saveCode.ini", m_root, USRDIR, m_name);
	
	::DeleteFile(strPath);
	
	for(int ii=0 ; ii<col ; ii++)
	{
		for(int jj=0 ; jj<row ; jj++)
		{
			//m_iIndex
			if(m_pSiseWnd[ii][jj])
			{
				if(m_pSiseWnd[ii][jj]->m_code->GetEditData().Compare("") == 0)
				{
					strCode += "";
				}
				else
				{
					strCode += m_pSiseWnd[ii][jj]->m_code->GetEditData();
				}
				
				strCode += ";";
				index++;
			}
		}
	}
	
	WritePrivateProfileString(szMatrix, codeAry, (LPCTSTR)strCode, (LPCTSTR)strPath);
}

void CViewWnd::SetPos()
{
}


void CViewWnd::RecvData(int kind, char* data)
{
	int col{}, row{};
	
	col = (kind)/10;	//2012.01.10 KSJ trkey->kind --> kind�� ������.
	row = (kind)%10;	//2012.01.10 KSJ trkey->kind --> kind�� ������.	
	m_pSiseWnd[col][row]->parsingSymbol(data);
}

LONG CViewWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;
	return ret;
}

COLORREF CViewWnd::GetColor(int color)
{
	if (color & 0x02000000)
		return color;
	
	return SendAxisMessage( MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

LRESULT CViewWnd::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	
	return m_pMainWnd->SendMessage(WM_USER, wParam, lPAram);
}


CString	CViewWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	CString strPath = Variant(homeCC, "");
	
	file.Format("%s\\%s\\%s", strPath, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

CString	CViewWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

CFont* CViewWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)SendAxisMessage( getFONT, (long)&fontR);	
}


void CViewWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC	backdc;
	backdc.CreateCompatibleDC(&dc); 
	CBitmap bitmap,  *pOldBitmap;
	CRect rectClient;
	GetClientRect(rectClient);
	
	bitmap.CreateCompatibleBitmap(&dc, rectClient.right, rectClient.bottom);
	pOldBitmap = (CBitmap*)backdc.SelectObject(&bitmap);
	backdc.SetBkMode(OPAQUE);
	
	backdc.FillSolidRect( rectClient, m_clrBgColor); 
	CFont* pOldFont = backdc.SelectObject(m_font);	
	
	Draw(&backdc, rectClient);	
	dc.BitBlt( rectClient.left , rectClient.top , rectClient.right , rectClient.bottom , &backdc, 0,0, SRCCOPY );
	bitmap.DeleteObject(); 
	backdc.SelectObject(pOldBitmap);
	backdc.SelectObject(pOldFont); 
	DeleteObject(pOldFont);
	DeleteObject(pOldBitmap);
	ReleaseDC(&backdc);	
	// Do not call CWnd::OnPaint() for painting messages
}

void CViewWnd::Draw(CDC *pDC, CRect rcClient)
{
	CRect rect,rectb;
	
	GetClientRect(&rect);
	
	//background
	pDC->FillSolidRect(&rect,GetColor(181));//64
	rectb = rect;
	
	//upper panel
// 	rect.bottom = offsetGH + gap2;
// 	rect.left = rect.left + gap1;
// 	rect.top = rect.top + gap1;
// 	rect.right = rect.right - gap1;
// 	
// 	pDC->FillSolidRect(&rect,GetColor(66));
//	DrawRoundBitmap(pDC, rect, ROUND_CONTENTS);
}


void CViewWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
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


void CViewWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != nullptr)
	{
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}
	DeleteObject(BitMap);
	memDC.DeleteDC();
}


void CViewWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	for (auto& wnd : _vWnd)
	{
		wnd->closeMap();
		wnd->DestroyWindow();
	}
	_vWnd.clear();	
}

void CViewWnd::chanegeTab(int index)
{
	for(int ii=0 ; ii<m_iCols ; ii++)
	{
		for(int jj=0 ; jj<m_iRows ; jj++)
		{
			m_pSiseWnd[ii][jj]->SendChangeTab(index);
		}
	}
}

void CViewWnd::CallSnapShot()
{
	m_pParent->SendMessage(WM_MANAGE,MK_SNAPSHOT,m_nBtnIndex);
}
