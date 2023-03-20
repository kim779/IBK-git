// AskingPriceIndicatorAddInImp.h: interface for the CAskingPriceIndicatorAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASKINGPRICEINDICATORADDINIMP_H__D927AE83_DA1F_4030_B127_B0F228D6D864__INCLUDED_)
#define AFX_ASKINGPRICEINDICATORADDINIMP_H__D927AE83_DA1F_4030_B127_B0F228D6D864__INCLUDED_

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/3/10
// Comments		: 
// Using way	: 
// See			: 
//
// Help Message Reference : BOUNDMARK
//	{{{
//		BOUNDMARK : ������Ʈ�� ������ ���� Scale�� �ش� ������ �����Ѽ��� ǥ���ϵ��� �˷��ִ� ���� �޽���
//			[Type I] CBoundMarker Class Header ����.
//			[Type II] ȣ�� ���� ���
//				BOUNDMARK=GN:UB,LB:IC:MT:YC:RT
//					IC = �����ڵ�
//					MT = �屸�� (���� �ɼ� ��� ����)
//						KOSPI		1
//						KOSDAQ		2
//						ETF_KOSPI	7	
//						ETF_KOSDAQ	9
//					YC = ��������
//					RT = ����� Chart ���� Code (1:����, 2:�����)
//	}}}
//-----------------------------------------------------------------------------

// (2004.10.22, ��¿�) ��/���Ѱ� ǥ�ñ�ɿ� ȣ������ ����� ������ Code Class�̴�.
/*
[Asking Price Indicator]

1. Action List
	On Mouse Over
		- Draw Line & Text (Value)
	On Mouse Move
		- Clear Previous Line & Text (Value)
		- Draw Next Line & Text (Value)
	On Mouse Out
		- Clear Previous Line & Text (Value)
	On Draw
		if On Mouse Over
			- Draw Line & Text (Value)

2. Drawing Method for Clear
	Line -> XOR Line
	Text -> Original Image Backup & Drawing

3. Action Flow
			On Draw
	Start	->	On Mouse Over
	Action	->	On Mouse Move
			On Draw (with New Graph)
	End	->	On Mouse Out
			On Draw
4. Event Routine
x	On Draw
x		- Draw BoundMark (Check Reverse)
x		- Set IndicatorRegion
x		- Check, SetCaptureFlag TRUE ~ Process as New Mouse Move
x			CALL ReleaseCaptureWithFlag
x			Get Mouse Position & State (GetCursorPos, NULL Flag)
x			CALL Event On Mouse Move
x	On Mouse Move
x		- Check, AskingPriceGShareFireFlag TRUE
x				~ It must be Pre-Process before Out-Indicator-Region-Process for Jumping to Out after Fire
x			CALL ClearAskingPriceGShareFireFlag
x		- Check, In Indicator Region (CRect::PtInRect)
x			- Check, SetCaptureFlag FALSE & No Mouse Flag & No Special Cursor (OCX's Get Cursor Handle)
x				CALL SetCaptureWithFlag for Mouse Out
x				CALL ShowNewIndicator
x			- Check, SetCaptureFlag
x				CALL ClearPreviousIndicatorDraw
x				CALL ShowNewIndicator
x		- Check, Out Indicator Region & SetCaptureFlag TRUE
x			CALL ClearPreviousIndicatorDraw
x			CALL ReleaseCaptureWithFlag
x	On LButtonDown
x		- Check, In Indicator Region & SetCaptureFlag TRUE ~ for Click Cursor
x			Set Yellow Diamond Cursor (LoadCursor, OCX's Set Cursor Handle)
x			Set AskingPriceGShareFireFlag TRUE
x			Fire AskingPriceGShare with IndicatedPrice
x	On LButtonUp
x		- Check, AskingPriceGShareFireFlag TRUE
x			CALL ClearAskingPriceGShareFireFlag

5. Library Routine
x	SetCpatureWithFlag
x		Set Capture
x		Set SetCaptureFlag TRUE
x		Set White Diamond Cursor (LoadCursor, OCX's Set Cursor Handle)
x	ReleaseCaptureWithFlag
x		Set SetCaptureFlag FALSE
x		Release Capture
x		Clear White Diamond Cursor (OCX's Set Cursor Handle NULL)
x		Release Backup Buffer (Check NOT NULL, DeleteObject, delete, NULL)
x		Set AskingPriceGShareFireFlag FALSE
x	ClearPreviousIndicatorDraw
x		Draw NOT Line on IndicatorPosition
x		Restore Backup Image to IndicatorTextArea [See 'Capturing an Image' of MSDN]
x	ShowNewIndicator
x		Set IndicatorPosition
x		Calculate New Indicator Value
x		Set IndicatorTextArea (GetTextExtent)
x		Backup New Indicator Drawing Area (Check Backup Buffer, Create on None)
x			[See 'Capturing an Image' of MSDN]
x		Draw New Indicator (Draw NOT Line, Draw Text with Rectangle)
x	ClearAskingPriceGShareFireFlag
x		Set AskingPriceGShareFireFlag FALSE
x		Clear Yellow Diamond Cursor (Load White Diamond Cursor, OCX's Set Cursor Handle)

6. Variable & Initializing
	BOOL		m_bOnSetCaprture		= FALSE;	~ Indicator Drawing Situation Flag	(SetCaptureFlag)
	int		m_nPreIndicatorPosY		= 0;		~ Last Drawing Position			(IndicatorPosition)
	CRect		m_rctPreIndicatorTextArea			~ Last Text Drawing Area		(IndicatorTextArea)
	CRect		m_rctIndicatorRegion				~ Indicator Region of BoundMark		(IndicatorRegion)
	CBitmap *	m_pTextAreaBackup		= NULL;		~ Backup of Indicator Text Area		(TextAreaBackupBitmap)
	BOOL		m_bOnFireAskingPriceGShare	= FALSE;	~ Asking Price GShare Firing Flag	(AskingPriceGShareFireFlag)
	double		m_dIndicatedPrice		= 0.;		~ Indicated Price			(IndicatedPrice)
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoundMarkerAddInImp.h"

interface IChartManager;
class CAskingPriceIndicatorAddInImp : public CBoundMarkerAddInImp
{
public:
	CAskingPriceIndicatorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CAskingPriceIndicatorAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.10.25, ��¿�) ȣ�������� ���� Member Variable�� �����Ѵ�.
protected:
	int			m_nIndicatorPosY;				// Current Indicator Drawing Position			(IndicatorPosition)
	BOOL		m_bOnSetCaprture;				// Indicator Drawing Situation Flag			(SetCaptureFlag)
	BOOL		m_bOnFireAskingPriceGShare;		// Asking Price GShare Firing Flag			(AskingPriceGShareFireFlag)
	CRect		m_rctAskingPriceMark;			// Indicator Region of BoundMark			(IndicatorRegion)
	double		m_dIndicatedPrice;				// Indicated Price							(IndicatedPrice)
	// (2006/11/5 - Seung-Won, Bae) Use MainOSB
	//	CBitmap *	m_pTextAreaBackup;			// Backup of Indicator Text Area			(TextAreaBackupBitmap)

// (2004.10.25, ��¿�) ȣ�������� ���� Library Routine�� �����Ѵ�.
protected:
	// (2004.10.25, ��¿�) ȣ������ ó�� Mark�� Clear��Ű�� Routine�� �����Ѵ�.
	void	ClearAskingPriceGShareFireFlag( void);

// (2004.10.25, ��¿�) SetCapture()��, ���� �����ϴ� Cursor�� �����ϸ鼭,
//		ReleaseCapture()�� ������ ���÷� OCX�� Cursor�� �����Ѵ�.
//		(�ٸ� �м��������� ���÷� ������ �����Ͽ� �߰����� ������ �� ���� �ʴ´�. ������ ��...)
protected:
	HCURSOR		m_hAPCursor;

// (2004.10.25, ��¿�) Bound���� �����ϴ� Interface�� �����Ѵ�.
//		Ȯ��� �����ڵ� ������ �޾� �����Ͽ� ��������ÿ� �̿��Ѵ�.
protected:
//	CString			m_strItemCode;
protected:
	virtual void	ResetBound( const char *p_szBound);

// (2004.10.26, ��¿�) ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
protected:
	double		m_dPacketUnitValue;

// (2004.10.26, ��¿�) ��ȸ���� ȣ�� String�� �����Ͽ�, ���ý� �ٷ� ������ �߻��ϵ��� �Ѵ�.
protected:
	CString		m_strAskingPrice;

// (2004.11.01, ��¿�) �屸�а� ��������, ȣ������ DLL�� �����Ͽ�, ȣ�� ������ �̿��Ѵ�.
//				FnGetPriceUnit( �屸��,  ����,  ��������,  ����/���������[nFlag 1:����, 2:�����] )
//					Error Value -99999999
#define _FnGetPriceUnit_ERROR		-99999999
typedef double ( *FnGetPriceUnit)( int nJongMokGubun, double fValue, double fPreClosePrice, int nFlag);
protected:
	int				m_nMarketType;
	int				m_nRatioByYCP;			// (2004.11.02, ��¿�) �����(�����)���� �������� Flag�� �޾� �����Ѵ�.
	double			m_dYClosePrice;
	HMODULE			m_hAskingPriceFixDLL;
	FnGetPriceUnit	m_fnFnGetPriceUnit;
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
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);

// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
protected:
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_ASKINGPRICEINDICATORADDINIMP_H__D927AE83_DA1F_4030_B127_B0F228D6D864__INCLUDED_)
