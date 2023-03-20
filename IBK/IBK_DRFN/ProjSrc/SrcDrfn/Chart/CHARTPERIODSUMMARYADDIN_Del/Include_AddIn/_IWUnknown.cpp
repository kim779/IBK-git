// IWUnknown.cpp: implementation of the CIWUnknown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "_IWUnknown.h"

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
	if( m_ulReference <= 0)
	{
		// (2006/6/19 - Seung-Won, Bae) Cuation! Do not update after delete!
		m_ulReference = 0;
		delete this;
		// (2006/6/27 - Seung-Won, Bae) Cuation! Do not access this.
		return 0;
	}

	return m_ulReference;
}
