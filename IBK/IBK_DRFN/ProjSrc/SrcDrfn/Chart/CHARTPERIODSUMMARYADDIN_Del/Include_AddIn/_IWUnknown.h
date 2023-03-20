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
};

// (2004.10.07, ��¿�) Interface Wrapper���� IUnknown�� Interface�� �ս��� ������ �� �ֵ��� Macro�� �����Ѵ�.
#define _IWRAPPER_IUNKNOWN_INTERFACE()															\
protected:																						\
	virtual ULONG	STDMETHODCALLTYPE AddRef(	void)	{	return CIWUnknown::AddRef();	}	\
	virtual ULONG	STDMETHODCALLTYPE Release(	void)	{	return CIWUnknown::Release();	}	\
	virtual HRESULT	STDMETHODCALLTYPE QueryInterface( REFIID iid, void ** ppvObject)	{	return E_NOINTERFACE;	}

// (2006/7/19 - Seung-Won, Bae) Manage Wrapper Class List
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
			if( m_bAutoDelete) delete m_pTarget;												\
			m_lReleaseMgr.AddTail( this);														\
			return 0;																			\
		}																						\
		return m_ulReference;																	\
	}																							\
	virtual HRESULT STDMETHODCALLTYPE	QueryInterface( REFIID iid, void ** ppvObject)			\
										{	return E_NOINTERFACE;	}
#define _IWRAPPER_IUNKNOWN_MANAGER_STRUCTOR( IWrapper)											\
protected:																						\
	IWrapper()	{	m_bAutoDelete = FALSE;	}													\
	~IWrapper()	{};
#define _IWRAPPER_IUNKNOWN_MANAGER_MGR( Interface, IWrapper)									\
protected:																						\
	typedef CTypedPtrList < CPtrList, Interface *> CIWrapperManagerBase;						\
	class CIWrapperManager : public CIWrapperManagerBase										\
	{																							\
		public:																					\
			~CIWrapperManager()	{	while( !IsEmpty()) delete ( IWrapper *)RemoveHead();	}	\
	};																							\
	friend class CIWrapperManager;																\
	static CIWrapperManager m_lReleaseMgr;
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
		return pIWrapper;																		\
	}

#define _IWRAPPER_IUNKNOWN_MANAGER( Interface, IWrapper, Target, m_pTarget)						\
	_IWRAPPER_IUNKNOWN_MANAGER_REFERENCE( m_pTarget)											\
	_IWRAPPER_IUNKNOWN_MANAGER_STRUCTOR( IWrapper)												\
	_IWRAPPER_IUNKNOWN_MANAGER_MGR( Interface, IWrapper)										\
	_IWRAPPER_IUNKNOWN_MANAGER_CREATE( Interface, IWrapper, Target, m_pTarget)
#define _IWRAPPER_IUNKNOWN_MANAGER_IMP( IWrapper)												\
	IWrapper::CIWrapperManager IWrapper::m_lReleaseMgr;

#endif // !defined(AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_)
