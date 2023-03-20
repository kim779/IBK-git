// IWPacket050531.h: interface for the CIWPacket050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKET050531_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
#define AFX_IWPACKET050531_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) Packet Data Object�� Interface Wrapper Class�� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IPacket050531.h"

class CPacket;
class CIWPacket050531 : public CIWUnknown, public IPacket050531
{
public:
	CIWPacket050531( CPacket *p_pPacket);
	virtual ~CIWPacket050531();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.11, ��¿�) Packet Data Object�� Pointer�� �����Ѵ�.
protected:
	CPacket *	m_pPacket;

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

#endif // !defined(AFX_IWPACKET050531_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
