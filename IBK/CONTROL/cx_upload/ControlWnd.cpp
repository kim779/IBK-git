// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_upload.h"
#include "ControlWnd.h"
#include "base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pWizard = nullptr;
	m_size = 0;
	m_offset = 0;
}

CControlWnd::~CControlWnd()
{
	m_file.Close();
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
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_EX(CControlWnd, "filepath", GetFilepath, SetFilepath, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "upload", upload, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetFilePath", GetFilePath, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {6B1FEEEC-AC5F-423B-B624-B68ADCFECFBF}
static const IID IID_IControlWnd =
{ 0x6b1feeec, 0xac5f, 0x423b, { 0xb6, 0x24, 0xb6, 0x8a, 0xdc, 0xfe, 0xcf, 0xbf } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

short CControlWnd::upload(LPCTSTR sPath)
{
	CString spath, tmp;

	spath = CString(sPath);
	spath.Trim();
	if (spath.CompareNoCase(m_filename) || spath.IsEmpty() || m_strFileName.IsEmpty())
	{
		AfxMessageBox("파일 선택해주세요. ");
		m_filename.Empty();
		m_strFileName.Empty();
		return 0;
	}
	spath = m_strFileName;

	m_file.Close();
	if (!m_file.Open(spath, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		m_strFileName.Empty();
		m_filename.Empty();
		return 0;
	}

	m_offset = 0;
	m_size = (int)m_file.GetLength();
	if (m_size == 0)
	{
		m_file.Close();
		return 0;
	}

	return sendData();
}

int CControlWnd::sendData()
{
	if (m_offset < 0)
		return -1;

	std::unique_ptr<char[]> wb = std::make_unique<char[]>(DF_FILESIZE);
	char*	base64encode = nullptr;
	char	flow = (m_offset == 0) ? DF_FIRST : DF_MIDDLE;
	const	int	datl = (m_offset + DF_FILESIZE > m_size) ? (m_size - m_offset) : DF_FILESIZE;

	if (datl > 0)
	{
		m_file.Seek(m_offset, CFile::begin);
		m_file.Read(wb.get(), datl);

#ifdef DF_BASE64
		datl = Base64Incode((unsigned char*)wb.get(), (unsigned char*&)base64encode, datl);
#endif
	}
	else
		flow = DF_LAST;


	CString tmps;
	std::unique_ptr<char[]> sndb = std::make_unique<char[]>(L_MID);
	st_mid_PMHOUPLD* pmid = (st_mid_PMHOUPLD*)sndb.get();

	pmid->flow = flow;
	CopyMemory(pmid->name, m_filename, m_filename.GetLength());
	tmps.Format("%5d", datl);
	CopyMemory(pmid->dlen, tmps, tmps.GetLength());
#ifdef DF_BASE64
	if (base64encode)
	{
		CopyMemory(pmid->data, base64encode, datl);
		free(base64encode);
	}
#else
	CopyMemory(pmid->data, wb.get(), datl);
#endif
	wb.reset();

	const	int	ret = sendTR("PMHOUPLD", US_KEY, sndb.get(), L_xMID + datl);
	sndb.reset();

	if (flow == DF_LAST)
		m_offset = -1;
	else
		m_offset += datl;

	return ret;
}

int CControlWnd::sendTR(CString strName, BYTE type, char* datb, int datl)
{
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datl + L_userTH + 128);
	struct	_userTH	udat {};
	int	idx = 0;

	// key & control name
	pWb[idx++] = 1;
	CopyMemory(&pWb[idx], m_param.name, m_param.name.GetLength());
	idx += m_param.name.GetLength();
	pWb[idx++] = '\t';

	// udat
	CopyMemory(udat.trc, strName, min(sizeof(udat.trc), strName.GetLength()));
	udat.stat = type;
	udat.key = m_param.key;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;
	
	// data
	CopyMemory(&pWb[idx], datb, datl);

	const	int	ret = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)pWb.get());
	pWb.reset();

	return ret;
}

void CControlWnd::SetParam(_param* pParam)
{
	m_param.key   = pParam->key;
	m_param.name  = CString(pParam->name, pParam->name.GetLength());
	m_param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString slog;
	CString sdata;

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			struct	_extTHx* exth = (struct _extTHx*)lParam;

			if (exth->data[0] == '1')
			{
				if (sendData() < 0)
				{
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange/*Change*/)),
							(LPARAM)m_param.name.operator LPCTSTR());
				}
			}
		}
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!
		break;
	default:break;
	}
	return 0;
}

BSTR CControlWnd::GetFilepath()
{
	CString strResult;

	strResult = m_strFileName;
	return strResult.AllocSysString();
}

void CControlWnd::SetFilepath(LPCTSTR lpszNewValue)
{
	// TODO: Add your property handler here
}

BSTR CControlWnd::GetFilePath()
{
	CString strResult;
	CString file, spath;

	m_strFileName.Empty();
	m_filename.Empty();

	file = _T("All files(*.*)|*.*|");
	CFileDialog dlg(TRUE, "", NULL, OFN_OVERWRITEPROMPT, file, this);
	if (dlg.DoModal() == IDOK)
	{
		spath = dlg.GetPathName();
		const	int	ifind = spath.ReverseFind('\\');
		if (ifind < 0)
			return m_strFileName.AllocSysString();

		m_strFileName = spath;
		m_filename = spath.Mid(ifind+1);
		m_filename.Trim();
	}

	return m_filename.AllocSysString();
}
