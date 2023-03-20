// YScaleSetUpAddInImp.h: interface for the CYScaleSetUpAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSCALESETUPADDINIMP_H__10CBB4F1_76B7_42A8_8400_DC07241C26DE__INCLUDED_)
#define AFX_YSCALESETUPADDINIMP_H__10CBB4F1_76B7_42A8_8400_DC07241C26DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

interface IBlock;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 07
// Comments		: YScale ���� AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CYScaleSetUpAddInImp : public CChartAddInBase  
{
public:
	enum YSCALE_APPLY_STATE
	{
		APPLY_CANCEL,			// ����
		APPLY_VALUE,	// ������
		APPLY_PERCENT	// ��������
	};

public:
	CYScaleSetUpAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CYScaleSetUpAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

public:
	void SetYScaleAllValue( int nYScaleSetUpType, double dMax, double dMin, double dMaxPercent, double dMinPercent );

protected:
	YSCALE_APPLY_STATE	m_eYScaleSetupApplyState;		// YScale ���� ����

	int m_nVertScaleGroupIndex;
	
	CString m_strGraphName;				// Graph �̸�
	CString m_strStockCode;				// �����ڵ�
	CString m_strPrevStockCode;			// ���� �����ڵ�
	CString	m_strExtMsg;				// ���� Message�� �ܿ� �κ��̴�.
										//		(�⺻���� '�׷�����:����,����'���� �����Ǹ� �ڿ� ':�����ڵ�'�� �߰��� �� �ִ�.)
	
	CString m_strFormat;				// ȣ�� ���е�.

	double	m_dYesterdayClosePrice;		// ���� ����
	double	m_dUpperBound;				// ���Ѱ�
	double	m_dLowerBound;				// ���Ѱ�
	
	int m_nYScaleMaxMinSetupType;		// ������ / ���� ( 0 : ������, 1 : ���� )

	// ������ - �ִ� / �ּҰ�
	double m_dYScaleMaxValue;
	double m_dYScaleMinValue;

	// ���� - �ִ� / �ּҰ�
	double m_dYScaleMaxPercentValue;
	double m_dYScaleMinPercentValue;


protected:
	void ResetYScaleSetUp( const char *p_szBound );
	
	void SetYesterdayClosePrice(const CString& strData);
	
	BOOL SetYScaleInfo( IBlock *pIBlock );

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData); // (2005/9/2 - Seung-Won, Bae) for String Data

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////
protected:
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
};

#endif // !defined(AFX_YSCALESETUPADDINIMP_H__10CBB4F1_76B7_42A8_8400_DC07241C26DE__INCLUDED_)
