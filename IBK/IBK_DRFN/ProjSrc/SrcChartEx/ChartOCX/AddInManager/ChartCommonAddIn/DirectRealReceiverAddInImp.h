// DirectRealReceiverAddInImp.h: interface for the CDirectRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTREALRECEIVERADDINIMP_H__54C4A92B_82E7_41CE_A02F_35878B5DF8D4__INCLUDED_)
#define AFX_DIRECTREALRECEIVERADDINIMP_H__54C4A92B_82E7_41CE_A02F_35878B5DF8D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RealReceiverAddInImp.h"						// for CChartAddInBase

class CPacketListManager;			// PacketListManager �߰� : �������� - ojtaso (20070118)
class CXScaleManager;				// XScaleManager �߰� : �������� - ojtaso (20070118)
class CDirectRealReceiverAddInImp : public CRealReceiverAddInImp
{
public:
	CDirectRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CDirectRealReceiverAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;


// (2009/9/14 - Seung-Won, Bae) for Old Interface
private:
	virtual BOOL	SetRealData( const char *p_szPacketName, double p_dData)
					{	return SetRealDataRQ( _T("DEFAULT"), p_szPacketName, p_dData, TRUE);		}
	virtual BOOL	SetRealData( const char *p_szPacketName, double p_dData, BOOL p_bRealData)
					{	return SetRealDataRQ( _T("DEFAULT"), p_szPacketName, p_dData, p_bRealData);	}
	virtual BOOL	SetRealDataRQ(LPCTSTR lpszRQ, const char *p_szPacketName, double p_dData)
					{	return SetRealDataRQ( lpszRQ, p_szPacketName, p_dData, TRUE);				}
	virtual BOOL	SetRealPacketEnd( BOOL p_bRealData)
					{	return SetRealPacketEndRQ( _T("DEFAULT"), p_bRealData);						}
	virtual BOOL	SetRealPacketEnd( void)
					{	return SetRealPacketEndRQ( _T("DEFAULT"), TRUE);							}
	virtual BOOL	SetRealPacketEndRQ(LPCTSTR lpszRQ)
					{	return SetRealPacketEndRQ( lpszRQ, TRUE);									}


protected:
	virtual BOOL	UpdatePacketData( const char *p_szPacketName, const int p_nIndex, const double p_dData);
	// �������� ����ó�� : �������� - ojtaso (20070208)
	// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	virtual BOOL	SetRealDataRQ(LPCTSTR lpszRQ, const char *p_szPacketName, double p_dData, BOOL p_bRealData);
	virtual BOOL	SetRealPacketEndRQ(LPCTSTR lpszRQ, BOOL p_bRealData);


//////////////////////////////////////////////////////////////////////
// (2006/8/1 - Seung-Won, Bae) for Direct Real
//////////////////////////////////////////////////////////////////////
protected:
	// ���簡 ����� - ojtaso (20080602)
	virtual void	OffsetPacketData(LPCTSTR lpszRQ, double dData);

// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
protected:
	virtual _int64			GetNMTRClock( void)		{	return CRealReceiverAddInImp::GetNMTRClock();	}
};

#endif // !defined(AFX_DIRECTREALRECEIVERADDINIMP_H__54C4A92B_82E7_41CE_A02F_35878B5DF8D4__INCLUDED_)
