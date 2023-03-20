// IGraphInfo.h: interface for the IGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
#define AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
#include "_AddInVer.h"	// for AddIn Version Define

interface IGraphInfo : public IUnknown
{
protected:
	virtual ~IGraphInfo() {}
		
public:
	// ����(05/07/14) ��ǥ ��꿡 �ʿ��� ���ǰ��� �����Ѵ�.
	virtual CList<double, double>* GetCommonCalculateVarList() = 0;
	virtual CList<double, double>* GetCalculateVarList(const char* pcszSubGraphName) = 0;
	virtual CList<double, double>* GetCalculateVarList(const int nSubGraphIndex) = 0;

	// ����(05/07/15) ��꿡 ���̴� ��Ŷ���� �����Ѵ�.
	virtual LPCTSTR GetCommonCalculatePacketNames() = 0;
	virtual LPCTSTR GetCalculatePacketNames(const int nSubGraphIndex) = 0;

// (2006/7/7 - Seung-Won, Bae) SubGraph Info
public:
	// ����׷����� �ε����� �����Ѵ�.
	virtual int		GetSubGraphIndex(const char* pcszSubGraphName) = 0;	
	virtual int		GetSubGraphCount( void) = 0;
	// ����(05/07/29) ����׷����� Show/Hide ���θ� �����Ѵ�.
	virtual bool	IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow) = 0;
	virtual bool	IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve Graph Info by string
public:
	virtual BOOL	GetGraphDataForMaking( CString &p_strData)			= 0;
	virtual BOOL	GetAllSubGraphDataForMaking( CString &p_strData)	= 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Data
public:
	virtual void	GetGraphDataDrawingData( BOOL &p_bLowHigh, BOOL &p_bLowCur, BOOL &p_bHighCur, BOOL &p_bPrevCur, int &p_nCurData) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve Signal Info
public:
	virtual void	GetSignalInfo( BOOL &p_bValidFlag, BOOL &p_bShowSignal) = 0;
};

#endif // !defined(AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
