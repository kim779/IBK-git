// IWPacket050531.cpp: implementation of the CIWPacket050531 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacket050531.h"

#include "Packetbase.h"			// for CPacket

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWPacket050531::CIWPacket050531( CPacket *p_pPacket)
{
	// (2004.10.11, ��¿�) Packet Data Object�� Pointer�� �����Ѵ�.
	m_pPacket = p_pPacket;
	if( !m_pPacket)
	{
		AfxMessageBox( "CIWPacket050531 Null Wrapping Error!");
		return;
	}
}

CIWPacket050531::~CIWPacket050531()
{

}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/11] Packet�� Type�� String���� ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWPacket050531::GetPacketTypeString( CString &p_strPacketType)
{
	p_strPacketType.Empty();
	if( !m_pPacket) return FALSE;
	p_strPacketType = m_pPacket->GetType();
	return TRUE;
}

// [04/10/26] Packet�� Type�� Unit Value(Double)�� ��ȸ�ϴ� Interface�� �����Ѵ�.
double CIWPacket050531::GetPacketTypeUnitValue( void)
{
	if( !m_pPacket) return 0;
	return m_pPacket->GetTypeFormatDouble();
}

// [04/11/30] Packet Name�� ���ϴ� Interface�� �����Ѵ�.
BOOL CIWPacket050531::GetPacketName( CString &p_strPacketName)
{
	if( !m_pPacket) return FALSE;
	p_strPacketName = m_pPacket->GetName();
	return TRUE;
}

// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
//			TRUE	-> ���� ���� ���
//			FALSE	-> ������ ����ų� ���� ���� ���� ���
BOOL CIWPacket050531::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData)
{
	if( m_pPacket) return m_pPacket->GetCurOrPrevIndexFromData( dData, nCurOrPrevIndex, dCurOrPrevIndexData);
	return FALSE;
}

// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
int CIWPacket050531::GetDataCount( void)
{
	if( m_pPacket) return m_pPacket->GetCount();
	return 0;
}

// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
double CIWPacket050531::GetData( int p_nIndex)
{
	if( m_pPacket) return m_pPacket->GetData( p_nIndex);
	return ERRORVALUE;
}

// [04/10/15] �ش� Index�� Data�� �����ϴ� Interface�� �����Ѵ�. (Zero Base)
BOOL CIWPacket050531::SetData( int p_nIndex, double p_dData)
{
	if( !m_pPacket) return FALSE;
	m_pPacket->UpdateData( p_dData, p_nIndex);
	return TRUE;
}

// [04/12/07] Double Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
CList< double, double> *CIWPacket050531::GetDataList( void)
{
	if( !m_pPacket) return NULL;
	return m_pPacket->GetnumericDataList();
}

// (2005/9/2 - Seung-Won, Bae) Get String Data List
CList< CString, CString> *CIWPacket050531::GetStrDataList( void)
{
	if( !m_pPacket) return NULL;
	return m_pPacket->GetstrDataList();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  AppendData
 - Created at  :  2005-07-26   19:56
 - Author      :  ������
 - Description :  Data�� �߰��ϴ� Interface�� �����Ѵ�
 -----------------------------------------------------------------------------------------*/
void CIWPacket050531::AppendData(double dData)
{
	if(!m_pPacket)
		return;

	m_pPacket->AppendData(dData);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ClearData
 - Created at  :  2005-07-29   10:04
 - Author      :  ������
 - Description :  Packet�� ����Ÿ�� Clear�Ѵ�. 
 -----------------------------------------------------------------------------------------*/
void CIWPacket050531::ClearData(const bool bBaseDataClear /* = false */)
{
	if(!m_pPacket)
		return;
	m_pPacket->ClearData(bBaseDataClear);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetType
 - Created at  :  2005-08-29   11:00
 - Author      :  ������
 - Description :  ��ŶŸ���� �����Ѵ�.
					strType : �Ӽ� page���� ������ type, -> ��ġ��ȸâ, data ��ü���⿡�� ���
					strExperssionType : �Ϲ����� type -> ȭ�� ������ ���Ҷ�, scale ǥ���Ҷ� ���
 -----------------------------------------------------------------------------------------*/
void CIWPacket050531::SetType(const CString& strType, const CString& strExperssionType)
{
	if(!m_pPacket)
		return;
	m_pPacket->SetType(strType, strExperssionType);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetType
 - Created at  :  2005-08-29   13:50
 - Author      :  ������
 - Description :  ��ŶŸ���� �����´�.
 -----------------------------------------------------------------------------------------*/
CString CIWPacket050531::GetType(const bool bIsFullType /* = true */)
{
	if(!m_pPacket)
		return _T("");
	return m_pPacket->GetType(bIsFullType);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetData
 - Created at  :  2005-09-01   10:34
 - Author      :  ������
 - Description :  �ش�Index�� ����Ÿ�� ��ȸ�ϴ� Interface�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWPacket050531::GetData(const int nIndex, double& dData)
{
	if(!m_pPacket)
		return false;

	return m_pPacket->GetData(nIndex, dData);
}
