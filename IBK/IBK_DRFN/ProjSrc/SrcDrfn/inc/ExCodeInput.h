#if !defined(AFX_EXCODEINPUT_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_)
#define AFX_EXCODEINPUT_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExCodeInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExCodeInput window

class CLoaderCodeInput;
class CExCodeInput
{
// Construction
public:
	CExCodeInput();

// Attributes
protected:
	CLoaderCodeInput*		m_pLoader;
	CDialog*				m_pParent;
	BOOL					m_bCreated;

	void	SetMyBaseDesk2(CBaseDesk2* pBaseDesk);

// Operations
public:

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/22
	// Return		: short �Էµ� �ڵ��� ���а�
	// Paramter
	//		LPCTSTR szCode : �ڵ��ȣ ���ڿ�
	// Comments		: �����ڵ�, �����ڵ���� ���а��� ���Ѵ�
	//-----------------------------------------------------------------------------
	short	GetGubunFromCode(LPCTSTR szCode);
	
	void	SetDataText(LPCTSTR szText);
	void	SetDataText2(LPCTSTR szCode);
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/22
	// Return		: CString
	// Paramter
	//		
	// Comments		: 
	//-----------------------------------------------------------------------------
	CString GetDataText();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: �ڵ���ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : �ڵ� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : �ڵ���Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� �ڵ���Ʈ���� �����մϴ�.
	//		UINT nProperties : �Ϲ������� ���Ǵ� �Ӽ� ���а�
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ �ڵ���ǲ���� �����ϰ�
	//				  �ڵ���ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  �ڵ���ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  �Ϲ������� ���Ǵ� �Ӽ����� �̸� ������ ���� ���а����� �����մϴ�.
	//
	//		_INCLUDE/ExCommonHeader.h
	//		const UINT CODEINPUT_PROP_STOCK		=	0;		// �ֽ� (�⺻)
	//		const UINT CODEINPUT_PROP_FO		=	1;		// ����,�ɼ�
	//		const UINT CODEINPUT_PROP_FUT		=	2;		// ����
	//		const UINT CODEINPUT_PROP_OPT		=	3;		// �ɼ�
	//		const UINT CODEINPUT_PROP_ITEM		=	4;		// ����
	//				  ������ ���� �߰��� �� �ֽ��ϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: �ڵ���ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : �ڵ� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : �ڵ���Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� �ڵ���Ʈ���� �����մϴ�.
	//		CStringList* pListProperty : �ڵ���ǲ�� �ʿ��� �Ӽ����ڿ��� ������ �ִ� ����Ʈ ������
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ �ڵ���ǲ���� �����ϰ�
	//				  �ڵ���ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  �ڵ���ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  StringList�� �ʱ� �Ӽ����� ����մϴ�. StringList�� ��� �Ӽ��� �����Ͽ��� �մϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: �ڵ���ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : �ڵ� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : �ڵ���Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� �ڵ���Ʈ���� �����մϴ�.
	//		LPCTSTR* lpszProperties : �Ӽ� ���ڿ��� �迭
	//		int nSize : �迭�� ��
	//					_countof() ��ũ�θ� �̿��Ͻʽÿ�.
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ �ڵ���ǲ���� �����ϰ�
	//				  �ڵ���ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  �ڵ���ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  ��� �Ӽ��� ���Ե� ���ڿ� �迭�� ����մϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: �ڵ���ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : �ڵ� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : �ڵ���Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� �ڵ���Ʈ���� �����մϴ�.
	//		LPCTSTR lpszProperties : ���ϴ� �Ϻκ��� �Ӽ����ڿ�
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ �ڵ���ǲ���� �����ϰ�
	//				  �ڵ���ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  �ڵ���ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  ��� �Ӽ��� �������� �ʰ� �Ϻκи� ������ �� �ֽ��ϴ�.
	//				  �⺻�Ӽ��� �ֽ��Դϴ�. �⺻�Ӽ��� �Ϻκи� ������ ������ 
	//				  LPCTSTR lpszProperties = _T("CD=1:CH=1:CT=1"); ���� �����մϴ�.
	//				  �Ӽ����� ���а��� �ݷ�':'�Դϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: �̺�Ʈ�� �޾Ҵ��� ����(�׻� TRUE)
	// Paramter
	//		LPCSTR lpszEventName : �ڵ���ǲ���κ��� ���۵� �̺�Ʈ�� �̸��Դϴ�.
	//		LPCSTR lpszParam : �̺�Ʈ�κ��� �Ѱ����� �Ķ���� �Դϴ�.
	// Comments		: �ڵ���ǲ���� �߻��� �̺�Ʈ�� �Ѱܹ޴� �Լ��Դϴ�.
	//				  virtual�� ����Ǿ������Ƿ� CExCodeInput�� ��ӹ޾� ����ϴ� class����
	//				  �ٽ� �����Ͽ� ����� �� �ֽ��ϴ�.
	//				  �̺�Ʈ�� ������ �Ķ���� ������ �Ʒ��� �����ϴ�.
	//			[id(1)] void SendTr(BSTR szTrList);
	//			[id(2)] void SelChanged(short nIndex, BSTR selectedCode);
	//			[id(3)] void LengthFull(BSTR szText);
	//			[id(4)] void Enter();
	//			[id(5)] void OnChar(BSTR bstrChar);
	//			[id(6)] void CodeNotFound(BSTR szCode);
	//			[id(DISPID_KEYUP)] void KeyUp();
	//			[id(DISPID_KEYDOWN)] void KeyDown();
	//			[id(7)] void HistoryChanged();
	//				  �̺�Ʈ�� �ڵ���ǲocx�� �߰�/�����ʿ� ���� ȭ�� dll ó���� ������ �� �ֽ��ϴ�.
	//
	//				  ȭ�� Dialog���� �̺�Ʈ�� ���������� RMSG_EVENT_CODEINPUT �޼����� �޽��ϴ�.
	//				  RMSG_EVENT_CODETINPUT�� _INCLUDE/ExCommonHeader.h�� ���ǵǾ��ֽ��ϴ�.
	//				  WPARAM�� �̺�Ʈ�� �߻��� ��Ʈ���� �ڵ��Դϴ�.
	//				  LPARAM�� �̺�Ʈ�̸��� �Ķ�����Դϴ�. LPST_CTRLEVENT ����ü �����ͷ� �Ѱ����ϴ�.
	//				  LPST_CTRLEVENT�� _INCLUDE/ExCommonHeader.h�� ���ǵǾ��ֽ��ϴ�.
	//				  ��� ��)
	//
	//			LRESULT CStdDialog::OnEventCodeInput(WPARAM wParam, LPARAM lParam)
	//			{
	//				HWND hCtrl = (HWND)wParam;
	//
	//				if(m_CodeInput.GetSafeHwnd() == hCtrl)
	//				{
	//					LPST_CTRLEVENT pEvent = (LPST_CTRLEVENT)lParam;
	//
	//					LPCSTR szEvent = pEvent->lpszEventName;
	//					LPCSTR szParam = pEvent->lpszParam;
	//				}
	//				else if(m_CodeInput2.GetSafeHwnd() == hCtrl)
	//				{
	//					LPST_CTRLEVENT pEvent = (LPST_CTRLEVENT)lParam;
	//
	//					LPCSTR szEvent = pEvent->lpszEventName;
	//					LPCSTR szParam = pEvent->lpszParam;
	//				}
	//				
	//				return 0L;
	//			}
	//-----------------------------------------------------------------------------
	virtual BOOL InvokeEvent(LPCSTR lpszEventName, LPCSTR lpszParam);

	void	SetEnabled(BOOL bEnable);
	BOOL	GetEnabled();

	long	GetBase(LPCTSTR lpszKey);

	void	SetFocus();

	void	SetCurSel(short nIndex);

	OLE_HANDLE	GetHWnd();		// OLE_HANDLE
	HWND	GetSafeHwnd();		// HWND

	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
	BOOL	ShowWindow(int nCmdShow);
	void	GetWindowRect(LPRECT lpRect);
	void	GetClientRect(LPRECT lpRect);

protected:
	class CX2000BaseDesk : public CBaseDesk2
	{
	public:
		LPDISPATCH m_pDisp;
		CX2000BaseDesk() : m_pDisp(NULL) {}
		~CX2000BaseDesk() { if(m_pDisp) m_pDisp->Release(); }

//		void Initialize();

		// * CBaseDesk Virtual ����
		virtual LPCSTR GetBasePath() ;
		virtual HWND  GetDeskHwnd() ;
		virtual LPDISPATCH GetUnknown();
		virtual LPUNKNOWN GetCtrlUnknown(LPCTSTR szName) ;
		virtual HWND GetCtrlHwnd(LPCTSTR szName);
		virtual LPVOID GetSharedInterface();
		virtual BOOL InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam);
		virtual BOOL InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam);
		virtual BOOL SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl);
		virtual LPVOID GetLSharedData();
		virtual void CallCommand(LPCSTR _szCmd);

		// CBaseDesk2���� �߰��Ǵ� �����
		virtual OLE_COLOR GetUserColor(short nIndex, LPCTSTR szName, short nFlag);
		virtual long GetDeskInfo(LPCSTR szKey);
		virtual long GetUserInfo(LPCSTR szKey);
		virtual long GetFrameInfo(LPCSTR szKey);
		virtual long GetMainInfo(LPCSTR szKey);

//* ���� ���� (2002.8.22 001) ����͸� �� �߰�
		virtual BOOL FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand);
		virtual void FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand);
		virtual LPCSTR FP_sGetMyVBName(HWND hWnd);
		virtual void FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue);
//* ���� �� (2002.8.22 001) 

		// CControlPos�� m_awndControls �ɹ������� �����͸� �����Ѵ�.
		// �� ������ �������� �� ������Ʈ�� ������ ��� �ִ�.
		virtual long FP_nGetCtrlPos();

		// ������Ʈ�� �������� ��Ų��.
		virtual void FP_vResizeCtrl();

		// {{ OCX���� DESK�� �޼��� ���� �� �ֵ��� ��û(����) 2003.05.19
		virtual void SetMapMessage(long lType, LPCSTR lpData);
		// }}
		
		friend class CExCodeInput;
		
	} m_x2000BaseDesk;
	friend class CX2000BaseDesk;

	CX2000BaseDesk*		m_pBaseDesk2;

// Implementation
public:
	virtual ~CExCodeInput();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCODEINPUT_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_)
