// IWPacket.h: interface for the CIWPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
#define AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/Packetbase.h"			// for CPacket
#include "../_IWUnknown.h"
#include "_IPacket.h"
#include "_IChartOCX.h"											// for IChartOCX

// RQ�� Packet���� : �������� - ojtaso (20070112)
#define _IWRAPPER_IUNKNOWN_MANAGER_CREATE_RQ(RQ, Interface, IWrapper, Target, MapStringToPtr, RQMap)	\
protected:																						\
	MapStringToPtr RQMap;																		\
public:																							\
	static Interface *CreateObject(RQ lpszRQ, Target *p_pTarget)								\
	{																							\
		IWrapper *pIWrapper = NULL;																\
		if( m_lReleaseMgr.IsEmpty())	pIWrapper = new IWrapper;								\
		else							pIWrapper = ( IWrapper *)m_lReleaseMgr.RemoveHead();	\
		pIWrapper->AddRef();																	\
		pIWrapper->SetTarget( p_pTarget, FALSE);												\
		pIWrapper->RQMap.SetAt(lpszRQ, (LPVOID)p_pTarget);										\
		return pIWrapper;																		\
	}		

// (2004.10.11, ��¿�) Packet Data Object�� Interface Wrapper Class�� �����Ѵ�.
class CIWPacket : public CIWUnknown, public IPacket
{
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IPacket, CIWPacket, CPacket, m_pPacket)
	// RQ�� Packet���� : �������� - ojtaso (20070112)
	_IWRAPPER_IUNKNOWN_MANAGER_CREATE_RQ(LPCSTR, IPacket, CIWPacket, CPacket, CMapStringToPtr, m_mapPacket)

// [04/10/11] Packet�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Packet�� Type�� String���� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL		GetPacketTypeString( CString &p_strPacketType);
	// [04/10/26] Packet�� Type�� Unit Value(Double)�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual double		GetPacketTypeUnitValue( void);
	// [04/11/30] Packet Name�� ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL		GetPacketName( CString &p_strPacketName);

// [04/10/11] �־����� ���� Data Index, �Ǵ� �� Data ���� �־���� ��ġ ������ Index�� ���Ѵ�. (Sort ����)
//			TRUE	-> ���� ���� ���
//			FALSE	-> ������ ����ų� ���� ���� ���� ���
public:
	virtual BOOL		GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData);

// (2004.10.15, ��¿�) Data�� ��ȸ �� �����ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/15] Data�� ���� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual int							GetDataCount( void);
	// [04/10/11] �ش� Index�� Data�� ��ȸ�ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual double						GetData( int p_nIndex);
	// [04/10/15] �ش� Index�� Data�� �����ϴ� Interface�� �����Ѵ�. (Zero Base)
	virtual BOOL						SetData( int p_nIndex, double p_dData);
	// [04/12/07] Double Data List�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual CList< double, double>	*	GetDataList( void);
	// (2005/9/2 - Seung-Won, Bae) Get String Data List
	virtual CList< CString, CString> *	GetStrDataList( void);
	// ����(05/07/26) Data�� �߰��ϴ� Interface�� �����Ѵ�.
	virtual void						AppendData(double dData);
	// ����(05/07/28) Data�� Clear�Ѵ�. 
	virtual void						ClearData(const bool bBaseDataClear = false);
	// ����(05/08/29) ��ŶŸ���� �����ϴ� Interface�� �����Ѵ�.
	virtual void						SetType(const CString& strType, const CString& strExpressionType);
	// ����(05/08/29) Ÿ�԰��� �������� Interface�� �����Ѵ�.
	virtual CString						GetType(const bool bIsFullType = true);
	// ����(05/09/01) �ش� Index�� ����Ÿ�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual bool						GetData(const int nIndex, double& dData);
};

#endif // !defined(AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
