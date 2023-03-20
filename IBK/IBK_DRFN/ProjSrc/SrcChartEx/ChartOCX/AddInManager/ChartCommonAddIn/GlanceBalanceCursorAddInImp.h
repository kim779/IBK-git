// GlanceBalanceCursorAddInImp.h: interface for the CGlanceBalanceCursorAddInImp class.
//
// The Add-In Implementation Class of the function which indicates Span at the mouse cursor
// when we use a "Glance-Balance Indicator".
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLANCEBALANCECURSORADDINIMP_H__CB9DBB09_6E08_4091_8AA7_169372144A63__INCLUDED_)
#define AFX_GLANCEBALANCECURSORADDINIMP_H__CB9DBB09_6E08_4091_8AA7_169372144A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"			// for CChartAddInBase
#include "../Include_AddIn/I000000/_IDoubleList.h"				// for IDoubleList



// Declare Interface ----------------------------------------------------------
interface IChartOCX;


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 04
// Comments		: "�ϸ����ǥ"�� ����� �� Mouse Cursor�� Span Data�� ǥ�����ִ�
//				: ����� Add-In�̴�.
// Using way	: �ٸ� AddIn�� ���� InitAddInDll()�� ���� Load�ȴ�.
// See			: CAskingPriceIndicatorAddInImp, CBoundMarkerAddInImp, CTimeMarkerAddInImp��
//				: �����Ͽ� ����.
//-----------------------------------------------------------------------------
class CGlanceBalanceCursorAddInImp : public CChartAddInBase  
{
// Constructor & Destructor
public:
	CGlanceBalanceCursorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase );
	virtual ~CGlanceBalanceCursorAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()


// Attributes
public:
	static CString m_strAddInItemName;		// Multiple Item in DLL
	
	CString m_strIndicatorName;				// Indicator Name 


// Attributes
protected:
	// a Flag about using "Span Cursor"
	BOOL m_bGlanceBalanceState;		// "�ϸ����ǥ"�϶� "Span" Data�� �����ٰ�����
											// �Ⱥ��� �� �������� �����Ѵ�.
	BOOL	m_bEnableGlanceBalanceCursor;	// (2006/10/17 - Seung-Won, Bae) Manage flag of Enable State.
	
	// a calculate condition of "Glance-Balance" Indicator
	int m_nPrevSpan;						// "���� Span" ��
	int m_nAfterSpan;						// "���� Span" ��
	
	int m_nStartIndex;						// Block���� ù Candle�� Index
	int m_nEndIndex;						// Block���� ������ Candel�� Index
	int m_nPrevSpanIndex;					// "���� Span"�� Index
	int m_nAfterSpanIndex;					// "���� Span"�� Index

	// a Region about Graph Block & Span Data rect
	CRect m_rctBlockRegion;					// View�� ��ü Graph �����̴�. (Block�� View �����̴�.)
	CRect m_rctGraphDrawingRegion;			// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)

	CPoint m_ptPrevSpan;					// "���� Span"�� ǥ���� ������ �߽� Point
	CPoint m_ptAfterSpan;					// "���� Span"�� ǥ���� ������ �߽� Point

	CString m_strPrevSpan;					// "���� Span"�� ���ڿ��� ����� ���� ����
	CString m_strAfterSpan;					// "���� Span"�� ���ڿ��� ����� ���� ����

	// a Compartment about X-Scale data
	CString m_strDateCompartment;			// XScale�� �ڷ����� ������
	CString m_strTimeCompartment;			// XScale�� �ð� ������

	CPoint	m_ptCursor;
	
// Methods
protected:
	BOOL GetGraphBlockRegion( void );

	BOOL GetSpanData( void );
	BOOL GetSpanTime( CPoint ptPoint );
	
	BOOL GetStringfromDataIdx( int nDataIdx, CString &strSpan );

	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName);
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);

	// Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void OnDrawAfterGraph( HDC p_hDC);		

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
	
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);

// Tool Interfaces
// Caution! If the Event is processed, it returns TRUE.
//	and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID);
protected:
	// Mouse cursor�� �̵��� ó���Ѵ�.
	virtual BOOL	OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	// �������� ���� �޴� ���� - ojtaso (20071025)
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
};

#endif // !defined(AFX_GLANCEBALANCECURSORADDINIMP_H__CB9DBB09_6E08_4091_8AA7_169372144A63__INCLUDED_)

