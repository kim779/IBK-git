// IWPacket.h: interface for the CIWPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
#define AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/DLL_Load/PacketBase.h"		// for CPacket
#include "../_IWUnknown.h"
#include "_IPacket.h"
#include "_IChartOCX.h"										// for IChartOCX

// (2004.10.11, ��¿�) Packet Data Object�� Interface Wrapper Class�� �����Ѵ�.
class CIWPacket : public CIWUnknown, public IPacket
{
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IPacket, CIWPacket, CPacket, m_pPacket)

// (2006/7/27 - Seung-Won, Bae) 
public:
	CPacket *	GetPacketPtr( void)	{	return m_pPacket;	}

// [04/10/11] Packet�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/26] Packet�� Type�� Unit Value(Double)�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual double		GetPacketTypeUnitValue( void);
	// [04/11/30] Packet Name�� ���ϴ� Interface�� �����Ѵ�.
	virtual IString *	GetPacketNameString( void);
	// ����(05/08/29) Ÿ���� �����ϴ� Interface�� �����Ѵ�.
	virtual void		SetType( const char *p_szType, const char *p_szExpressionType);
	// ����(05/08/29) Ÿ�԰��� �������� Interface�� �����Ѵ�.
	virtual IString *	GetType(const bool bIsFullType = true);

// (2004.10.15, ��¿�) Data�� ��ȸ �� �����ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
	// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
	//			TRUE	-> ���� ���� ���
	//			FALSE	-> ������ ����ų� ���� ���� ���� ���
	virtual int		GetDataCount( void);
	virtual int		GetIndexFromData( double p_dData);
	virtual BOOL	GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData);
	// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual BOOL	GetData( const int nIndex, double &p_dData);
	virtual BOOL	GetData( const int nIndex, ILPCSTR &p_pszData);
	virtual BOOL	GetDataPastandFuture( int p_nIndex, double &p_dData);
	// [04/10/15] �ش� Index�� Data�� �����ϰų� �߰��ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual BOOL	SetData( int p_nIndex, double p_dData);
	virtual void	AppendData(double dData);
	// ����(05/07/28) Data�� Ŭ�����Ѵ�. true�ϰ��� �⺻����Ÿ�� Clear.(true�� ST.������ ����� �ʿ������)
	virtual void	ClearData(const bool bBaseDataClear);

// [04/12/07] Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	virtual IDoubleList *	GetDataList( void);
	virtual IStringList *	GetStrDataList( void);
};

#endif // !defined(AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
