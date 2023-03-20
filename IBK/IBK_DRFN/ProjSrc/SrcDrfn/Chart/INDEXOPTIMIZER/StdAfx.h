// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__51E1CBAA_8DBD_4588_99AA_6169A2A5B9D5__INCLUDED_)
#define AFX_STDAFX_H__51E1CBAA_8DBD_4588_99AA_6169A2A5B9D5__INCLUDED_

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
///////�߰� ��� ���� : ���� ��� ���� ////
//Skin And Control
#include "Control/XTabCtrl.h"
//Base
//#include "../../COMMON_BASE/BaseInfo/BaseInfo.h"
// {{ ������ ���̺귯�� ��ũ ( Ȯ�� dll )  }}
//#include "../../../DataMgr/DBMgr.h"

#include "../../inc/EnvDir.h"

#include "../Common_ST/STConfigDef.h"//������� �߰�
#include "../Common_ST/StrategyLoader.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/ISDataItem.h"

#include "../../inc/ExCommonHeader.h"
//////////
#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExPlatform.h"	// �÷��� ���� �κ� ����.

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "../../inc/ExComboBox.h"
#include "../../inc/ICompanyMng.h"	// ����Ʈ�� ������ ������ ����.
#include "../Common_ST/EscDialog.h"	//Esc������ �� ȭ��ݱ� : KHD:2006.11.17

#include "../Common_ST/STSiteDef.h"	//'@', "@" ����

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__51E1CBAA_8DBD_4588_99AA_6169A2A5B9D5__INCLUDED_)
