// ExOCXDLL.h: interface for the CExOCXDLL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXOCXDLL_H__77308CF7_9EA9_4613_A2F1_40AC279DDBF6__INCLUDED_)
#define AFX_EXOCXDLL_H__77308CF7_9EA9_4613_A2F1_40AC279DDBF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExDialog.h"
#include "ExControls.h"

#include "ExPlatform.h"	// �÷��� ���� �κ� ����.
#include "WndObj.h"
#include "Packet.h"

#include "OcxDLLDef.h"

class CExOCXDLL : public CExDialog  
{
	DECLARE_DYNCREATE(CExOCXDLL);

// Construction
public:
	CExOCXDLL();   // standard constructor
	CExOCXDLL(const UINT nID, CWnd* pParent = NULL);   // standard constructor);

#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

	USE_DR_PLATFORM();
	
	void SetPermanentHandle(CWnd* pWnd);

	CString		GetCodeName(LPCTSTR lpszCode);
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/12
	// Return		: void
	// Paramter
	//		UINT nFlag :
	//         1 : nParam1(x���� ����), nParam2(y���� ����)
	//		   2 : nParam1(x���� ���밪), nParam2(y���� ���밪)
	//		   3 : nParam1(x���� ���밪)
	//		   4 : nParam1(y���� ���밪)
	//		   5 : nParam1(x���� ���밪<��ü����>), nParam2(y���� ���밪<��ü����>)
	//		   6 : nParam1(x���� ���밪<��ü����>)
	//		   7 : nParam1(y���� ���밪<��ü����>)
	//		   8 : nParam1(x���� ���밪<���ο���>), nParam2(y���� ���밪<���ο���>)
	//		  10 : Flag�� 1�� ���� �����ϳ� ���ø����� ���� ȭ�鿡�� ȣ���� ���
	//		short nSizeX : X������
	//		short nSizeY : Y������
	// Comments		: ChildFrame�� ũ�⸦ �����Ѵ�.
	//				  ũ�Ⱚ�� -�̸� ���Ұ�
	//-----------------------------------------------------------------------------
	void	ChangeFrameSize(UINT nFlag, short nSizeX, short nSizeY);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/12
	// Return		: HWND ChildFrame�� �ڵ鰪
	// Paramter
	//		void
	// Comments		: ChildFrame�� �ڵ��� ����
	//-----------------------------------------------------------------------------
	AFX_INLINE HWND	GetFrameHandle();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: CResourceManager* ���ҽ� �Ŵ��� ������
	// Paramter
	//		void
	// Comments		: ȭ�鿡�� ���Ǵ� ���ҽ� �Ŵ����� �Ѱܹ޴´�
	//-----------------------------------------------------------------------------
	AFX_INLINE CResourceManager* GetResourceManager();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: void
	// Paramter
	//		CString strDLLParam : DtsDll.ocx���� �Ѱܹ��� �Ķ���� ��ü ���ڿ�
	// Comments		: DtsDll.ocx������ �� �����ζ� ���õ� �Ķ����(3��)�� dllȭ�� �ʱ⿡
	//				  �Ѱ��ش�. �̶� �Ķ���͸� Key=Value���·� ȭ�� ���̾�α׿��� �����ϴµ�
	//				  �̶� ���Ǵ� �Լ���.
	//				  CStdDialog�� �̺� �����Ǿ��־� �Ϲ������� ����ϴ� ���� ����
	//				  �Ķ���� ���п� ���� �б�Ǵ� ��쿡 ����� �� ����
	//-----------------------------------------------------------------------------
	void	SetParameters1(CString strDLLParam);
	void	SetParameters2(CString strDLLParam);
	void	SetParameters3(CString strDLLParam);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/17
	// Return		: Key�� �ش��ϴ� Value
	// Paramter
	//		LPCTSTR lpszKey : �Ķ���� Key
	// Comments		: dll OCX�� Dialog�� Create�ϱ����� 3���� Paramter�� �Ѱ��ش�
	//				  Parameter�� �������� ���ð����ϸ� dllȭ���� �Ѱܹ޴´�.
	//				  �� Parameter�� ���ڿ� ���̰� �������� Key=Value���¸� �����ݷ�(;)���� 
	//				  �����Ѵ�.
	//				  ���� ��� �������� ù��° Parameter ������ "AA=10;BB=-30;CC=0"�̾��ٸ�
	//				  GetParameter1(_T("BB");�� ���ϰ��� "-30"�� �Ǵ°��̴�.
	//-----------------------------------------------------------------------------
	CString		GetParameter1(LPCTSTR lpszKey);
	CString		GetParameter2(LPCTSTR lpszKey);
	CString		GetParameter3(LPCTSTR lpszKey);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/17
	// Return		: void
	// Paramter
	//		void
	// Comments		: ����� �Ķ���� ������ Create������ �ʿ��� ��찡 ����.
	//				  ���� �Ķ���͸� ����ϰ� ���̻� ������� �ʾ� �޸𸮿��� �����Ҷ�
	//				  �� �Լ��� ȣ���Ѵ�. �Ķ���ʹ� 3�������Ƿ� �׿� �ش��ϴ� �Լ��� ȣ���Ѵ�.
	//				  dllȭ���� ����ɶ��� �� �Լ��� ȣ������ �ʾƵ� �޸𸮿��� �����ȴ�.
	//-----------------------------------------------------------------------------
	void	RemoveParameter1();
	void	RemoveParameter2();
	void	RemoveParameter3();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/17
	// Return		: void
	// Paramter
	//		void
	// Comments		: ����ũ OCXũ�ⵥ�� ���̾�α� ũ�⸦ �����Ų��.
	//-----------------------------------------------------------------------------
	void	DoFullWindowSize();

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: void
	// Paramter
	//		LPCTSTR lpszMessage : �޼��������쿡 ǥ���� ���ڿ�
	// Comments		: ��ȭ�� �ϴܿ� �ִ� �޼��� �����쿡 ������ ���ڿ��� ǥ���� �� ����
	//				  �޼��� ���ڿ��� ������ �������� �����޴� �޼��� ���ڿ� �����̾�� ��
	//-----------------------------------------------------------------------------
	void	SendMessageString(LPCTSTR lpszMessage);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: void
	// Paramter
	//		void
	// Comments		: dll ȭ�鿡�� fnLoad���� ȣ��Ǵ� �Լ�
	//-----------------------------------------------------------------------------
	virtual void FormLoad();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/14
	// Return		: void
	// Paramter
	//		void
	// Comments		: ����ũ���� ��� ��Ʈ��, ��ũ��Ʈ �ε�, ������ �Ϸ�Ǿ����� ȣ���
	//-----------------------------------------------------------------------------
	virtual void FormLoadCompleted();

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2006/2/21
	// Return		: �ǿ��� ����Ʈ ����
	// Paramter
	//		HWND hFocus : �ǿ��� �̿ܿ� ��Ŀ�������϶� �ǿ����� �̵��Ǵ� �������ڵ�
	//		HWND hChild : �ǿ����� ���Ե� �������ڵ�
	//		... : �߰� �ǿ����� ���Ե� �������ڵ� (�׻� ������ ���ڿ� 0�� �Է�)
	// Comments		: CExOCXDLL���� �ǿ����� ��Ŀ���� �޴� �������ڵ��� ����
	//-----------------------------------------------------------------------------	
	int	AddTabStopHandle(HWND hFocus, HWND hChild, ...);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2006/2/21
	// Return		: void
	// Paramter
	//		void
	// Comments		: �ǿ��� ����Ʈ ����
	//				  �ǿ��� ����Ʈ�� �籸���Ҷ����� �� �Լ��� ȣ���ؼ� ����Ʈ�� ����
	//-----------------------------------------------------------------------------
	void ClearTabStopHandle();

	int ExMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	long GetOpenMapData(CString& strData);
	void SetOpenMapData(LPCTSTR lpszData);

protected:
	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/2/18
	// �Լ� �̸�          : Platform_GetBase
	// ���� Ÿ��          : long
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		LPCTSTR szKey    : 
	// �Լ� ����          : �� TR IN / OUT�� ���õ� Ŭ���� ������ ���´�.
	//						   SzKey���� ���� 
	//                         "PACKIN"  : CBaseCtrlPackIn�� ������ Ŭ������ ������
	//                         "PACKOUT" : CBaseCtrlPackOut�� ������ Ŭ������ ������
	//                      �� DLLȭ�鿡�� TRó���� ���ؼ��� ������ ����� �Ѵ�.
	///////////////////////////////////////////////////////////////////////////////
	virtual	long Platform_GetBase(LPCTSTR szKey);
	virtual long Platform_GetKey(LPCTSTR szKey);
	virtual void Platform_SetKey(LPCTSTR szKey, long dwKey);

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/2/21
	// �Լ� �̸�          : Platform_Initialize
	// ���� Ÿ��          : void
	// �Ķ����
	//		long dwMainDllObj  : 
	//		long dwBaseDesk  : 
	//		long dwRscMng  : 
	//		CStringList* pAllProperties   : 
	// �Լ� ����          : CBaseDesk2�� �����͸� �Ѱ��ش�.
	//                      ����ũ���� �ѿ��� SetAllProperties ������ �״�� �Ѱ��ش�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Platform_Initialize(long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties);

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/2/19
	// �Լ� �̸�          : Platform_SetProperty
	// ���� Ÿ��          : void
	// �Ķ����
	//		STDATAITEM *stData  : 
	//		int nDataCnt   : 
	// �Լ� ����          : DLLOCX���� � ���� �Ѱ��ְ��� �� ��
	///////////////////////////////////////////////////////////////////////////////
	virtual void Platform_SetProperty(STDATAITEM *stData, int nDataCnt);

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/2/19
	// �Լ� �̸�          : Platform_GetState
	// ���� Ÿ��          : long
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		int nStatus  : 
	//		long dwData1  : 
	//		long dwData2   : 
	// �Լ� ����          : DLLOCX�� ���� ���������� �Ѱܹ޴´�.
	///////////////////////////////////////////////////////////////////////////////
	virtual long Platform_GetState(int nStatus, long dwData1, long dwData2);
	virtual long Platform_SetState(int nStatus, long dwData1, long dwData2);

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ���ؿ�(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// ���� ��¥          : 2003/2/19
	// �Լ� �̸�          : Platform_WindowProc
	// ���� Ÿ��          : LRESULT
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		UINT message  : 
	//		WPARAM wParam  : 
	//		LPARAM lParam  : 
	//		BOOL &bContinue   : 
	// �Լ� ����          : DLL_OCX���� �߻��ϴ� �޽����� �״�� �ش�.
	//                      nContinue 0 : �޽��� ó����. ���ϵ� ������ DLL_OCX���� �����ϱ� ����.
	//                                1 : ó���� �޽��� ����
	//                                2 : �޽��� ó����. ���ϰ� �ǹ� ����. DLL_OCX���� �޽��� ���ó��.
	//	message�� RMSG_FORMINITIALCOMPLETED �ϰ�� ���� �ε尡 �� ���Ŀ� �Ҹ��� �޽���.
	//  �����ɼ����ͱ�ȸ�� ��Ʈ�ѵ鿡 ���� �����ϰ� TR�� �������� ����ũ�� ��û�Ѵ�.
	//  "DLL�δ� OCX.doc" ��������.
	///////////////////////////////////////////////////////////////////////////////
	virtual LRESULT Platform_WindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue);
	virtual LRESULT Platform_PreTranslateMessage(MSG* pMsg);
	
	virtual void Platform_OnSize(UINT nType, int cx, int cy);

	virtual int Packet_GetData(char* buf, int bufLen);

	virtual int Packet_GetPackDataSize(char* pIn);
	virtual int Packet_SetData(char* buf, int bufLen);
	virtual int Packet_SetData2(char* buf, int bufLen);
	virtual int Packet_SetData3(char* buf, int bufLen);
	virtual int Packet_SetData999(int nType, char* buf, int bufLen);

	virtual void OnGSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpGSharedData, BOOL bWithStart);
	virtual void OnLSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpLSharedData, BOOL bWithStart);
	
	void	ListupControlHandle(CWnd* pParent);
	BOOL	GotoNextWindowFocus(UINT nKey, BOOL bShift);

private:
	CMapStringToString*		m_pMapParam1;
	CMapStringToString*		m_pMapParam2;
	CMapStringToString*		m_pMapParam3;

	HWND					m_hChildFrame;
	HWND					m_hFocusChild;
	CArray<HWND, HWND>		m_arChildHandle;
	UINT					m_nChildHandlePos;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExOCXDLL)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExOCXDLL)
	virtual void OnCancel(){}
	virtual void OnOK(){}
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	LRESULT OnLShareChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

AFX_INLINE CResourceManager* CExOCXDLL::GetResourceManager() {return m_pResMng;}
AFX_INLINE HWND CExOCXDLL::GetFrameHandle()
{
	if(!m_hChildFrame)
		m_hChildFrame = (HWND)m_pBaseDesk->GetFrameInfo(_T("FRAMEHADLE"));

	return m_hChildFrame;
}

//static CBaseDesk2* m_pBaseDesk;
//static IGateManager* m_pIGateManager;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXOCXDLL_H__77308CF7_9EA9_4613_A2F1_40AC279DDBF6__INCLUDED_)
