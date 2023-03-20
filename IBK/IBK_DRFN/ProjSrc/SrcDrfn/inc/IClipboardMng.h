#if !defined(AFX_HANARO_ICLIPBOARDMANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_ICLIPBOARDMANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IClipboardMng.h"
#include "IBaseDefine.h"

//  int nID = 29;
// 	IClipboardManager* pMng = (IClipboardManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IClipboardManager* pMng = (IClipboardManager*)AfxGetPctrInterface(nID);

// ������  : ���ؿ� 2006.01.25(��)
// History : 2006.01.25 �������̽�
//

#define		NCB_BLANKDATA		"!!!!!"
// "!!!!!" �� ���� �÷������� ����ϰ� �����Ƿ� Ŭ������ �������� ������� ������^^


DECLARE_INTERFACE(IClipboardManager)
{
	STDMETHOD_(BOOL ,IsDeskClipboardFormatAvailable) (UINT uFormat) PURE;
	STDMETHOD_(BOOL ,GetData)(UINT uFormat, LPVOID lpszBuffer ,int nBufSize ,HWND &hWnd) PURE;
	STDMETHOD_(BOOL ,GetDataLength)(UINT uFormat, unsigned long *pnSize, HWND &hWnd) PURE;	
	STDMETHOD_(BOOL ,SetData)(UINT uFormat, LPVOID lpszBuffer, unsigned long nSize, HWND &hWnd) PURE;
	STDMETHOD_(BOOL ,Empty)(UINT uFormat, HWND &hWnd) PURE;
};

#endif //AFX_HANARO_ICLIPBOARDMANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
