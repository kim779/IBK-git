#if !defined(AFX_EXSPININPUT_H__5DB22E28_4AAE_4B4E_8AEA_83A80ED6F03B__INCLUDED_)
#define AFX_EXSPININPUT_H__5DB22E28_4AAE_4B4E_8AEA_83A80ED6F03B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExSpinInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExSpinInput window

class CLoaderSpinInput;
class CExSpinInput
{
// Construction
public:
	CExSpinInput();

// Attributes
protected:
	CLoaderSpinInput*		m_pLoader;
	CDialog*				m_pParent;
	BOOL					m_bCreated;

	void	SetMyBaseDesk2(CBaseDesk2* pBaseDesk);

// Operations
public:
	BOOL	InitSpinInput(CBaseDesk2* pBaseDesk);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ������ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : ���� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : ������Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� ������Ʈ���� �����մϴ�.
	//		UINT nProperties : �Ϲ������� ���Ǵ� �Ӽ� ���а�
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ ������ǲ���� �����ϰ�
	//				  ������ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  ������ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  �Ϲ������� ���Ǵ� �Ӽ����� �̸� ������ ���� ���а����� �����մϴ�.
	//
	//		_INCLUDE/ExCommonHeader.h
	//		const UINT SPININPUT_PROP_STOCKQTY	=	0;		// �ֽļ��� (�⺻)
	//		const UINT SPININPUT_PROP_INNERPRC	=	1;		// �峻ȣ��
	//		const UINT SPININPUT_PROP_FUTPRC	=	2;		// ����ȣ��
	//		const UINT SPININPUT_PROP_OPTPRC	=	3;		// �ɼ�ȣ��
	//				  ������ ���� �߰��� �� �ֽ��ϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSpinInput(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ������ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : ���� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : ������Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� ������Ʈ���� �����մϴ�.
	//		CStringList* pListProperty : ������ǲ�� �ʿ��� �Ӽ����ڿ��� ������ �ִ� ����Ʈ ������
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ ������ǲ���� �����ϰ�
	//				  ������ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  ������ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  StringList�� �ʱ� �Ӽ����� ����մϴ�. StringList�� ��� �Ӽ��� �����Ͽ��� �մϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSpinInput(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ������ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : ���� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : ������Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� ������Ʈ���� �����մϴ�.
	//		LPCTSTR* lpszProperties : �Ӽ� ���ڿ��� �迭
	//		int nSize : �迭�� ��
	//					_countof() ��ũ�θ� �̿��Ͻʽÿ�.
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ ������ǲ���� �����ϰ�
	//				  ������ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  ������ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  ��� �Ӽ��� ���Ե� ���ڿ� �迭�� ����մϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSpinInput(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ������ǲ �ʱ�ȭ(����) ��������
	// Paramter	
	//		CDialog* pParent : ȭ��dll�� Dialog(CExOCXDLL�� Parent�� Dialog)
	//		CRect& rcControl : ���� ��Ʈ���� Rect
	//					   ResourceEditor���� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� 
	//					   pRect���� �Է��� �ʿ�����ϴ�.
	//		UINT nID : ������Ʈ�� ���ҽ� ID 
	//				   ���ҽ� �����Ϳ��� Rect���� ���ϱ� ���� ��Ʈ���� ����ߴٸ� �� ��Ʈ���� 
	//				   ID�� �Է��Ͻʽÿ�. ��Ʈ���� Rect�� ���ؼ� ������Ʈ���� �����մϴ�.
	//		LPCTSTR lpszProperties : ���ϴ� �Ϻκ��� �Ӽ����ڿ�
	// Comments		: ocx��� ��Ʈ���� �ڵ���ǲ�� �����մϴ�.
	//				  CBaseDesk2* �� �Ѱ� ����ũ�� ������ ������ǲ���� �����ϰ�
	//				  ������ǲ���κ��� �̺�Ʈ�� ���� �� �ֵ��� �մϴ�.
	//				  ������ǲ�� ����ϱ� ���ؼ��� �ݵ�� ȣ���ؾ� �ϴ� �Լ��Դϴ�.
	//				  ��� �Ӽ��� �������� �ʰ� �Ϻκи� ������ �� �ֽ��ϴ�.
	//				  �⺻�Ӽ��� �Ϻκи� ������ ������ 
	//				  LPCTSTR lpszProperties = _T("CD=1:CH=1:CT=1"); ���� �����մϴ�.
	//				  �Ӽ����� ���а��� �ݷ�':'�Դϴ�.
	//-----------------------------------------------------------------------------
	BOOL	InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSpinInput(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: �̺�Ʈ�� �޾Ҵ��� ����(�׻� TRUE)
	// Paramter
	//		LPCSTR lpszEventName : ������ǲ���κ��� ���۵� �̺�Ʈ�� �̸��Դϴ�.
	//		LPCSTR lpszParam : �̺�Ʈ�κ��� �Ѱ����� �Ķ���� �Դϴ�.
	// Comments		: ������ǲ���� �߻��� �̺�Ʈ�� �Ѱܹ޴� �Լ��Դϴ�.
	//				  virtual�� ����Ǿ������Ƿ� CExSpinInput�� ��ӹ޾� ����ϴ� class����
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
	//				  �̺�Ʈ�� ������ǲocx�� �߰�/�����ʿ� ���� ȭ�� dll ó���� ������ �� �ֽ��ϴ�.
	//
	//				  ȭ�� Dialog���� �̺�Ʈ�� ���������� RMSG_EVENT_SPININPUT �޼����� �޽��ϴ�.
	//				  RMSG_EVENT_SPININPUT�� _INCLUDE/ExCommonHeader.h�� ���ǵǾ��ֽ��ϴ�.
	//				  WPARAM�� �̺�Ʈ�� �߻��� ��Ʈ���� �ڵ��Դϴ�.
	//				  LPARAM�� �̺�Ʈ�̸��� �Ķ�����Դϴ�. LPST_CTRLEVENT ����ü �����ͷ� �Ѱ����ϴ�.
	//				  LPST_CTRLEVENT�� _INCLUDE/ExCommonHeader.h�� ���ǵǾ��ֽ��ϴ�.
	//				  ��� ��)
	//
	//			LRESULT CStdDialog::OnEventSpinInput(WPARAM wParam, LPARAM lParam)
	//			{
	//				HWND hCtrl = (HWND)wParam;
	//
	//				if(m_SpinInput.GetSafeHwnd() == hCtrl)
	//				{
	//					LPST_CTRLEVENT pEvent = (LPST_CTRLEVENT)lParam;
	//
	//					LPCSTR szEvent = pEvent->lpszEventName;
	//					LPCSTR szParam = pEvent->lpszParam;
	//				}
	//				else if(m_SpinInput2.GetSafeHwnd() == hCtrl)
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

	void	SetInitValue(long lValue);
	long	GetInitValue();

	void 	SetDataText(LPCTSTR lpszText);
	CString	GetData();

	void	SetEnabled(BOOL bEnable);
	BOOL	GetEnabled();
	
	void	SetVariation(long lValue);
	long	GetVariation();

	long	GetBase(LPCTSTR lpszKey);

	void	SetFocus();

	OLE_HANDLE	GetHWnd();		// OLE_HANDLE
	HWND	GetSafeHwnd();		// HWND
	
	void	SetRange(int nMin, int nMax);
	void	ClearEditText();

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

		friend class CExSpinInput;
		
	} m_x2000BaseDesk;
	friend class CX2000BaseDesk;

	CX2000BaseDesk*		m_pBaseDesk2;

// Implementation
public:
	virtual ~CExSpinInput();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXSPININPUT_H__5DB22E28_4AAE_4B4E_8AEA_83A80ED6F03B__INCLUDED_)
