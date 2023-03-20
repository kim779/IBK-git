// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_)
#define AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifndef _USESTD_OJTASO

#define _USESTD_OJTASO
#include <list>
#include <map>		// �������� ������ : �������� - ojtaso (20070208)
#include <string>	// �������� ������ : �������� - ojtaso (20070208)
#include <algorithm>
using namespace std;

#endif // #ifndef _USESTD_OJTASO

#include <AfxTempl.h>

//@100121-alzoyes dsp ��ũ�ɼǿ��� StaAfx.h�� ��ƾ������.
#define __MULTI_LANG__
#define _MT_MANUAL_LOAD

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExPlatform.h"	// �÷��� ���� �κ� ����.
#include <afxdisp.h>

#include "../../inc/IMasterDataMng.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IDebugMng.h"
#include "../../inc/IAUPartnerMng.h"
#include "../../inc/IAUGwanMng.h"

#include "../../inc/MasterDataSub.h"

#include "../Common_ST/STSiteDef.h"	//'@', "@" ����

#include "LogMan.h"

extern IDebugManager*	g_pDebugMng;
extern IPartnerUtilManager* g_pPtUtilMng;
extern CLogMan g_logMan;

//@�����߰�
// Market Type (�ֽ�/����/����/�ֽļ���/�ؿ�)

//-->@inc\MasterDataSub.h �� ������ ������ ���ǵ�.
//enum { STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, STOCKFUTOPT_CHART, FOREIGN_CHART };
//enum { MARKET_UNKNOWN = -1, MARKET_KOSPI_STOCK, MARKET_KOSDAQ_STOCK, MARKET_ELW, MARKET_KOSPI_UPJONG, MARKET_KOSDAQ_UPJONG, MARKET_KFUTURE, MARKET_KOPTION, MARKET_FUTURE_SPREAD, MARKET_STOCKFUT, MARKET_STOCKOPT, MARKET_US_ITEM};
//<--@inc\MasterDataSub.h �� ������ ������ ���ǵ�.

// �⺻ / ����ڼ���
enum { MODE_DEFAULT, MODE_USER };
// ���񺯰� / ��Ʈ�߰� / �����߰� / ������ø / ����
enum { CHANGE_ITEM, NEW_CHART, NEW_ITEM, OVERLAPP_ITEM, REPLAY_ITEM };
//@�����߰�

// Period Type (ƽ/��/��/��/��/��/��)
//enum { TICK_DATA_CHART, SEC_DATA_CHART, MIN_DATA_CHART, HOUR_DATA_CHART, DAY_DATA_CHART, WEEK_DATA_CHART, MONTH_DATA_CHART };

#define nScreenID_6204		7119		//0369 :�̴�ƽ��Ʈ
#define nScreenID_6205		7120		//0370 :�̴Ͼ�����Ʈ
#define gSTOCKFUTOPT_0503	7114		//������Ʈ
#define SCREENID_FOREIGN	7116		//�ؿ�����
#define nScreenID_6301		7125		//���ֿ� 
#define nScreenID_6401		508		//0508
#define gFUTOPTJIPYO_0515	515		//0515
#define gSTOCKFUTOPT_0516	516		//0516

#define		theStdParent		((CStdDialog*)GetParent())

extern CString gszDebug;

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
int		GetCommonDataInfo(HWND hWnd, int nDataIndex, int nSubInfo);
int		SetCommonDataInfo(HWND hWnd, int nDataIndex, int nData);
CString GetTrCodeFromCode(HWND hWnd, CString strCode);
//20110502 �̹��� <<

const UINT RMSG_GET_STDDLG_PTR = ::RegisterWindowMessage("RMSG_GET_STDDLG_PTR");//20220117 �̹��� >>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_)
