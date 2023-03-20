// IWPacket.cpp: implementation of the CIWPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacket.h"

#include "IWDoubleList.h"		// for CIWDoubleList
#include "IWStringList.h"		// for CIWStringList
#include "IWString.h"			// for CIWString

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
IString *CIWPacket::GetPacketNameString( void)
{
	if( !m_pPacket) return NULL;
	return CIWString::CreateObject( m_pPacket->GetName());
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetType
 - Created at  :  2005-08-29   11:00
 - Author      :  ������
 - Description :  ��ŶŸ���� �����Ѵ�.
					strType : �Ӽ� page���� ������ type, -> ��ġ��ȸâ, data ��ü���⿡�� ���
					strExperssionType : �Ϲ����� type -> ȭ�� ������ ���Ҷ�, scale ǥ���Ҷ� ���
 -----------------------------------------------------------------------------------------*/
void CIWPacket::SetType( const char *p_szType, const char *p_szExpressionType)
{
	if(!m_pPacket) return;
	m_pPacket->SetType( p_szType, p_szExpressionType);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetType
 - Created at  :  2005-08-29   13:50
 - Author      :  ������
 - Description :  ��ŶŸ���� �����´�.
 -----------------------------------------------------------------------------------------*/
IString *CIWPacket::GetType(const bool bIsFullType)
{
	if( !m_pPacket) return NULL;
	return CIWString::CreateObject( m_pPacket->GetType(bIsFullType));
}

// (2007/2/3 - Seung-Won, Bae) Retrieve the Data Index of Real Data Start.
int CIWPacket::GetIndexOfRealDataStart( void)
{
	if( !m_pPacket) return 0;
	return m_pPacket->GetStartPeriod();
}

// (2007/5/2 - Seung-Won, Bae) Check Market Type.
CPacketType::COMMODITY_BOND_NAME CIWPacket::GetCommodityBondName( void)
{
	if( !m_pPacket) return CPacketType::NONAME;
	return m_pPacket->GetCommodityBondName();
}

// ����Ʈ �����ð� ���� - ojtaso (20080811)
int CIWPacket::GetDateUnitType( void)
{
	if( !m_pPacket) return -1;
	return m_pPacket->GetDateUnitType();
}

//////////////////////////////////////////////////////////////////////
// (2004.10.15, ��¿�) Data�� ��ȸ �� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
//			TRUE	-> ���� ���� ���
//			FALSE	-> ������ ����ų� ���� ���� ���� ���
int CIWPacket::GetDataCount( void)
{
	if( m_pPacket) return m_pPacket->GetCount();
	return 0;
}
int CIWPacket::GetIndexFromData( double p_dData)
{
	if( !m_pPacket) return -1;
	return m_pPacket->GetIndexFromData( p_dData);
}
// ����Ʈ �����ð� ���� - ojtaso (20080811)
int CIWPacket::GetIndexFromData( double p_dData, const char *p_szType)
{
	if( !m_pPacket) return -1;
	return m_pPacket->GetIndexFromData( p_dData, p_szType);
}
BOOL CIWPacket::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData)
{
	if( m_pPacket) return m_pPacket->GetCurOrPrevIndexFromData( dData, nCurOrPrevIndex, dCurOrPrevIndexData);
	return FALSE;
}

// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�. (Zero Base)
BOOL CIWPacket::GetData(const int nIndex, double &p_dData)
{
	p_dData = ERRORVALUE;
	if( !m_pPacket) return FALSE;
	p_dData = m_pPacket->GetData( nIndex);
	return p_dData != ERRORVALUE;
}
BOOL CIWPacket::GetData( const int nIndex, ILPCSTR &p_pszData)
{
	p_pszData = NULL;
	if( !m_pPacket) return FALSE;
	p_pszData = CIWString::CreateObject( m_pPacket->GetStrData( nIndex));
	return TRUE;
}
BOOL CIWPacket::GetDataPastandFuture( int p_nIndex, double &p_dData)
{
	if( !m_pPacket) return FALSE;
	return m_pPacket->GetDataPastandFuture( p_nIndex, p_dData);
}

// [04/10/15] �ش� Index�� Data�� �����ϰų� �߰��ϴ� Interface�� �����Ѵ�. (Zero Base)
BOOL CIWPacket::SetData( int p_nIndex, double p_dData)
{
	if( !m_pPacket) return FALSE;
	m_pPacket->UpdateData( p_dData, p_nIndex);
	return TRUE;
}
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


//////////////////////////////////////////////////////////////////////
// [04/12/07] Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/12/07] Double Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
IDoubleList *CIWPacket::GetDataList( void)
{
	if( !m_pPacket) return NULL;
	CList<double, double> *plDoubleList = m_pPacket->GetnumericDataList();
	if( !plDoubleList) return NULL;
	return CIWDoubleList::CreateObject( plDoubleList);
}

// (2005/9/2 - Seung-Won, Bae) Get String Data List
IStringList *CIWPacket::GetStrDataList( void)
{
	if( !m_pPacket) return NULL;
	CList<CString, CString> *plStringList = m_pPacket->GetstrDataList();
	if( !plStringList) return NULL;
	return CIWStringList::CreateObject( plStringList);
}

// ���簡 ����� - ojtaso (20080602)
void CIWPacket::OffsetPacketData(double dData)
{
	if(!m_pPacket)
		return;

	m_pPacket->OffsetPacketData(dData);
}