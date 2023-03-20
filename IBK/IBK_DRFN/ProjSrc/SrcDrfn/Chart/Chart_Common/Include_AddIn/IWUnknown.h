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
	virtual ULONG	STDMETHODCALLTYPE AddRef(	void)	{	return CIWUnknown::AddRef();	}	\
	virtual ULONG	STDMETHODCALLTYPE Release(	void)	{	return CIWUnknown::Release();	}	\
	virtual HRESULT	STDMETHODCALLTYPE QueryInterface( REFIID iid, void ** ppvObject)	{	return E_NOINTERFACE;	}

#endif // !defined(AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_)
