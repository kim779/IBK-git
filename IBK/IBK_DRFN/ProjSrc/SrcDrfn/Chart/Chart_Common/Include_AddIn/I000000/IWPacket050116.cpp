// IWPacket050116.cpp: implementation of the CIWPacket050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacket050116.h"

#include "Packetbase.h"			// for CPacket

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWPacket050116::CIWPacket050116( CPacket *p_pPacket)
{
	// (2004.10.11, ��¿�) Packet Data Object�� Pointer�� �����Ѵ�.
	m_pPacket = p_pPacket;
	if( !m_pPacket)
	{
		AfxMessageBox( "CIWPacket050116 Null Wrapping Error!");
		return;
	}
}

CIWPacket050116::~CIWPacket050116()
{

}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/11] Packet�� Type�� String���� ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWPacket050116::GetPacketTypeString( CString &p_strPacketType)
{
	p_strPacketType.Empty();
	if( !m_pPacket) return FALSE;
	p_strPacketType = m_pPacket->GetType();
	return TRUE;
}

// [04/10/26] Packet�� Type�� Unit Value(Double)�� ��ȸ�ϴ� Interface�� �����Ѵ�.
double CIWPacket050116::GetPacketTypeUnitValue( void)
{
	if( !m_pPacket) return 0;
	return m_pPacket->GetTypeFormatDouble();
}

// [04/11/30] Packet Name�� ���ϴ� Interface�� �����Ѵ�.
BOOL CIWPacket050116::GetPacketName( CString &p_strPacketName)
{
	if( !m_pPacket) return FALSE;
	p_strPacketName = m_pPacket->GetName();
	return TRUE;
}

// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
//			TRUE	-> ���� ���� ���
//			FALSE	-> ������ ����ų� ���� ���� ���� ���
BOOL CIWPacket050116::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData)
{
	if( m_pPacket) return m_pPacket->GetCurOrPrevIndexFromData( dData, nCurOrPrevIndex, dCurOrPrevIndexData);
	return FALSE;
}

// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
int CIWPacket050116::GetDataCount( void)
{
	if( m_pPacket) return m_pPacket->GetCount();
	return 0;
}

// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
double CIWPacket050116::GetData( int p_nIndex)
{
	if( m_pPacket) return m_pPacket->GetData( p_nIndex);
	return ERRORVALUE;
}

// [04/10/15] �ش� Index�� Data�� �����ϴ� Interface�� �����Ѵ�. (Zero Base)
BOOL CIWPacket050116::SetData( int p_nIndex, double p_dData)
{
	if( !m_pPacket) return FALSE;
	m_pPacket->UpdateData( p_dData, p_nIndex);
	return TRUE;
}

// [04/12/07] Double Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
CList< double, double> *CIWPacket050116::GetDataList( void)
{
	if( !m_pPacket) return NULL;
	return m_pPacket->GetnumericDataList();
}
