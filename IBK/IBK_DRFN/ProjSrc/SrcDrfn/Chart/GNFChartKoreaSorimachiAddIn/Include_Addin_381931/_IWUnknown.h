// IWUnknown.h: interface for the CIWUnknown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_)
#define AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	�� AddIn�� OCX���� �̿�Ǵ� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ Version�� AddIn ���� Module�� �Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.07, ��¿�) Interface Wrapper Class�� �⺻������ IUnknown Interface�� Routine��
//		������ �� �ֵ��� �����ϴ� Base Wrapper Class�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2004.10.07, ��¿�) Interface Wrapper�� Version�� ��ġ�� 8�ڸ��� ������ �����ϵ���
//		8������ 10������ �ڵ� ��ȯ�ϴ� Macro�� �����Ѵ�.
//		(�⺻������ 050116�̸� 8������ �����Ǵµ�, �̸� 10���� ��ġ�� ��ȯ�ϴ� Macro�� �����Ѵ�.)
#define _IWRAPPER_DECIMAL( OCTAL)	( 1OCTAL - 1000000)

class CIWUnknown : public CObject  
{
public:
	CIWUnknown();
protected:
	virtual ~CIWUnknown();

// (2004.10.07, ��¿�) AddRef()�� Release()�� �����Ѵ�. (QueryInterface()�� NULL�� ����� ���´�.)
protected:
	ULONG	m_ulReference;
protected:
	virtual ULONG	STDMETHODCALLTYPE AddRef(	void);
	virtual ULONG	STDMETHODCALLTYPE Release(	void);
	virtual HRESULT	STDMETHODCALLTYPE QueryInterface( REFIID iid, void ** ppvObject)	{	return E_NOINTERFACE;	}

// (2008/2/21 - Seung-Won, Bae) Do not use ReleaseManager in Debug
protected:
	static BOOL		m_bTraceForDelete;
protected:
	virtual void	PostConstruct( void)	{}
	virtual void	PreDestructX( void)		{}
	virtual void	PreDestruct( void)		{}
};


// (2004.10.07, ��¿�) Interface Wrapper���� IUnknown�� Interface�� �ս��� ������ �� �ֵ��� Macro�� �����Ѵ�.
#define _IWRAPPER_IUNKNOWN_INTERFACE()															\
protected:																						\
	virtual ULONG	STDMETHODCALLTYPE AddRef(	void)	{	return CIWUnknown::AddRef();	}	\
	virtual ULONG	STDMETHODCALLTYPE Release(	void)	{	return CIWUnknown::Release();	}	\
	virtual HRESULT	STDMETHODCALLTYPE QueryInterface( REFIID iid, void ** ppvObject)	{	return E_NOINTERFACE;	}


//////////////////////////////////////////////////////////////////////
//	_IWRAPPER_IUNKNOWN_MANAGER_REFERENCE
#include <afxtempl.h>	// for CTypedPtrList
#define _IWRAPPER_IUNKNOWN_MANAGER_REFERENCE( m_pTarget)										\
protected:																						\
	virtual ULONG STDMETHODCALLTYPE		AddRef(	void)											\
										{	return CIWUnknown::AddRef();	}					\
	virtual ULONG STDMETHODCALLTYPE		Release( void)											\
	{																							\
		m_ulReference--;																		\
		if( m_ulReference <= 0)																	\
		{																						\
			m_ulReference = 0;																	\
			PreDestructX();																		\
			if( m_bAutoDelete) delete m_pTarget;												\
			if( m_bTraceForDelete) delete this;													\
			else m_lReleaseMgr.AddTail( this);													\
			return 0;																			\
		}																						\
		return m_ulReference;																	\
	}																							\
	virtual HRESULT STDMETHODCALLTYPE	QueryInterface( REFIID iid, void ** ppvObject)			\
										{	return E_NOINTERFACE;	}


//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_STRUCTOR
#define _IWRAPPER_IUNKNOWN_REALNAME( IWrapper)	#IWrapper
#define _IWRAPPER_IUNKNOWN_CREATE_TRACE( IWrapper)												\
	if( m_bTraceForDelete)																		\
	{																							\
		m_nObjectCount++;																		\
		TRACE( "\nThe %s is created to %d!\n",													\
			_IWRAPPER_IUNKNOWN_REALNAME( IWrapper), m_nObjectCount);							\
	}
#define _IWRAPPER_IUNKNOWN_DESTROY_TRACE( IWrapper)												\
	if( m_bTraceForDelete)																		\
	{																							\
		m_nObjectCount--;																		\
		TRACE( "\nThe %s is destroyed to %d!\n",												\
			_IWRAPPER_IUNKNOWN_REALNAME( IWrapper), m_nObjectCount);							\
	}
#define _IWRAPPER_IUNKNOWN_MANAGER_STRUCTOR( IWrapper)											\
protected:																						\
	static long		m_nObjectCount;																\
	IChartOCX *		m_pIChartOCX;																\
protected:																						\
	IWrapper()																					\
	{																							\
		m_bAutoDelete	= FALSE;																\
		m_pIChartOCX	= NULL;																	\
		_IWRAPPER_IUNKNOWN_CREATE_TRACE( IWrapper)												\
	}																							\
	~IWrapper()																					\
	{																							\
		if( m_pIChartOCX) m_pIChartOCX->Release();												\
		m_pIChartOCX = NULL;																	\
		_IWRAPPER_IUNKNOWN_DESTROY_TRACE( IWrapper)												\
	}																							\
public:																							\
	void				SetIChartOCX( IChartOCX *pIChartOCX)									\
	{																							\
		if( m_pIChartOCX) m_pIChartOCX->Release();												\
		m_pIChartOCX = pIChartOCX;																\
		if( m_pIChartOCX) m_pIChartOCX->AddRef();												\
	}																							\
protected:																						\
	virtual IChartOCX *	GetIChartOCX( void)														\
	{																							\
		if( m_pIChartOCX) m_pIChartOCX->AddRef();												\
		return m_pIChartOCX;																	\
	}																							\
protected:																						\
	virtual void		PreDestructX( void)														\
	{																							\
		PreDestruct();																			\
		SetIChartOCX( NULL);																	\
	}


//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_MGR
#define _IWRAPPER_IUNKNOWN_MANAGER_MGR( Interface, IWrapper)									\
		_IWRAPPER_IUNKNOWN_MANAGER_MGR2( Interface, IWrapper)
#define _IWRAPPER_IUNKNOWN_MANAGER_MGR2( Interface, IWrapper)									\
protected:																						\
	typedef CTypedPtrList < CPtrList, Interface *> CIWrapperManagerBase;						\
	class CIWrapperManager : public CIWrapperManagerBase										\
	{																							\
		public:																					\
			~CIWrapperManager()																	\
			{																					\
				while( !IsEmpty()) delete ( IWrapper *)RemoveHead();							\
			}																					\
	};																							\
	friend class CIWrapperManager;																\
	static CIWrapperManager m_lReleaseMgr;


//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_CREATE
#define _IWRAPPER_IUNKNOWN_MANAGER_CREATE( Interface, IWrapper, Target, m_pTarget)				\
protected:																						\
	BOOL	m_bAutoDelete;																		\
	Target *m_pTarget;																			\
	void SetTarget( Target *p_pTarget, BOOL p_bAutoDelete)										\
	{																							\
		m_pTarget = p_pTarget;																	\
		m_bAutoDelete = p_bAutoDelete;															\
	}																							\
public:																							\
	Target *GetObjectPtr( void)	{	return m_pTarget;	}										\
	static Interface *CreateObject( Target *p_pTarget, BOOL p_bAutoDelete = FALSE)				\
	{																							\
		IWrapper *pIWrapper = NULL;																\
		if( m_lReleaseMgr.IsEmpty())	pIWrapper = new IWrapper;								\
		else							pIWrapper = ( IWrapper *)m_lReleaseMgr.RemoveHead();	\
		pIWrapper->AddRef();																	\
		pIWrapper->SetTarget( p_pTarget, p_bAutoDelete);										\
		pIWrapper->PostConstruct();																\
		return pIWrapper;																		\
	}


#define _IWRAPPER_IUNKNOWN_MANAGER( Interface, IWrapper, Target, m_pTarget)						\
	_IWRAPPER_IUNKNOWN_MANAGER_REFERENCE( m_pTarget)											\
	_IWRAPPER_IUNKNOWN_MANAGER_STRUCTOR( IWrapper)												\
	_IWRAPPER_IUNKNOWN_MANAGER_MGR( Interface, IWrapper)										\
	_IWRAPPER_IUNKNOWN_MANAGER_CREATE( Interface, IWrapper, Target, m_pTarget)


#define _IWRAPPER_IUNKNOWN_MANAGER_IMP( IWrapper)												\
	IWrapper::CIWrapperManager	IWrapper::m_lReleaseMgr;										\
	long						IWrapper::m_nObjectCount = 0;

#endif // !defined(AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_)
