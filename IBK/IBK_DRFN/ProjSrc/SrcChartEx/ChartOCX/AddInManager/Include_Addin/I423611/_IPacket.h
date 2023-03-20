// IPacket.h: interface for the IPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
#define AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CList
#include "_AddInVer.h"		// for AddIn Version Define

interface IPacket : public IUnknown
{
protected:
	virtual ~IPacket()	{}

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
	virtual CList< double, double> *	GetDataList( void) = 0;
	// (2005/9/2 - Seung-Won, Bae) Get String Data List
	virtual CList< CString, CString> *	GetStrDataList( void) = 0;
	
	// ����(05/07/26) Data�� �߰��ϴ� Interface�� �����Ѵ�.
	virtual void						AppendData(double dData) = 0;
	
	// ����(05/07/28) Data�� Ŭ�����Ѵ�. true�ϰ��� �⺻����Ÿ�� Clear.(true�� ST.������ ����� �ʿ������)
	virtual void						ClearData(const bool bBaseDataClear = false) = 0;
	// ����(05/08/29) Ÿ���� �����ϴ� Interface�� �����Ѵ�.
	virtual void						SetType(const CString& strType, const CString& strExpressionType) = 0;
	// ����(05/08/29) Ÿ�԰��� �������� Interface�� �����Ѵ�.
	virtual CString						GetType(const bool bIsFullType = true) = 0;
	// ����(05/09/01) �ش� Index�� ����Ÿ�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual bool						GetData(const int nIndex, double& dData) = 0;

};

#endif // !defined(AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
