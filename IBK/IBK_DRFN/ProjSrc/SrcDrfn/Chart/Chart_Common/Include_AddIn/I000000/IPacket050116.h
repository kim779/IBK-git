// IPacket050116.h: interface for the IPacket050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKET050116_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
#define AFX_IPACKET050116_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_

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

// (2004.10.11, ��¿�) Packet Data Object�� Interface�� �����Ѵ�.
//		AddIn DLL ���� Interface�� Macro Interface�� �����Ѵ�.
#if IPacketVer < 050116
	#define	IPacket		IPacket050116
	#undef	IPacketVer
	#define	IPacketVer	050116
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CList

class IPacket050116 : public IUnknown  
{
protected:
	virtual ~IPacket050116()	{}

// [04/10/11] Packet�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Packet�� Type�� String���� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL		GetPacketTypeString( CString &p_strPacketType) = 0;
	// [04/10/26] Packet�� Type�� Unit Value(Double)�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual double		GetPacketTypeUnitValue( void) = 0;
	// [04/11/30] Packet Name�� ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL		GetPacketName( CString &p_strPacketName) = 0;

// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
//			TRUE	-> ���� ���� ���
//			FALSE	-> ������ ����ų� ���� ���� ���� ���
public:
	virtual BOOL		GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) = 0;

// (2004.10.15, ��¿�) Data�� ��ȸ �� �����ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual int							GetDataCount( void) = 0;
	// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual double						GetData( int p_nIndex) = 0;
	// [04/10/15] �ش� Index�� Data�� �����ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual BOOL						SetData( int p_nIndex, double p_dData) = 0;
	// [04/12/07] Double Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual CList< double, double>	*	GetDataList( void) = 0;
};

#endif // !defined(AFX_IPACKET050116_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
