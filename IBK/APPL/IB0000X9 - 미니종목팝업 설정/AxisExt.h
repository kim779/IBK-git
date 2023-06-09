#pragma once

#include <vector>
#include <algorithm>
using namespace std;

#include <math.h>
#include "Common.h"
#include "../../h/axisfire.h"
#include "../../h/ledger.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxLineStyleCombo.h"
#include "../../control/fx_misc/misctype.h"

// for speed parsing
class StringProxy : public pair<LPCSTR,LPCSTR> {
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}
	string ToStr() { return string(first, second); }
	CString ToCStr() { return CString(first, second-first); }
	LPCSTR operator()() { return first; }
};

class op_notice_splitter {
public:
	bool operator() (const char ch)
	{
		return (ch=='\r')||(ch=='\n')||(ch=='\t');
	}
};

class op_account_splitter
{
public:
	bool operator() (const char ch)
	{
		return (ch=='\t')||(ch=='|');
	}
};

class CSortStringArray : public CStringArray {
public:
	void Sort();
private:
	BOOL CompareAndSwap(int pos);
};

class CfxEditEx : public CfxEdit {
public:
	CfxEditEx(CFont *pFont=NULL) : CfxEdit(pFont) {}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message==WM_NCPAINT)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.DeflateRect(CSize(-1,-1));
			CDC *pDC = GetDC();
			pDC->FillSolidRect(rc, RGB(0x7d, 0x7d, 0x7d));
			ReleaseDC(pDC);
			return 0;
		}
		return CEdit::WindowProc(message, wParam, lParam);
	}
};

class CfxStaticEx : public CfxStatic {
public:
	CfxStaticEx(CFont *pFont=NULL) : CfxStatic(pFont) {}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message==WM_NCPAINT)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.DeflateRect(CSize(-1,-1));
			CDC *pDC = GetDC();
			pDC->FillSolidRect(rc, RGB(0x7d, 0x7d, 0x7d));
			ReleaseDC(pDC);
			return 0;
		}
		return CfxStatic::WindowProc(message, wParam, lParam);
	}
};

class CAxisExt
{
public:
	CAxisExt(CWnd *pParent);
	void AxInit();
protected:
	CWnd *m_pParent;
	CFont *m_pFont, *m_pFontB;
	COLORREF m_crMinus, m_crPlus, m_crNormal, m_crBk, m_crBodyBk, m_crFocus, m_crWhite, m_crLine, m_crMadoBk, m_crMasuBk, m_crBlack, m_crCtrlBk;
	CString m_usid;
public:
	int      Variant(int cmd, int data);
	LPCTSTR  Variant(int cmd, LPCTSTR data);
	CFont*   GetAxFont(LPCTSTR name, int point, bool italic, int bold);
	CBitmap* GetAxBitmap(LPCTSTR path);
	HBITMAP  GetAxHBitmap(LPCTSTR path);
	COLORREF GetIndexColor(int index);
	CPen*    GetAxPen(COLORREF clr, int width, int style);
	CBrush*  GetAxBrush(COLORREF clr);
	BOOL     SendTR(LPCSTR name, char type, LPCSTR data, int dlen, int key);
	int      GetNextPrice(int price, CODE_TYPE type, bool isplus);
	int      GetTick(int fprc, int tprc, CODE_TYPE type);
	int      GetPrice(int price, CODE_TYPE type, int tick);
	void     SetView(LPCSTR option);
	void     SetModal(LPCSTR option);
	void	 SetPopup(LPCSTR option, BOOL bClose);
	void     SetTrigger(LPCSTR data);
	double   Str2Double(LPCSTR data, int dlen);
	double   Str2Double(const CString &data) { return Str2Double((LPCSTR)data, data.GetLength()); }
	double   Str2Double(const StringProxy &sp) { return Str2Double(sp.first, sp.second-sp.first); }
	int      Str2Int(LPCSTR data, int dlen);
	int      Str2Int(const CString &data) { return Str2Int((LPCSTR)data, data.GetLength()); }
	int      Str2Int(const StringProxy &sp) { return Str2Int(sp.first, sp.second-sp.first); }
	int      GetGjga(int curr, int diff, char udgb);
	LPCSTR   GetDiffGiho(double val);
	CString  GetTrimStr(LPCSTR data, int dlen);
	CString  Int2Str(int data, int round = 2);
	CString  Int2CommaStr(int val);
	CString  Int2CommaStr(LPCSTR val);
	CString  Double2CommaStr(double val);
	CString  Str2CommaStr( LPSTR src );
	CString  DiffStr(char udgb, int diff, int round = 2);
	COLORREF GetColor(double gjga, double curr);
	COLORREF GetColor(double val);
	struct _ledgerH* GetLedger(struct _ledgerH *ledger);
	void     ParseString(LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval);
	void     ParseString(const StringProxy &sp, CHAR del, vector<StringProxy> &retval);
	void	 ParseNotice(LPCSTR st, LPCSTR ed, vector<StringProxy> &retval);
	void	 ParseAccount(LPCSTR st, LPCSTR ed, vector<StringProxy> &retval);
public:
};

inline LPCSTR CAxisExt::GetDiffGiho(double val)
{
	static LPCSTR giho[3] = { "��", "��", " "};
	if (val>0.0) return giho[0];
	else if (val<0.0) return giho[1];
	else return giho[2];
}

inline CString CAxisExt::GetTrimStr(LPCSTR data, int dlen)
{
	CString buff(data, dlen);
	buff.TrimLeft(); buff.TrimRight();
	return (LPCSTR)buff;
}

inline double CAxisExt::Str2Double( LPCSTR data, int dlen )
{
	char buff[32];
	CopyMemory(buff, data, dlen);
	buff[dlen] = 0;
	return (buff[1]=='-') ? atof(&buff[1]) : atof(&buff[0]);
}

inline int CAxisExt::Str2Int( LPCSTR data, int dlen )
{
	char buff[32];
	CopyMemory(buff, data, dlen); 
	buff[dlen] = 0;
	return atoi(buff);
}

inline CString CAxisExt::Int2Str( int data, int round )
{
	char buff[32]{};
	if (round==0)
	{
		sprintf(buff, "%d", data);
	}
	else
	{
		const int pval = (int)(pow(10, round));
		if (data<0)
			sprintf(buff, "-%d.%0.*d", abs(data)/pval, round, abs(data)%pval);
		else
			sprintf(buff, "%d.%0.*d", data/pval, round, data%pval);
	}
	return buff;
}

inline CString CAxisExt::Str2CommaStr( LPSTR src )
{
	char dst[32]{}, * buf{}, * str{};
	int len = strlen(src);

	if (src[0]=='-')
	{
		dst[0] = '-';
		str = &src[1];
		buf = &dst[1];
		--len;
	}
	else
	{
		str = &src[0];
		buf = &dst[0];
	}
	
	switch ((len - 1) % 3) {
		case 3: // fake label to make gcc happy 
			while (*str) { 
				*buf++ = ','; 
				case 2: *buf++ = *str++; 
				case 1: *buf++ = *str++; 
				case 0: *buf++ = *str++; 
			} 
			break;
		default:
			break;
	}
	*buf = '\0'; 
	
	return dst;
}

inline CString CAxisExt::Int2CommaStr( int val )
{
	char dst[32]{}, src[32]{}, * buf{}, * str{};
	
	int len = sprintf(src, "%d", val);
	
	if (val<0)
	{
		dst[0] = '-';
		str = &src[1];
		buf = &dst[1];
		--len;
	}
	else
	{
		str = &src[0];
		buf = &dst[0];
	}
	
	switch ((len - 1) % 3) {
	case 3: /* fake label to make gcc happy */
		while (*str) { 
			*buf++ = ','; 
	case 2: *buf++ = *str++; 
	case 1: *buf++ = *str++; 
	case 0: *buf++ = *str++; 
		} 
	}
	*buf = '\0'; 
	
	return dst;
}

inline CString CAxisExt::Double2CommaStr( double val )
{
	char dst[32]{}, src[32]{}, * buf{}, * str{};
	
	int len = sprintf(src, "%.f", val);
	
	if (val<0)
	{
		dst[0] = '-';
		str = &src[1];
		buf = &dst[1];
		--len;
	}
	else
	{
		str = &src[0];
		buf = &dst[0];
	}
	
	switch ((len - 1) % 3) {
	case 3: /* fake label to make gcc happy */
		while (*str) { 
			*buf++ = ','; 
	case 2: *buf++ = *str++; 
	case 1: *buf++ = *str++; 
	case 0: *buf++ = *str++; 
		} 
	}
	*buf = '\0'; 
	
	return dst;
}


inline void CAxisExt::ParseString( LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval )
{
	retval.clear();
	LPCSTR pos{};
	while(st<ed)
	{
		pos = find(st, ed, del);
		retval.push_back( StringProxy(st, pos) );
		st = pos + 1;
	}
}

inline int CAxisExt::GetTick( int fprc, int tprc, CODE_TYPE type )
{
	int tick=0;
	if (fprc<tprc) 
	{
		for(; fprc<tprc; ++tick)
			fprc = GetNextPrice(fprc, type, true);
	}
	else if (fprc>tprc)
	{
		for(; fprc>tprc; --tick)
			fprc = GetNextPrice(fprc, type, false);
	}
	return tick;
}

inline int CAxisExt::GetPrice(int price, CODE_TYPE type, int tick)
{
	const bool isplus = (tick>0);
	tick = (isplus) ? tick : -tick;
	for(int n=0; n<tick; ++n)
		price = GetNextPrice(price, type, isplus);
	return price;
}


inline COLORREF CAxisExt::GetColor(double val)
{
	if (val>0.0) return m_crPlus;
	else if (val<0.0) return m_crMinus;
	else return m_crNormal;
}

