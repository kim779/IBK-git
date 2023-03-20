// IWPacket.cpp: implementation of the CIWPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWPacket)

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/26] Packet�� Type�� Unit Value(Double)�� ��ȸ�ϴ� Interface�� �����Ѵ�.
double CIWPacket::GetPacketTypeUnitValue( void)
{
	if( !m_pPacket) return 0;
	return m_pPacket->GetTypeFormatDouble();
}

// [04/11/30] Packet Name�� ���ϴ� Interface�� �����Ѵ�.
BOOL CIWPacket::GetPacketName( CString &p_strPacketName)
{
	if( !m_pPacket) return FALSE;
	p_strPacketName = m_pPacket->GetName();
	return TRUE;
}

// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
//			TRUE	-> ���� ���� ���
//			FALSE	-> ������ ����ų� ���� ���� ���� ���
BOOL CIWPacket::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData)
{
	if( m_pPacket) return m_pPacket->GetCurOrPrevIndexFromData( dData, nCurOrPrevIndex, dCurOrPrevIndexData);
	return FALSE;
}

// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
int CIWPacket::GetDataCount( void)
{
	if( m_pPacket) return m_pPacket->GetCount();
	return 0;
}

// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
double CIWPacket::GetData( int p_nIndex)
{
	if( m_pPacket) return m_pPacket->GetData( p_nIndex);
	return ERRORVALUE;
}
double CIWPacket::GetDataPastandFuture( int p_nIndex)
{
	if( m_pPacket) return m_pPacket->GetDataPastandFuture( p_nIndex);
	return ERRORVALUE;
}
BOOL CIWPacket::GetData(const int nIndex, CString &p_strData)
{
	if( !m_pPacket) return FALSE;
	p_strData = m_pPacket->GetStrData( nIndex);
	return TRUE;
}

// [04/10/15] �ش� Index�� Data�� �����ϴ� Interface�� �����Ѵ�. (Zero Base)
BOOL CIWPacket::SetData( int p_nIndex, double p_dData)
{
	if( !m_pPacket) return FALSE;
	m_pPacket->UpdateData( p_dData, p_nIndex);
	return TRUE;
}

// [04/12/07] Double Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
CList< double, double> *CIWPacket::GetDataList( void)
{
	if( !m_pPacket) return NULL;
	return m_pPacket->GetnumericDataList();
}

// (2005/9/2 - Seung-Won, Bae) Get String Data List
CList< CString, CString> *CIWPacket::GetStrDataList( void)
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
void CIWPacket::AppendData(double dData)
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
void CIWPacket::ClearData(const bool bBaseDataClear /* = false */)
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
void CIWPacket::SetType(const CString& strType, const CString& strExperssionType)
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
CString CIWPacket::GetType(const bool bIsFullType /* = true */)
{
	if(!m_pPacket)
		return _T("");
	return m_pPacket->GetType(bIsFullType);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/15 - Seung-Won, Bae) Get Index and Data
//////////////////////////////////////////////////////////////////////

int CIWPacket::GetIndexFromData( double p_dData)
{
	if( !m_pPacket) return -1;
	return m_pPacket->GetIndexFromData( p_dData);
}

BOOL CIWPacket::GetDataPastandFuture( int p_nIndex, double &p_dData)
{
	if( !m_pPacket) return FALSE;
	return m_pPacket->GetDataPastandFuture( p_nIndex, p_dData);
}
