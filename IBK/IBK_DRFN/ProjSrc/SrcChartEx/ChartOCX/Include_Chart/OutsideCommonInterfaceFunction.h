// OutsideCommonInterfaceFunction.h: interface for the COutsideCommonInterfaceFunction class.
// ����(05/07/28) ��Ƽ��Ʈ��(�ܺ�DLL)���� �Լ��� ȣ���ϴ� �Լ����� ����ü.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTSIDECOMMONINTERFACEFUNCTION_H__6E946239_8F15_49F3_89C0_6D6A117578DA__INCLUDED_)
#define AFX_OUTSIDECOMMONINTERFACEFUNCTION_H__6E946239_8F15_49F3_89C0_6D6A117578DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

#include "BlockBaseEnum.h"				// for CToolOptionInfo

class CMChartInterface;
class CMChartInterface061115;
class COldMChartInterface;			// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
class COldMChartInterface061115;
class COutsideCommonInterface;
class COutsideCommonInterfaceFunction  
{
//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for Interface in COutsideCommonInterface
//////////////////////////////////////////////////////////////////////
private:
	COutsideCommonInterface* m_pOutsideCommonInterface;

public:
	COutsideCommonInterfaceFunction();
	virtual ~COutsideCommonInterfaceFunction();

	void						SetOutsideCommonInterface(COutsideCommonInterface* pOutsideCommonInterface, CString p_strDataPath = "");
	COutsideCommonInterface *	GetOutsideCommonInterface( void);

public:
	// ����ó��.
	void Outside_SetRealInfo (void* pDrds, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);
	void Outside_UnSetRealInfo (void* pDrds, HWND hWnd,	UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);

	// Hts Path�� User ID ���ϱ�
	LPCTSTR Outside_GetMainPath();
	LPCTSTR Outside_GetUserID();
	
	// TR ��û
	void Outside_RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew);

	// ������ǥ Load ������ ���� ����
	bool Outside_IsLoad_MarketChart() const;

//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for Interface in CMChartInterface
//////////////////////////////////////////////////////////////////////
public:
	BOOL				InitMChartInterface();
private:
	BOOL						m_bMChartInterfaceInited;
	CMChartInterface *			m_pMChartInterface;
	COldMChartInterface *		m_pOldMChartInterface;	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface

public:
	// ����Ÿâ.
	bool MChart_IsActiveDataWindow();
	void MChart_SetDataWindowData(CList<CString, CString&>* pListData);

	// ȯ�漳��.
	bool MChart_SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)	;	// ��������� �� ������
	bool MChart_SetYScalePositionToOutside(const int nPosition)				;	// Y�� ����ǥ�� (0:����, 1:������, 2:����, 3:����)
	bool MChart_SetRightMarginToOutside(const int nRightMargin)				;	// ������ ����
	bool MChart_SetShowIndicatorVariableToOutside(const bool bShow)			;	// ��ǥ���� - ��ǥ���� ǥ��
	bool MChart_SetShowIndicatorValueToOutside(const bool bShow)			;	// ��ǥ���� - ��ǥ�� ǥ��
	bool MChart_SetShowHighLowBoundToOutside(const bool bShow)				;	// ��/���Ѱ� ǥ��
	bool MChart_SetShowExRightDividendToOutside(const bool bShow)			;	// �Ǹ���/���� ǥ��
	bool MChart_SetShowCurrentPriceToOutside(const int nShowType)			;	// ���ݴ������� ���簡ǥ��
	bool MChart_SetTrendLineColorToOutside(const COLORREF clrLineColor)		;	// �߼��� ����
	bool MChart_SetShowTrendLineValueToOutside(const int nPosition)			;	// �߼����� ǥ�� (0: , 1: , 2: , 3: )
	bool MChart_SetTimeLinkageToOutside(const bool bTimeLinkage)			;	// �ð����� ����
	bool MChart_SetAutoSaveToOutside(const bool bAutoSave)					;	// ��Ʈ����� �ڵ� ���� ����
	bool MChart_SetShowTrendLineTextToOutside(const bool bShow)				;	// �߼���Text ���̱�
	bool MChart_SetShowTimeLineTextToOutside(const bool bShow)				;	// �ð���Text ���̱�
	bool MChart_SetShowPriceLineLeftTextToOutside(const bool bShow)			;	// ���ݼ� ���� Text ���̱�
	bool MChart_SetShowPriceLineRightTextToOutside(const bool bShow)		;	// ���ݼ� ������ Text ���̱�
	bool MChart_SetShowTrisectionLineTextToOutside(const bool bShow)		;	// ���м� Text ���̱�
	bool MChart_SetShowQuadrisectionLineTextToOutside(const bool bShow)		;	// ���м� Text ���̱�
	bool MChart_SetShowControlLineTextToOutside(const bool bShow)			;	// ������ Text ���̱�
	bool MChart_GetImaginaryCandleToOutside(bool& bImaginaryCandle)			;	// ���������.
	bool MChart_GetUseGapRevisionToOutside(bool& bUseGapRevision)			;	// ������
	bool MChart_GetYScalePositionToOutside(int& nPosition)					;	// Y�� ���� ǥ��
	bool MChart_GetRightMarginToOutside(int& nRightMargin)					;	// ������ ����
	bool MChart_GetShowIndicatorVariableToOutside(bool& bShow)				;	// ��ǥ���� - ��ǥ���� ǥ��
	bool MChart_GetShowIndicatorValueToOutside(bool& bShow)					;	// ��ǥ���� - ��ǥ��
	bool MChart_GetShowHighLowBoundToOutside(bool& bShow)					;	// ��/���Ѱ� ǥ��
	bool MChart_GetShowExRightDividendToOutside(bool& bShow)				;	// �Ǹ���/���� ǥ��
	bool MChart_GetShowCurrentPriceToOutside(int& nShowType)				;	// ���� �������� ���簪 ǥ��
	bool MChart_GetTrendLineColorToOutside(COLORREF& clrLineColor)			;	// �߼��� ����
	bool MChart_GetShowTrendLineValueToOutside(int& nPosition)				;	// �߼����� ǥ��
	bool MChart_GetTimeLinkageToOutside(bool& bTimeLinkage)					;	// �ð����� ����
	bool MChart_GetAutoSaveToOutside(bool& bAutoSave)						;	// ��Ʈ����� �ڵ�����
	bool MChart_GetShowTrendLineTextToOutside(bool& bShow)					;	// �߼���Text ���̱�
	bool MChart_GetShowTimeLineTextToOutside(bool& bShow)					;	// �ð���Text ���̱�
	bool MChart_GetShowPriceLineLeftTextToOutside(bool& bShow)				;	// ���ݼ� ���� Text ���̱�
	bool MChart_GetShowPriceLineRightTextToOutside(bool& bShow)				;	// ���ݼ� ������ Text ���̱�
	bool MChart_GetShowTrisectionLineTextToOutside(bool& bShow)				;	// ���м� Text ���̱�
	bool MChart_GetShowQuadrisectionLineTextToOutside(bool& bShow)			;	// ���м� Text ���̱�
	bool MChart_GetShowControlLineTextToOutside(bool& bShow)				;	// ������ Text ���̱�
	bool MChart_ChartEnvSetupOk();						// Ok��ư�� �������� ��Ƽ��Ʈ�� �˷��ش�.
	bool MChart_SetPriceLog(const bool bLog);	// ������Ʈ log ����

	bool MChart_OnChartEnvSetupOk( void);

//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for New Interface in 06/11/15
//////////////////////////////////////////////////////////////////////
public:
	BOOL						InitMChartInterface061115();
private:
	BOOL						m_bMChartInterfaceInited061115;
	CMChartInterface061115 *	m_pMChartInterface061115;
	COldMChartInterface061115 *	m_pOldMChartInterface061115;	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface

// (2006/11/15 - Seung-Won, Bae) Revised Price
public:
	BOOL	GetRevisedPriceFlag( void);
	BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag);

// (2006/11/15 - Seung-Won, Bae) Notify the Event of Indicator Added or Removed.
public:
	BOOL	OnIndicatorAddOrRemoved( const char *p_szGraphName, const char *p_szIndicatorName, const BOOL p_bAdded);

// (2006/11/15 - Seung-Won, Bae) User Zoom Changed.
public:
	BOOL	OnUserZoomChanged( const int p_nDataCountInView);

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
public:
	BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);

	// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
	BOOL	OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID, LPCTSTR strCustItem1, LPCTSTR strCustItem2, LPCTSTR strCustItem3);

// (2009/9/3 - Seung-Won, Bae) for Old MChartInterface
protected:
	BOOL	m_bIsOldInterface;
};

#endif // !defined(AFX_OUTSIDECOMMONINTERFACEFUNCTION_H__6E946239_8F15_49F3_89C0_6D6A117578DA__INCLUDED_)
