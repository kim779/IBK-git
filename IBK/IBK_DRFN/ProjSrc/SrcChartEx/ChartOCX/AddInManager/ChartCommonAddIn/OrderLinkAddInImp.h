// OrderLinkAddInImp.h: interface for the COrderLinkAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERLINKADDINIMP_H__D201031A_2204_4FDF_B676_81AB5EAFB10C__INCLUDED_)
#define AFX_ORDERLINKADDINIMP_H__D201031A_2204_4FDF_B676_81AB5EAFB10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoundMarkerAddInImp.h"

interface IChartManager;

class COrderLinkAddInImp : public CBoundMarkerAddInImp  
{
public:
	COrderLinkAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~COrderLinkAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.10.25, ��¿�) ȣ�������� ���� Member Variable�� �����Ѵ�.
protected:
	bool		m_bRun;

	int			m_nIndicatorPosY;				// Current Indicator Drawing Position			(IndicatorPosition)
	BOOL		m_bOnSetCaprture;				// Indicator Drawing Situation Flag			(SetCaptureFlag)
	CRect		m_rctAskingPriceMark;			// Indicator Region of BoundMark			(IndicatorRegion)
	double		m_dIndicatedPrice;				// Indicated Price							(IndicatedPrice)
	// (2006/11/5 - Seung-Won, Bae) Use MainOSB
	//	CBitmap *	m_pTextAreaBackup;			// Backup of Indicator Text Area			(TextAreaBackupBitmap)

// Current Chart View Info
protected:
	BOOL		m_bLog;					// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	BOOL		m_bReverse;				// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	double		m_dViewMin;				// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double		m_dViewMax;				// View���� ��� Graph�� View�� Maximum Scale ���̴�.
	CRect		m_rctBlockRegion;
	CRect		m_rctGraphRegion;
	CRect		m_rctGraphDrwingRegion;
	int			m_nStartIndex;
	int			m_nEndIndex;


// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
protected:
	HCURSOR		m_hAPCursor;

// (2004.10.25, ��¿�) Bound���� �����ϴ� Interface�� �����Ѵ�.
//		Ȯ��� �����ڵ� ������ �޾� �����Ͽ� ��������ÿ� �̿��Ѵ�.
protected:
	CString			m_strItemCode;
protected:
	virtual void	ResetBound( const char *p_szBound);

// (2004.10.26, ��¿�) ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
protected:
	double		m_dPacketUnitValue;

// (2004.10.26, ��¿�) ��ȸ���� ȣ�� String�� �����Ͽ�, ���ý� �ٷ� ������ �߻��ϵ��� �Ѵ�.
protected:
	CString		m_strAskingPrice;
	CString		m_strRQ;		// 20081031 ����ǥ

protected:
	int				m_nMarketType;
	int				m_nRatioByYCP;			// (2004.11.02, ��¿�) �����(�����)���� �������� Flag�� �޾� �����Ѵ�.
	double			m_dYClosePrice;
// (2004.11.22, ��¿�) ���������� m_apiAskingPriceIndicatorAddInImp�κ��� �����Ͽ� CYChangeMark�� ������ ������� �Ѵ�.
protected:
	double			GetYClosePrice( void) const		{	return m_dYClosePrice;	}

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void);
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	virtual void	OnDrawAfterGraph(HDC p_hDC);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);

// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
protected:
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
		// Chart View Info
	BOOL	GetChartBasicData( void);


// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	// Tool Commands
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption );
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);
protected:
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
};

#endif // !defined(AFX_ORDERLINKADDINIMP_H__D201031A_2204_4FDF_B676_81AB5EAFB10C__INCLUDED_)
