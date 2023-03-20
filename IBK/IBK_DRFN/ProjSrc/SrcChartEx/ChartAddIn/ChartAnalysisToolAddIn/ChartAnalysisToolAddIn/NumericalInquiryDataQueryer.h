// NumericalInquiryDataQueryer.h: interface for the CNumericalInquiryDataQueryer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMERICALINQUIRYDATAQUERYER_H__09E3EC22_7739_49FE_A42B_BEA331215EE3__INCLUDED_)
#define AFX_NUMERICALINQUIRYDATAQUERYER_H__09E3EC22_7739_49FE_A42B_BEA331215EE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>									// for CTypedPtrList
#include "../Include_Analysis/Include_AddIn.h"			// for Ixxxxxx.h

class CRowData : public CObject
{
public:
	CRowData( const char *p_szName, const char *p_szData)
	{
		m_strName = p_szName;
		m_strData = p_szData;
	}
	CRowData( const CRowData& src)
	{
		m_strName = src.m_strName;
		m_strData = src.m_strData;
	}
public:
	CString	m_strName;
	CString m_strData;
public:
	void operator =( const CRowData& src)
	{
		m_strName = src.m_strName;
		m_strData = src.m_strData;
	}
};
typedef CTypedPtrList < CObList, CRowData *> CInquiryItem;
typedef CTypedPtrList < CObList, CInquiryItem *> CInquiryDataBase;
// (2006/2/8 - Seung-Won, Bae) Redefine AddTail to distinguish AddTail of 'CObList *'
class CInquiryData : public CInquiryDataBase
{
public:
	POSITION AddTail( CInquiryItem *newElement) { return CObList::AddTail( ( CObject *)newElement); }
};

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/24
// Comments		: Data Queryer for Tooltip & Numerical Data Inquery Dlg
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CNumericalInquiryDataQueryer : public CObject  
{
public:
	CNumericalInquiryDataQueryer( IChartOCX *p_pIChartOCX);
	virtual ~CNumericalInquiryDataQueryer();

// (2006/6/24 - Seung-Won, Bae) Manage the OCX Interfaces
protected:
	IChartOCX *				m_pIChartOCX;
	IChartManager *			m_pIChartManager;
	IPacketManager *		m_pIDefaultPacketManager;
	IPacketListManager *	m_pIPacketListManager;

// (2006/6/24 - Seung-Won, Bae) Manage the Data with Group (?)
public:	// for Numerical Inquiry Dlg
	CInquiryData	m_listPrevInquiryData;	// ��ġ��ȸâ�� ����� ��ȸ����Ÿ �����̳�.
	CInquiryData	m_listInquiryData;	// ��ġ��ȸâ�� ����� ��ȸ����Ÿ �����̳�.
	int				m_nMaxDecimal;					// ��ġ���� ����Ÿ�� �ִ� �Ҽ��� �ڸ���.

// (2006/6/24 - Seung-Won, Bae) Overations
public:
	// 1. Get the Data
	//>>[A00000441]ǳ������ ����� ��ü ��ȸ �ɼ� �߰�
	//BOOL			ParsingInquiryData(CString szRQ, CPoint point, bool bToolTip = false, BOOL bPairTick = FALSE);					// ���콺�����Ͱ� ��ġ�Ѱ��� ����Ÿ�Ľ��Ͽ� ��������.
	BOOL			ParsingInquiryData(CString szRQ, CPoint point, bool bToolTip = false, BOOL bPairTick = FALSE, BOOL bViewAll = TRUE);					// ���콺�����Ͱ� ��ġ�Ѱ��� ����Ÿ�Ľ��Ͽ� ��������.
	//<<
	// XScale Text ����ǥ�� - ojtaso (20080429)
	BOOL			ParsingInquiryHorzScaleTextData(CString szRQ, CPoint point, CString& strScaleText);
	// 2. Get the first Inquiry Data
	CInquiryItem *	GetHead( void);
	// ��ġ��ȸ, ������ ����� �߰� ǥ�� - ojtaso (20080710)
	CInquiryItem *	GetPrevHead( void);
	// 3. Check the Data View Width
	BOOL			GetInquiryDataMaxLength( CInquiryItem *pListData, int& nFirstColumnMaxLength, int& nSecondColumnMaxLength);	// ��ġ��ȸ����Ÿ���� �ִ���� ��������.

// (2006/6/24 - Seung-Won, Bae) Library Functions
protected:
	UINT	FindInquiryDataIndex( CPoint point, POSITION p_psnViewData);			// ���콺 ��ġ�Ѱ��� DataIndex��������.
	// ��ġ��ȸâ�� �ڷ����� Format, X Scale �������� ���� ó�� - ojtaso (20070827)
	BOOL	SetHorzScaleDataToContainer(CString szRQ, const CString& strHorzScaleName, int nDataIndex, CInquiryItem *listData, const int nRow, const int nColumn);	// ���ν����ϵ���Ÿ �����̳ʿ� ����.
	void	SetInquiryDataToContainer(CString szRQ, const CString& strTitle, const CString& strPacketNames, int nDataIndex, CInquiryItem *listData, bool bToolTip, bool bScaleData_Date, bool bSetTitle = true);	// ��ȸ����Ÿ �����̳ʿ� ����.
	CString GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const;				// 1ƽ�ϰ�� ��Ŷ����.
// (2006/7/10 - Seung-Won, Bae) Use new Logic for Data Index of Mouse Point (TimeMarker version. BaeSeungWon.)
//	int		GetIndexInRightMargin(const int nIndex) const;

// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in Numerical Inquiry Dialog
protected:
	BOOL	m_bWithHorizontalLine;
	HWND	m_hOcxWnd;
public:
	void	EnableHorizontalLineInNIDLG( BOOL p_bEnable)	{	m_bWithHorizontalLine = p_bEnable;	}

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
public:
	void	ClearInquiryDataList( void);
};

#endif // !defined(AFX_NUMERICALINQUIRYDATAQUERYER_H__09E3EC22_7739_49FE_A42B_BEA331215EE3__INCLUDED_)
