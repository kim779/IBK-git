// RsiPacketGroup.h: interface for the CRsiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSIPACKETGROUP_H__E74B9B17_E6E8_4D4D_8DE0_DFEBAE9FD8AC__INCLUDED_)
#define AFX_RSIPACKETGROUP_H__E74B9B17_E6E8_4D4D_8DE0_DFEBAE9FD8AC__INCLUDED_

// (2004.11.29, ��¿�) Real Setting Info�� �����ϴ� Data Object List Class�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CTypedPtrList

// (2004.11.29, ��¿�) Real Setting Info Object Class�� �����Ѵ�.
class CRealSettingInfo : public CObject
{
public:
	CRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
	{
		m_strChartPacketName	= p_szChartPacketName;
		m_strRealFieldName		= p_szRealFieldName;
		m_strShareName		= p_szShareName;
		m_strRQ.Empty();			// RQ�� Real���� : �������� - ojtaso (20070111) 
	}
	// RQ�� Real���� : �������� - ojtaso (20070111)
	CRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
	{
		m_strChartPacketName	= p_szChartPacketName;
		m_strRealFieldName		= p_szRealFieldName;
		m_strShareName		= p_szShareName;
		m_strRQ = lpszRQ;
	}
public:
	CString		m_strChartPacketName;
	CString		m_strRealFieldName;
	CString		m_strShareName;	
	CString		m_strRQ;			// RQ�� Real���� : �������� - ojtaso (20070111)
};

// (2004.11.29, ��¿�) ���� Real Packet Name ���� �����ϴ� Real Info List Packet Group Base Class�� �����Ѵ�.
typedef CTypedPtrList< CObList, CRealSettingInfo *> CRsiPacketGroupBase;

// (2004.11.29, ��¿�) Real Setting Info�� Real Packet Name���� �����ϴ� Data Object Class�� �����Ѵ�.
class CRsiPacketGroup : public CObject
{
public:
	CRsiPacketGroup( const char *p_szRealPacketName);
	virtual ~CRsiPacketGroup();

// (2004.11.29, ��¿�) �������� Real Setting Info�� Packet ���� �����Ѵ�.
public:
	CString	m_strRealPacketName;

// (2004.11.30, ��¿�) CDrdsAdvisedInfoList�� CDaiPacketGroup Adding�� CDaiPacketGroup�� CDrdsAdvisedInfoList�� 
//		�������� �ʵ��� CDaiPacketGroup�� CDaiPacketGroupBase���� ��¹��� �ʰ� Member�� �����Ѵ�.
protected:
	CRsiPacketGroupBase		m_lRsiPacketGroup;
public:
	void				RemoveAt( POSITION position)			{	m_lRsiPacketGroup.RemoveAt( position);			}
	BOOL				IsEmpty( void) const					{	return m_lRsiPacketGroup.IsEmpty();				}
	POSITION			AddTail( CRealSettingInfo *newElement)	{	return m_lRsiPacketGroup.AddTail( newElement);	}
	POSITION			GetHeadPosition( void) const			{	return m_lRsiPacketGroup.GetHeadPosition();		}
	CRealSettingInfo *	GetAt( POSITION position) const			{	return m_lRsiPacketGroup.GetAt( position);		}
	CRealSettingInfo *&	GetNext( POSITION& rPosition)			{	return m_lRsiPacketGroup.GetNext( rPosition);	}
	CRealSettingInfo *	RemoveHead( void)						{	return m_lRsiPacketGroup.RemoveHead();			}

// (2004.11.29, ��¿�) Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
	BOOL	AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// RQ�� Real���� : �������� - ojtaso (20070111)
	BOOL	AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);

// (2004.11.29, ��¿�) ������ Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
	void	RemoveAllRealSettingInfo( void);
	// 2. Real Setting Info ���� �����޾� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
	BOOL	RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// RQ�� Real���� : �������� - ojtaso (20070111)
	BOOL	RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);

// (2004.11.29, ��¿�) ������ Real Setting Info�� �˻��ϴ� Interface�� �����Ѵ�.
public:
	// 1. Real Setting Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
	POSITION	FindRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// RQ�� Real���� : �������� - ojtaso (20070111)
	POSITION	FindRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// 2. Local Share Name���� �����޾� �ش� ���������� ��ϵ� Real Setting Info�� �ִ��� �˻��Ѵ�.
	POSITION	FindShareName( const char *p_szShareName);
};

#endif // !defined(AFX_RSIPACKETGROUP_H__E74B9B17_E6E8_4D4D_8DE0_DFEBAE9FD8AC__INCLUDED_)
