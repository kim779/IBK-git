#if !defined(AFX_DRPLATFORM_H__44C03842_6C3B_4036_ABB3_BF9422C675BE__INCLUDED_)
#define AFX_DRPLATFORM_H__44C03842_6C3B_4036_ABB3_BF9422C675BE__INCLUDED_

// #include "Platform.h"
// **************************************************************
// 2003.02.20 Junok Lee.  mailto:alzioyes@hotmail.com
//
// �θ� �÷��� ���� �κ��� ����Ϸ���
// 01. #include "Platform.h"	// �÷��� ���� �κ� ����.
// 02. Ŭ���������� USE_DR_PLATFORM() ���Խ�Ų��.
// 03. CtlInterpace.cpp	: �÷������� �߰��Ǵ� �������̽��� ���⿡ 
// 04. CtlPackInOut.cpp : TR I/O ���� ����
// 05. CtlSharedData.cpp : Local��������
// 06. CtlGlobalSharedData.cpp : �۷ι� ��������
// ==============================================================

// {{ �÷��� �����ؼ� ó���ϴ� �κ�
#include "CtrlPackBase.h"
#include "GlobalSharedData.h"	// �۷ι�����
#include "LSharedData.h"			// ���ð���
#include "BaseDesk.h"			// CBaseDesk2����
//#include "../_COMMON/TrComm.h"		// IAuTrCommManager ��뿡 ���� �ּ�ó�� ojtaso

#define MakeValInit(xpVal, xxVal)		xpVal=xxVal;
#define	MakeNull(xpVal)	xpVal=NULL;
#define	SafeDelete(xpVal)	if(xpVal) delete xpVal; xpVal=NULL;

// StdAfx.h���� �Ʒ� 4���� �ʿ��� �κ� �߰�.
//01. #define MakeValInit(xpVal, xxVal)		xpVal=xxVal;
//02. #define	MakeNull(xpVal)	xpVal=NULL;
//03. #define	SafeDelete(xpVal)	if(xpVal) delete xpVal; xpVal=NULL;
//04. #define		IMPLCTRLCLASS			CDispUtil

#define		DGetBaseDeskHWnd()		m_pBaseDesk->GetDeskHwnd();
// }}

// BEGIN_DR_PLATFORM ����
#define		USE_DR_PLATFORM()		public:					\
	CBaseDesk2*		m_pBaseDesk;	/* CBaseDesk2�ڵ�*/		\
	CResourceManager* m_pResMng;	/* ���ҽ��Ŵ����ڵ� */	\
															\
	/* IPlatformObj �������̽� ����*/						\
	BEGIN_INTERFACE_PART(PlatformObj, IPlatformObj)								\
		STDMETHOD_(long, PFGetBase)(LPCTSTR szKey) ;							\
		STDMETHOD_(long, PFGetKey)(LPCTSTR szKey) ;								\
				/* szKey : "MainDll", "BaseDesk", "RscMng" */					\
		STDMETHOD_(void, PFSetKey)(LPCTSTR szKey, long dwKey) ;					\
		STDMETHOD_(void, PFInitialize) (long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties) ;	\
		STDMETHOD_(void, PFSetProperty) (STDATAITEM *stData, int nDataCnt);								\
		STDMETHOD_(long, PFGetState) (int nStatus, long dwData1, long dwData2) ;						\
		STDMETHOD_(long, PFSetState) (int nStatus, long dwData1, long dwData2) ;						\
		STDMETHOD_(LRESULT, PFWindowProc) (UINT message, WPARAM wParam, LPARAM lParam, int &nContinue);	\
		STDMETHOD_(LRESULT, PFPreTranslateMessage) (MSG* pMsg) ; \
	END_INTERFACE_PART(PlatformObj)			\
											\
	/* IAWndObj �������̽� ���� */			\
	BEGIN_INTERFACE_PART(WndObj, IAWndObj)	\
		STDMETHOD_(void, OnSize) (UINT nType, int cx, int cy);	\
	END_INTERFACE_PART(WndObj)									\
																\
	/* TR IN/OUT ���� ��øŬ���� ����							
	   CBaseCtrlPackIn �� �����Ѵ�.								
	   TR IN�� �����Ѵ�.										
	   �������� CtlPackInOut.cpp */								\
	class CImplPackIn  : public CBaseCtrlPackIn	\
	{											\
	public:										\
		virtual int GetData(char* buf, int bufLen);		\
	} m_xImplPackIn;									\
	friend class CImplPackIn;							\
														\
	/* CBaseCtrlPackOut �� �����Ѵ�.					
	 TR OUT�� �����Ѵ�.								
	 �������� CtlPackInOut.cpp */						\
	class CImplPackOut : public CBaseCtrlPackOut		\
	{													\
	public:												\
		CImplPackOut();									\
		virtual int GetPackDataSize(char* pIn);			\
														\
		/* ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� ����� ������ ����� ����Ÿ */	\
		virtual int SetData(char* buf, int bufLen);							\
																			\
		/* ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� *�ΰ���϶��� ����Ÿ*/				\
		virtual int SetData2(char* buf, int bufLen);						\
																			\
		/* ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� !�ΰ���϶��� ����Ÿ*/				\
		virtual int SetData3(char* buf, int bufLen);						\
																			\
		/* ����Ÿ������									
		 ������ ����� Ÿ�Ե� ����
		 nType  : 1 -> SetData ��Ÿ��
		          2 -> SetData2 ��Ÿ��
		          3 -> SetData3 ��Ÿ��
		 Ȥ � ��Ÿ���� �� �ʿ��ϰ� ���� �𸣱� ������ �� ��ƾ�� �߰���.*/	\
		virtual int SetData999(int nType, char* buf, int bufLen);	\
																	\
		char	m_aData[6];											\
		int		m_nLines, m_nBozoSize;								\
	} m_xImplPackOut;												\
	friend class CImplPackOut;										\
																	\
	/* { ���� ������ ó�� */										\
	CString			m_sLocalSharedList;		/*���ð������� ���ڿ� */	\
	/* Local(ȭ��) SharedData */										\
	ILSharedData*	m_pILSharedData;									\
	ILAdviseSink*	m_pILAdviseSink;									\
	CStringList		m_szLShareName;										\
	CString			m_szLShareData;										\
	BOOL			m_EnableGlobalShare;	/* Global�����͸� ������ �� �ִ� ���� ���θ� üũ */ \
	CList	<ST_LSHAREDFORMAT, ST_LSHAREDFORMAT> m_ListLShare;		\
																	\
	BOOL InitLSharedData();		/* Local���� �ʱ�ȭ */				\
	BOOL InitStart(LPCSTR szShareName, CString& szValue);	/* ���ð��� �ʱ�ȭ�ÿ� ó�� */ \
	BOOL InitSet();										\
	BOOL InitGet(LPCSTR szShareName);					\
	BOOL ChangedLShareData(LPCSTR szShareName, LPCSTR szValue);	\
	BOOL ParedShareDataChange(LPCSTR szSharedName, LPCSTR szData);	\
	/* } ���� ������ ó�� */							\
														\
	CString			m_sGlobalSharedList;	/*�۷ι��������� ���ڿ�*/ \
	/* Global ���������� */											\
	class CGSharedCli : CGlobalSharedClient												\
	{																					\
	public:																				\
		CGSharedCli() : m_pGSharedSvr(NULL),m_bGetGData(FALSE),m_szGSharedData("") {}	\
		CDeskSharedServer*	m_pGSharedSvr;												\
		CList	<ST_GSHAREDFORMAT, ST_GSHAREDFORMAT> m_ListShare;						\
		BOOL GetData(LPCSTR _szName, CString& szValue);									\
		LPCSTR GetData2(LPCSTR _szName);												\
		BOOL SetData(LPCSTR _szName, LPCSTR _szValue);									\
		BOOL Advise(LPCSTR _szName, CGlobalSharedClient* _pClient);						\
		BOOL UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient);					\
		void MakeList(CString szList);													\
		POSITION FindShareData(LPCSTR _szName);											\
		/*// �����Ͱ� ����Ǿ��� �� ������ �߻��ϵ��� ��û�Ѵ�.*/\
		void CallCheckSharedData(LPCSTR _szData);										\
		int  AddSharedNameToServer();													\
		int  RemoveSharedNameToServer();												\
		void IniStartGShareData();														\
		BOOL m_bGetGData;																\
		CString m_szGSharedData;														\
		/*// Virtual*/\
		virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue) ;						\
	} m_xGSharedCli;																	\
	friend class CGSharedCli;

/*//	USE_DR_PLATFORM ��*/\

// IAuTrCommManager ��뿡 ���� �ּ�ó�� ojtaso
//#define		USE_DR_TRCOMM()		public:					\
//	/* ICallbackStatus �������̽� ���� */							\
//	BEGIN_INTERFACE_PART(CallBack, ICallbackStatus)				\
//		STDMETHOD(ReceiveData) (HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);	\
//		STDMETHOD(StatusChange) (int nState);						\
//	END_INTERFACE_PART(CallBack)										\
//	long		m_lTRKey;		/* ���� ����ϴ� TRComm key */		\
//	HRESULT RecieveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen);
///*//	USE_DR_TRCOMM ��*/\
//
//	
#define		INIT_DR_PLATFORM()			m_pBaseDesk=NULL;		\
	MakeValInit(m_sGlobalSharedList , "");						\
	MakeValInit(m_sLocalSharedList , "" );						\
	MakeValInit(m_pILSharedData , NULL);						\
	MakeValInit(m_pILAdviseSink , NULL);						\
	MakeValInit(m_pResMng, NULL);



//class CGSharedCli : CGlobalSharedClient				
//{													
//public:												
//	CGSharedCli() : m_pGSharedSvr(NULL) {}			
//	CDeskSharedServer*	m_pGSharedSvr;				
//	CList	<ST_GSHAREDFORMAT, ST_GSHAREDFORMAT> m_ListShare;	
//																
//	CString		m_szShareValue;									
//																
//	BOOL		GetData(LPCSTR _szName, CString& szValue);		
//	LPCSTR		GetData2(LPCSTR _szName);						
//	BOOL		SetData(LPCSTR _szName, LPCSTR _szValue);		
//	BOOL		Advise(LPCSTR _szName, CGlobalSharedClient* _pClient);		
//	BOOL		UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient);	
//																			
//	void		MakeList(CString szList);						
//	POSITION	FindShareData(LPCSTR _szName);					
//	/* �����Ͱ� ����Ǿ��� �� ������ �߻��ϵ��� ��û�Ѵ�. */	
//	void		CallCheckSharedData(LPCSTR _szData);			
//	int			AddSharedNameToServer();						
//	int			RemoveSharedNameToServer();						
//	void		IniStartGShareData();							
//																
//	/* Virtual */												
//	virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue) ;	
//	void		CallCheckSharedData_4Acct(LPCSTR _szData);		
//}; //m_xGSharedCli;												

#endif // !defined(AFX_DRPLATFORM_H__44C03842_6C3B_4036_ABB3_BF9422C675BE__INCLUDED_)
