// IPacket.h: interface for the IPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
#define AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"		// for CPacketType::COMMODITY_BOND_NAME
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"			// for ILPCSTR

interface IDoubleList;
interface IStringList;
interface IPacket : public IUnknown
{
protected:
	virtual ~IPacket()	{}

// [04/10/11] Packet�� ������ ��ȸ �� �����ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/26] Packet�� Type�� Unit Value(Double)�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual double		GetPacketTypeUnitValue( void) = 0;
	// [04/11/30] Packet Name�� ���ϴ� Interface�� �����Ѵ�.
	virtual IString *	GetPacketNameString( void) = 0;
	// ����(05/08/29) Ÿ���� �����ϴ� Interface�� �����Ѵ�.
	virtual void		SetType( const char *p_szType, const char *p_szExpressionType) = 0;
	// ����(05/08/29) Ÿ�԰��� �������� Interface�� �����Ѵ�.
	virtual IString *	GetType(const bool bIsFullType = true) = 0;
	// (2007/2/3 - Seung-Won, Bae) Retrieve the Data Index of Real Data Start.
	virtual int			GetIndexOfRealDataStart( void) = 0;
	// (2007/5/2 - Seung-Won, Bae) Check Market Type.
	virtual CPacketType::COMMODITY_BOND_NAME	GetCommodityBondName( void)	= 0;
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	virtual int			GetDateUnitType( void) = 0;

// (2004.10.15, ��¿�) Data�� ��ȸ �� �����ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
	// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
	//			TRUE	-> ���� ���� ���
	//			FALSE	-> ������ ����ų� ���� ���� ���� ���
	virtual int		GetDataCount( void) = 0;
	virtual int		GetIndexFromData( double p_dData) = 0;
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	virtual int		GetIndexFromData( double p_dData, const char *p_szType) = 0;
	virtual BOOL	GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) = 0;
	// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual BOOL	GetData( const int nIndex, double &p_dData) = 0;
	virtual BOOL	GetData( const int nIndex, ILPCSTR &p_pszData) = 0;
	virtual BOOL	GetDataPastandFuture( int p_nIndex, double &p_dData) = 0;
	// [04/10/15] �ش� Index�� Data�� �����ϰų� �߰��ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual BOOL	SetData( int p_nIndex, double p_dData) = 0;
	virtual void	AppendData(double dData) = 0;
	// ����(05/07/28) Data�� Ŭ�����Ѵ�. true�ϰ��� �⺻����Ÿ�� Clear.(true�� ST.������ ����� �ʿ������)
	virtual void	ClearData(const bool bBaseDataClear = false) = 0;

// [04/12/07] Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	virtual IDoubleList *	GetDataList( void) = 0;
	virtual IStringList *	GetStrDataList( void) = 0;
	// ���簡 ����� - ojtaso (20080602)
	virtual void OffsetPacketData(double dData) = 0;
};

#endif // !defined(AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
