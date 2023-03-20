// IWUnknown.cpp: implementation of the CIWUnknown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWUnknown.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWUnknown::CIWUnknown()
{
	// (2004.10.07, ��¿�) AddRef()�� Release()�� �����Ѵ�. (QueryInterface()�� NULL�� ����� ���´�.)
	m_ulReference = 0;
}

CIWUnknown::~CIWUnknown()
{

}

// (2004.10.07, ��¿�) AddRef()�� Release()�� �����Ѵ�. (QueryInterface()�� NULL�� ����� ���´�.)
ULONG CIWUnknown::AddRef( void)
{
	m_ulReference++;
	return m_ulReference;
}
ULONG CIWUnknown::Release( void)
{
	m_ulReference--;
	if( m_ulReference) return m_ulReference;
	delete this;
	return 0;
}
