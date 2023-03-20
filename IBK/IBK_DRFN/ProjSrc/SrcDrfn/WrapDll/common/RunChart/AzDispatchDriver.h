#if !defined(AFX_AZDISPATCHDRIVER_H__95FF7BB7_E274_4DAA_93EF_F33B5631FB7B__INCLUDED_)
#define AFX_AZDISPATCHDRIVER_H__95FF7BB7_E274_4DAA_93EF_F33B5631FB7B__INCLUDED_
//  By Junok Lee, 2003.03.05
// --------------------------------------------------------------------
//	���� : LPSTR �Ǵ� LPCSTR, LPCTSTR ���� OLECHAR*������ ó���ϴ� ��ƾ
//		WCHAR szuText[80]; // Special treatment for ASCII client
//		LPCSTR szKey="OcxDLL0";
//		::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szKey, -1, szuText, sizeof(szuText));
//		BSTR bstrTemp = ::SysAllocString(szuText);
//		::SysFreeString(bstrTemp);
//
//	mfc�� OLEDisp2.cpp ��������
//		void COleDispatchDriver::InvokeHelperV(.....)
//
// --------------------------------------------------------------------
/*	Sample Code
/ --------------------------------------------------------------------
  ���� : �޼ҵ带 ȣ���ϴ� ��ƾ, Property������ ���Լ��� ���� �κп� ���ԵǾ�����.
BOOL YOUR_CLASS_NAME::AddToScriptEngine(LPCSTR szDLLKeyName)
{
	LPDISPATCH pDLLDispatch = m_pCrInfo->m_pDispatch;
	IDispatch* pDeskDispatch = m_pBaseDesk->GetUnknown();

	OLECHAR* szMethod = L"AddUserScriptItem";
	long result=0;
	BYTE parms[] =
		VTS_BSTR VTS_DISPATCH;

	CAzDispatchDriver aDriver(pDeskDispatch);
	aDriver.FInvokeHelper(szMethod, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szDLLKeyName, pDLLDispatch);

	return (BOOL)result;
}

void YOURCLASSNAME::AddToScriptEngine(LPDISPATCH pTargetDisp)
{
	OLECHAR* szMethod = L"AddUserScriptItem";
	long result=0;
	BYTE parms[] =
		VTS_BSTR VTS_DISPATCH;

	CAzDispatchDriver aDriver(pTargetDisp);
	DISPID dispid;
	aDriver.FGetDispid(szMethod, dispid);

	struct
	{
		char *Name;
		LPDISPATCH pDisp;
	} mYData[] = { {"Main", 0},
	{ "PrimLib", 0 } };

	for(int i=0; i<2; i++)
	{
		aDriver.FInvokeHelper(dispid, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
			mYData[i].Name, mYData[i].pDisp);
	}
//      Method�� ����ȣ���� ����� ��
//	CAzDispatchDriver aDriver(pDeskDispatch);
//	aDriver.FInvokeHelper(szMethod, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szDLLKeyName, pDLLDispatch);
}


	�Ӽ� ���� ����
  	IUnknown *pUnknown = m_pBaseDesk->GetCtrlUnknown("HtcOutput0");
	if(pUnknown)
	{
		LPCTSTR szText = "test";	
		CAzDispatchDriver azDriver(pUnknown);
		azDriver.FSetProperty(DISPID_TEXT, VT_BSTR, szText);
	}
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AzDispatchDriver.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAzDispatchDriver document
#define VT_MFCFORCEPUTREF   0x8000  // force DISPATCH_PROPERTYPUTREF

class CAzDispatchDriver
{
public:
	CAzDispatchDriver()
	{
		m_pDispatch=NULL;
	}	

	CAzDispatchDriver(IUnknown* pUnknown)
	{
		IDispatch *pDisp;
		pUnknown->QueryInterface(IID_IDispatch, (void**)&pDisp);
		pDisp->Release();

		m_pDispatch=pDisp;
		m_DispatchDriver.AttachDispatch(m_pDispatch);
	}

	CAzDispatchDriver(IDispatch* pDispatch)
	{
		m_pDispatch=pDispatch;
		m_DispatchDriver.AttachDispatch(m_pDispatch);
	}

	virtual ~CAzDispatchDriver()
	{
		if(m_pDispatch) m_DispatchDriver.DetachDispatch();
	}

// Attributes
protected:
	LPDISPATCH m_pDispatch;
	COleDispatchDriver	m_DispatchDriver;

public:

// Operations
	void SetDispatch(IDispatch* pDispatch)
	{
		m_pDispatch=pDispatch;
		m_DispatchDriver.AttachDispatch(m_pDispatch);
	}

	IDispatch* GetDispatch()
	{
		return m_pDispatch;
	}

public:

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FGetDispid
	// ���� Ÿ��          : 	HRESULT
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		OLECHAR* szKeyName  : 
	//		DISPID &dispid   : 
	// �Լ� ����          : DISPID�� ���Ѵ�.
	///////////////////////////////////////////////////////////////////////////////
	HRESULT FGetDispid(OLECHAR* szKeyName, DISPID &dispid)
	{
		if(m_pDispatch==0)	return S_FALSE;
		HRESULT hr = m_pDispatch->GetIDsOfNames(IID_NULL, &szKeyName, 1, LOCALE_USER_DEFAULT, &dispid);
		return hr;
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FInvokeHelper
	// ���� Ÿ��          : 	void
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		OLECHAR* szMethod  : 
	//		WORD wFlags  : 
	//		VARTYPE vtRet  : 
	//				void* pvRet  : 
	//		const BYTE* pbParamInfo  : 
	//		...   : 
	//		  : 
	// �Լ� ����          : szMethod �̸����� Function(Method)�� ȣ���Ѵ�.
	///////////////////////////////////////////////////////////////////////////////
	void FInvokeHelper(OLECHAR* szMethod, WORD wFlags, VARTYPE vtRet,
		void* pvRet, const BYTE* pbParamInfo, ...)
	{
		DISPID		dwDispID = 0L;
		HRESULT hr = FGetDispid(szMethod, dwDispID);
		if(SUCCEEDED(hr))
		{
			va_list argList;
			va_start(argList, pbParamInfo);
			FInvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo,
				argList);
			va_end(argList);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FInvokeHelper
	// ���� Ÿ��          : 	void
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		DISPID dwDispID  : 
	//		WORD wFlags  : 
	//		VARTYPE vtRet  : 
	//				void* pvRet  : 
	//		const BYTE* pbParamInfo  : 
	//		...   : 
	// �Լ� ����          : DISPID�� Function(Method)�� ȣ���Ѵ�.
	///////////////////////////////////////////////////////////////////////////////
	void FInvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
		void* pvRet, const BYTE* pbParamInfo, ...)
	{
		va_list argList;
		va_start(argList, pbParamInfo);
		FInvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo,
			argList);
		va_end(argList);
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FGetProperty
	// ���� Ÿ��          : 	void
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		OLECHAR* szProperty  : 
	//		VARTYPE vtProp  : 
	//		void* pvProp   : 
	// �Լ� ����          : szProperty �̸����� �Ӽ����� ����´�.
	//	Sample
	//OLECHAR* wszFnName = L"SAppRegKey";
	//CString result;
	//FGetProperty(wszFnName, VT_BSTR, (void*)&result);
	///////////////////////////////////////////////////////////////////////////////
	void FGetProperty(OLECHAR* szProperty, VARTYPE vtProp, void* pvProp)
	{
		DISPID		dwDispID = 0L;
		HRESULT hr = FGetDispid(szProperty, dwDispID);

		FGetProperty(dwDispID, vtProp, pvProp);
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FGetProperty
	// ���� Ÿ��          : 	void
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		DISPID dwDispID  : 
	//			VARTYPE vtProp  : 
	//		void* pvProp   : 
	// �Լ� ����          : DISPID�� �Ӽ����� ����´�.
	///////////////////////////////////////////////////////////////////////////////
	void FGetProperty(DISPID dwDispID,	VARTYPE vtProp, void* pvProp)
	{
		FInvokeHelper(dwDispID, DISPATCH_PROPERTYGET,vtProp, pvProp, NULL);
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FSetProperty
	// ���� Ÿ��          : 	void
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		OLECHAR* szProperty  : 
	//		VARTYPE vtProp  : 
	//		...   : 
	// �Լ� ����          : szProperty �̸����� �Ӽ����� �����Ѵ�.
	//	Sample
	//LPCSTR propVal = "DeskPath";
	//CString result;
	//FSetProperty(wszFnName, VT_BSTR, propVal);
	///////////////////////////////////////////////////////////////////////////////
	void FSetProperty(OLECHAR* szProperty, VARTYPE vtProp, ...)
	{
		DISPID		dwDispID = 0L;
		HRESULT hr = FGetDispid(szProperty, dwDispID);

		va_list argList;    // really only one arg, but...
		va_start(argList, vtProp);
		FSetPropertyV(dwDispID, vtProp, argList);
		va_end(argList);
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FSetProperty
	// ���� Ÿ��          : 	void
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		DISPID dwDispID  : 
	//		VARTYPE vtProp  : 
	//		...   : 
	// �Լ� ����          : DISPID�� �Ӽ����� �����Ѵ�.
	///////////////////////////////////////////////////////////////////////////////
	void FSetProperty(DISPID dwDispID, VARTYPE vtProp, ...)
	{
		va_list argList;    // really only one arg, but...
		va_start(argList, vtProp);
		FSetPropertyV(dwDispID, vtProp, argList);
		va_end(argList);
	}

public:
	// �Ʒ� �κ��� ���������� �ݵǴ� �Լ� �̹Ƿ� ����ȣ�� �� �ʿ䰡 ���� ��쿡�� ����Ѵ�.
	// void FSetPropertyV(DISPID dwDispID, VARTYPE vtProp, va_list argList)
	// void FInvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, va_list argList);

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FSetPropertyV
	// ���� Ÿ��          : void
	// �Ķ����
	//		DISPID dwDispID  : 
	//		VARTYPE vtProp  : 
	//		va_list argList   : 
	// �Լ� ����          : �Ӽ����� �����ϴ� ��ƾ
	///////////////////////////////////////////////////////////////////////////////
	void FSetPropertyV(DISPID dwDispID, VARTYPE vtProp, va_list argList)
	{
		BYTE rgbParams[2];
		if (vtProp & VT_BYREF)
		{
			vtProp &= ~VT_BYREF;
			vtProp |= VT_MFCBYREF;
		}
		
		#if !defined(_UNICODE) && !defined(OLE2ANSI)
				if (vtProp == VT_BSTR)
					vtProp = VT_BSTRA;
		#endif

		WORD wFlags;
		if (vtProp & VT_MFCFORCEPUTREF)
		{
			wFlags = DISPATCH_PROPERTYPUTREF;
			vtProp &= ~VT_MFCFORCEPUTREF;
		}
		else
		{
			if (vtProp == VT_DISPATCH)
				wFlags = DISPATCH_PROPERTYPUTREF;
			else
				wFlags = DISPATCH_PROPERTYPUT;
		}
		
		rgbParams[0] = (BYTE)vtProp;
		rgbParams[1] = 0;
		FInvokeHelperV(dwDispID, wFlags, VT_EMPTY, NULL, rgbParams, argList);
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/3/7
	// �Լ� �̸�          : FInvokeHelperV
	// ���� Ÿ��          : 	void
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		DISPID dwDispID  : 
	//		WORD wFlags  : 
	//				VARTYPE vtRet  : 
	//		void* pvRet  : 
	//		const BYTE* pbParamInfo  : 
	//		va_list argList   : 
	// �Լ� ����          : �������� IDispatch�� Invoke�� ȣ���ϴ� ��ƾ
	///////////////////////////////////////////////////////////////////////////////
	void FInvokeHelperV(DISPID dwDispID, WORD wFlags,
		VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, va_list argList)
	{
		if(m_pDispatch==0) return;

		if(!m_pDispatch)	m_DispatchDriver.AttachDispatch(m_pDispatch);
		
		m_DispatchDriver.InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, argList);
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AZDISPATCHDRIVER_H__95FF7BB7_E274_4DAA_93EF_F33B5631FB7B__INCLUDED_)
