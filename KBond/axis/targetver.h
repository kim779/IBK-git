#pragma once

// SDKDDKVer.h�� �����ϸ� �ְ� ������ ���뼺�� ���� Windows �÷����� ���ǵ˴ϴ�.

// ���� Windows �÷����� ���� ���� ���α׷��� �����Ϸ��� ��쿡�� SDKDDKVer.h�� �����ϱ� ����
// WinSDKVer.h�� �����ϰ� _WIN32_WINNT ��ũ�θ� �����Ϸ��� �÷������� �����Ͻʽÿ�.

#include <SDKDDKVer.h>

#include <afxtempl.h>   // MFC Template support
#include <afxmt.h>      // MFC Multi-threading support
#include <afxole.h>     // MFC OLE support
#include <shlobj.h>     // IShellFolder support
#include <lm.h>         // WNet support

#include "htmlhelp.h"


#include "axStd.h"
#define AFX_ZERO_INIT_OBJECT(base_class) memset(((base_class*)this)+1, 0, sizeof(*this) - sizeof(class base_class));

