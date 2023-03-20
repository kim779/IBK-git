// AxisExt.cpp : implementation file
//

#include "stdafx.h"
#include "IB301000.h"
#include "AxisExt.h"

#define NEW_HOGA 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAxisExt::CAxisExt(CWnd *pParent)
: m_pParent(pParent), m_bLogging(FALSE), m_bLogPath("")
{
	AxInit();
}

void CAxisExt::AxInit()
{
	m_crMinus = GetIndexColor(COLOR_MINUSTXT);
	m_crPlus  = GetIndexColor(COLOR_PLUSTXT);
	m_crNormal= GetIndexColor(COLOR_TEXT);
	m_crBk = GetIndexColor(COLOR_BK);
	m_crBodyBk = GetIndexColor(COLOR_BODYBACK);
	m_crFocus =  GetIndexColor(COLOR_FOCUS);
	m_crLine = GetIndexColor(COLOR_LINE);
	m_crMadoBk = RGB(0xE6, 0xE6, 0xFF);
	m_crMadoRsvdBk = RGB(242,242,252);
	m_crMasuBk = RGB(0xFF, 0xE8, 0xF3);
	m_crMasuRsvdBk = RGB(255,243,247);
	m_crBlack = RGB(0, 0, 0);
	
	m_crWhite = RGB(255, 255, 255);
	m_pFont  = GetAxFont(_T("����ü"), 9, false, 0);
	m_pFontB = GetAxFont(_T("����ü"), 9, false, FW_BOLD);
	
	m_usid = Variant(userCC, "");


	// 20221201 ȣ������ ����
	LPCSTR home = Variant(homeCC, "");
	CString file;
	file.Format("%s\\tab\\axis.ini", home);
	m_bNewHOGA = (BOOL)GetPrivateProfileInt("K2023SyStem", "Newhgunit", 0, file);
}

CAxisExt::~CAxisExt()
{
}

LPCTSTR CAxisExt::Variant( int cmd, LPCTSTR data )
{
	return (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

int CAxisExt::Variant(int cmd, int data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

CFont* CAxisExt::GetAxFont(LPCTSTR name, int point, bool italic, int bold)
{
	struct _fontR fontR;
	fontR.name   = (LPTSTR)name;
	fontR.point  = point;
	fontR.italic = italic;
	fontR.bold   = bold;
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CAxisExt::GetAxBitmap(LPCTSTR path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path);
}

HBITMAP CAxisExt::GetAxHBitmap(LPCTSTR path)
{
	CBitmap* pBmp = GetAxBitmap(path);
	return (pBmp) ? pBmp->operator HBITMAP() : NULL;
}

COLORREF CAxisExt::GetIndexColor(int index)
{
	return (COLORREF)m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CPen* CAxisExt::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CAxisExt::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

BOOL CAxisExt::SendTR(LPCSTR name, char type, LPCSTR data, int dlen, int key)
{
	if (strlen(name)!=8) return FALSE;
	
	char buff[1024*16];

	// Header setting
	struct _userTH *udat = (struct _userTH*)buff;
	CopyMemory(udat->trc, name, 8);
	udat->key  = key;
	udat->stat = type;
	
	// Data setting
	CopyMemory(&buff[L_userTH], data, dlen);
	
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, dlen), (LPARAM)buff);
}

int CAxisExt::GetPriceUnit(int iPrice, bool bPlus, bool bKospi)
{

/*
*/
#ifdef	NEW_HOGA
	if (m_bNewHOGA) {
		if (bPlus)
		{
			if (iPrice < 2000)			iPrice += 1;
			else if (iPrice < 5000)		iPrice += 5;
			else if (iPrice < 20000)	iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice < 200000)	iPrice += 100;
			else if (iPrice < 500000)	iPrice += 500;
			else if (iPrice >= 500000)	iPrice += 1000;
		}
		else
		{
			if (iPrice <= 2000)         iPrice -= 1;
			else if (iPrice <= 5000)	iPrice -= 5;
			else if (iPrice <= 20000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice <= 200000)	iPrice -= 100;
			else if (iPrice <= 500000)	iPrice -= 500;
			else if (iPrice > 500000)	iPrice -= 1000;
		}
	}
	else {
		if (bPlus)
		{
			if (bKospi)
			{
				if (iPrice < 10000)		iPrice += 10;
				else if (iPrice < 50000)	iPrice += 50;
				else if (iPrice < 100000)	iPrice += 100;
				else if (iPrice < 500000)	iPrice += 500;
				else if (iPrice >= 500000)	iPrice += 1000;
			}
			else
			{
				if (iPrice < 1000)		iPrice += 1;
				else if (iPrice < 5000)		iPrice += 5;
				else if (iPrice < 10000)	iPrice += 10;
				else if (iPrice < 50000)	iPrice += 50;
				else if (iPrice >= 50000)	iPrice += 100;
			}
		}
		else
		{
			if (bKospi)
			{
				if (iPrice <= 10000)	iPrice -= 10;
				else if (iPrice <= 50000)	iPrice -= 50;
				else if (iPrice <= 100000)	iPrice -= 100;
				else if (iPrice <= 500000)	iPrice -= 500;
				else if (iPrice > 500000)	iPrice -= 1000;
			}
			else
			{
				if (iPrice <= 1000)			iPrice -= 1;
				else if (iPrice <= 5000)	iPrice -= 5;
				else if (iPrice <= 10000)	iPrice -= 10;
				else if (iPrice <= 50000)	iPrice -= 50;
				else if (iPrice > 50000)	iPrice -= 100;
			}
	}

	}
#else 
	if (bPlus)
	{
		if (bKospi)
		{
			if (iPrice < 10000)		iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice < 100000)	iPrice += 100;
			else if (iPrice < 500000)	iPrice += 500;
			else if (iPrice >= 500000)	iPrice += 1000;
		}
		else
		{
			if (iPrice < 1000)		iPrice += 1;
			else if (iPrice < 5000)		iPrice += 5;
			else if (iPrice < 10000)	iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice >= 50000)	iPrice += 100;
		}
	}
	else
	{
		if (bKospi)
		{
			if (iPrice <= 10000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice <= 100000)	iPrice -= 100;
			else if (iPrice <= 500000)	iPrice -= 500;
			else if (iPrice > 500000)	iPrice -= 1000;
		}
		else
		{
			if (iPrice <= 1000)			iPrice -= 1;
			else if (iPrice <= 5000)	iPrice -= 5;
			else if (iPrice <= 10000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice > 50000)	iPrice -= 100;
		}
	}
#endif


	return iPrice;
}

int CAxisExt::GetETFPrice( int price, bool isplus)
{
	if (isplus)
	{
		price += 5;
	}
	else
	{
		price -= 5;
	}

	return price;
}

int CAxisExt::GetNextPrice( int price, CODE_TYPE type, bool isplus, bool bKospi )
{
	switch(type)
	{
	case CT_SPREAD:
	case CT_FUTURE:
		return GetPriceUnit(price,isplus, bKospi);
		break;
	case CT_ETF:
		return GetETFPrice(price,isplus);
		break;
	}
	MessageBox(0,  "���ݻ�������", "IBK��������", MB_OK|MB_ICONERROR);
	return 0;
}

COLORREF CAxisExt::GetColor( int gjga, int curr )
{
	if      (gjga<curr) return m_crPlus;
	else if (gjga>curr) return m_crMinus;
	else                return m_crNormal;
}

CString CAxisExt::DiffStr( char udgb, int diff, int round /*= 2*/ )
{
	int pos;
	char buff[32];

	switch(udgb)
	{
	case '1': 
		pos = sprintf(buff, "��"); break;
	case '2':
	case '+':
		pos = sprintf(buff, "��"); break;
	case '5':
	case '-':
		pos = sprintf(buff, "��"); break;
	case '4':
		pos = sprintf(buff, "��"); break;
	case '3':
	default:
		pos = sprintf(buff, " "); break;
	}

	if (round==0)
	{
		sprintf(&buff[pos], "%2d", diff);
	}
	else
	{
		int pval = (int)pow(10, round);
		sprintf(&buff[pos], "%2d.%0.*d", diff/pval, round, diff%pval);
	}
	return buff;
}

int CAxisExt::GetGjga( int curr, int diff, char udgb )
{
	if (udgb=='1' || udgb=='2' || udgb=='+') return curr-diff;
	else if (udgb=='4' || udgb=='5' || udgb=='-') return curr+diff;
	return curr;
}

struct _ledgerH* CAxisExt::GetLedger( struct _ledgerH *ledger )
{
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)ledger);

	LPCSTR usid = Variant(userCC, (LPCSTR)NULL);
	CopyMemory(ledger->usid, usid, strlen(usid));
	CopyMemory(ledger->fkey, "C", 1);
	CopyMemory(ledger->mkty, "1", 1);

	return ledger;
}

void CAxisExt::ParseString( StringProxy &sp, CHAR del, vector<StringProxy> &retval )
{
	ParseString(sp.first, sp.second, del, retval);
}

void CAxisExt::SetView( LPCSTR option )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)option);
}

void CAxisExt::SetModal( LPCSTR option )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeMODAL), (LPARAM)option);
}

void CAxisExt::SetTrigger( LPCSTR data )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)data);
}

void CAxisExt::SetLogMode( BOOL bLogging )
{
	if (m_bLogging == bLogging) return;

	m_bLogging = bLogging;

	if (m_bLogging)
	{
		LPCSTR home = Variant(homeCC, "");
		LPCSTR usid = Variant(userCC, "");
		int major = Variant(majorCC, 0);
		CTime time = CTime::GetCurrentTime();
		m_bLogPath.Format("%s\\user\\%s_%04X_%s.log", home, usid, major, time.FormatGmt("%Y%m%d"));
		MessageBox(0, m_bLogPath, "", MB_OK);
	}
}

void CAxisExt::WriteLog( LPCSTR log, ... )
{
	if (!m_bLogging) return;
	if (m_bLogPath.IsEmpty()) return;

	FILE *fp = fopen((LPCSTR)m_bLogPath, "a+");
	if (!fp) return;
	
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	char buff[80];
	int dlen = sprintf(buff, "[%02d/%02d %02d:%02d:%02d.%03d] ", 
					tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	fwrite(buff, 1, dlen, fp);
	
	va_list argptr;
	va_start(argptr, log);
	vfprintf(fp, log, argptr);
	va_end(argptr);
	fprintf(fp, "\n");
	
	fclose(fp);
}

int CAxisExt::GetLogConfig( LPCSTR key )
{
	CString iniPath;
	LPCSTR home = Variant(homeCC, "");
	LPCSTR name = Variant(nameCC, "");
	
	iniPath.Format("%s\\user\\%s\\DEBUG.INI", home, name);
	return GetPrivateProfileInt("ENABLE", key, 0, iniPath);
}
