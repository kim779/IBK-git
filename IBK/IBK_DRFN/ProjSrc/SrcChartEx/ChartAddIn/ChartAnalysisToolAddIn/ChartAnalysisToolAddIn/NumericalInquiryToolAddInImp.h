// NumericalInquiryToolAddInImp.h: interface for the CNumericalInquiryToolAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMERICALINQUIRYTOOLADDINIMP_H__385F097B_E6E8_40F1_BFF7_30F4087BABAE__INCLUDED_)
#define AFX_NUMERICALINQUIRYTOOLADDINIMP_H__385F097B_E6E8_40F1_BFF7_30F4087BABAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"		// for Ixxxxxx.h

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/28
// Comments
//
//	1. Tool Type with Crosshairs and Numerical Inquiry DLG (NIDLG) with Z-Order
//			.......... (Look header of Drag NIDLG AddIn Tool Object Class)
//	2. AddIn Object and Co-Operations.
//			.......... (Look header of Drag NIDLG AddIn Tool Object Class)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/7/3
// Comments		: Numercial Inquiry Tool AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CInquiryDataWindowDlg;
class CNumericalInquiryToolAddInImp : public CChartAddInBase
{
public:
	CNumericalInquiryToolAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CNumericalInquiryToolAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed for Capture/Path on MouseMove
protected:
	BOOL	m_bOnToolExecuted;
// (2006/6/28 - Seung-Won, Bae) Manage Notify from Over-NIDLG to Drag-NIDLG for avoiding the Multiple NIDLG Active.
protected:
	BOOL	m_bOverActivated;
	HWND	m_hOcxWnd;
	
// (2006/6/27 - Seung-Won, Bae) Manage Numerical Inquiry Dialog
protected:
	CInquiryDataWindowDlg *	m_pNumericalInquiryDlg;	// ��ġ��ȸâ ���̾�α�.


// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
	virtual void	OnResetMainBlock( void);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_NUMERICALINQUIRYTOOLADDINIMP_H__385F097B_E6E8_40F1_BFF7_30F4087BABAE__INCLUDED_)
