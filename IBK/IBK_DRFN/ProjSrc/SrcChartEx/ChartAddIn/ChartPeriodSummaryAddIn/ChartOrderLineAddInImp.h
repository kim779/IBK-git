// ChartOrderLineAddInImp.h: interface for the CChartOrderLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/IPropertyMap.h"

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

#define OL_FILE	"OrderLine.cfg"	// �ֹ��� ���� ���ϸ�

class CLineObj;
typedef CTypedPtrList< CObList, CLineObj *>  COLList;

class CChartOrderLineAddInImp : public CChartAddInBase
{
// AddIn ID
public:
	static CString m_strAddInItemName;

// Constructor & Destructor
public:
	CChartOrderLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartOrderLineAddInImp();

// Current Chart Data Info
protected:
    CString		m_strCode;              // �����ڵ�, Item Code
	CString		m_strCodeName;			// �����, Item Name
	CString		m_strDateType;			// ���غ�, Current Date/Time Type String => ��/��/��/��/ƽ
	int			m_nDateType;			// ��¥ Ÿ��, ���غ�
	CString		m_strRQ;				

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

// Current Tool State
protected:
    int			m_nCommandType;			// ��� Ÿ��, Tool State Mode
										// 0: Not Initialized.
										// 1: Drawing Mode of new Object
										// 2: Selection Mode (Selected or not)
	DWORD		m_dwMouseActionID;		// Current Mouse Action ID
	CLineObj *	m_pCurSelectedLine;		// ���õ� �ֹ���, Selected Object

// Tool Object Data
protected:
	COLList		m_LineList;						// �ֹ��� ����Ʈ

// (2010/4/17 - Seung-Won, Bae) Make it static to share the Order Line Info within all Chart.
public:
	static CStringList											m_slSavedOrderLines;		// ����� ��� �ֹ���
	static CTypedPtrList< CObList, CChartOrderLineAddInImp *>	m_alOrderLineAddInManager;
public:
	COLList *	GetLineList( void)			{	return &m_LineList;		}
	void		InvalidateControl( void)	{	if( m_pIChartOCX) m_pIChartOCX->InvalidateControl();	}

// Chart Data and View Base Info Collecting
protected:
	// Chart Data Info
	//		m_strCode, m_strCodeName		- InvokeAddInStr(), It is called in MChart TR Data Changing.
	//		m_strDateType, m_nDateType		- OnPacketDataMultiItem()
	// Chart View Info
	BOOL	GetChartBasicData( void);
			// OnDrawAfterGraph()

// Current Tool State
protected:
	// Selection Change
	CLineObj *	ChangeSelection( CLineObj *p_pNewSelection);
				// ChangeToolMode()											- Release Selection for new Mode
				// DeleteSelection()										- Release Selection for delete
				// DeleteAll()												- Release Selection for delete all
				// OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_THIS_TOOL)	- Change Selection for delete
				// OnLButtonDown()											- Select new Creation
				// SelectLine()												- new Selection
				
	// Tool Mode Change
	BOOL		ChangeToolMode( int p_nNewMode);
				// OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, T_ORDER_LINE,)				- Start/End Drawing Mode
				// OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, T_TOOL_SELECTION,)			- Release Drawing Mode
				// OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, T_ALL_TOOL_RESET_STATE,)	- Release Drawing Mode
				// OnResetChart()															- Release Drawing Mode
				// OnCmdMsg( ID_KSA_ORDERLINE_RELEASE)										- Release Drawing Mode by context menu

// Draw
protected:
	//	OnDrawAfterGraph()
	//	DrawCursor()

// Create
protected:
	// It is processed with 4 Step.
	//	1. LButton Down						- OnLButtonDown()
	//  2. Mouse Move with LButton Down		- OnMouseMove()
	//	3. LButton Up						- OnLButtonUp()
	//	4. Config the Object.				- in OnLButtonUp()

// Select
protected:
	BOOL	SelectLine( POINT &p_pt, DWORD p_dwMouseActionID);
			// OnLButtonDown()
			// OnRButtonDown()

// Modify
protected:
	// It is processed with 3 Step.
	//	1. LButton Down						- OnLButtonDown()
	//  2. Mouse Move with LButton Down		- OnMouseMove()
	//	3. LButton Up						- OnLButtonUp()

// Delete
protected:
	// Delete Selected Object
	BOOL	DeleteSelection( BOOL p_bUnregisterOrderManager = TRUE);
			// OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_THIS_TOOL, "ORDER_LINE", ...)	- Delete by order
			// OnLButtonUp()																- Empty Object
			// OnCmdMsg( ID_KSA_ORDERLINE_DELETE, )											- Delete by Menu
			// OnKeyDown( VK_DELETE, )														- Delete by 'Del' Key
			// OnOLConfig()																	- Canceled new Object
			// OnDeleteLine()																- Delete with file by Menu
	// Delete All Object
	BOOL	DeleteAll( void);
			// ~CChartOrderLineAddInImp()
			// OnAddInToolCommand( T_DELETE_ALL)	- Delete all by Tool Command
			// LoadOrderLine()						- Delete for new Loading.

// Save and Load
protected:
	// OnDeleteLine()	- Delete Line and Save File directly for context menu.
	// OnOLConfig()		- Add new Line with save Btn of dialog.
	// OnOLMan()		- Delete Lines of Item and Save Files directly.
	// LoadOrderLine()	- Reload Lines in TR and Reset Chart.

// Register and Unregister to order manager
protected:
	void	RegPS(		long pLine/*IN*/);				// ������Ʈ addin manager�� ���
			// OnLButtonDown()	- new Object
			// LoadOrderLine()	- Loaded Object
	void	UnregPS(	long pLine/*IN*/);				// addin manager���� ������Ʈ ��� ����
			// DeleteSelection()	- Deletion by not order manager.
			// DeleteAll()			- Remove all

protected:
	void LoadOrderLine();			// ����� �ֹ��� �ҷ�����
	void OnOLConfig();				// �ֹ��� ����
	void OnOLMan();					// �ֹ��� ����
	void OnDeleteLine();			// �ֹ��� ���� & �޸�(ȭ��) ����

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//int GetIndexFromValue(long lx/*IN*/, int& ix/*OUT*/);
	int GetIndexFromValue(__int64 lx/*IN*/, int& ix/*OUT*/);
	// 2011.01.06 by SYS <<

	long GetXPosition( int x);
	
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//int GetValueFromIndex(int ix/*IN*/, long& lx/*OUT*/);
	int GetValueFromIndex(int ix/*IN*/, __int64& lx/*OUT*/);
	// 2011.01.06 by SYS <<

	int GetIndexFromPoint(CPoint& pt/*IN,OUT*/, int& x/*OUT*/);
	int GetYPositionOnLine(int& x/*IN,OUT*/, CPoint pt1/*IN*/, CPoint pt2/*IN*/);

	BOOL IsValidIndex(int nIndex);	// �־��� �ε����� ��ȿ�� �ε������� ���� �Ǻ�
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);	// ��Ʈ���� ����
	void	OnUpdatePacketDataMultiItem2( IPacket *pIPacketForCur);

	// 20081007 JS.Kim	����ڽð��� ����
	bool	RecalcLineXPoint(const char* szName, int nTimeDiff);

	CPoint m_ptCursor;				// ���콺 Ŭ�� ��ġ

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);

// Event Overriding
protected:
	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2004.10.11, ��¿�) AddIn DLL�� OCX Event�� �����Ѵ�.
protected:
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);

	CDialog* m_pDlgConfirm;		//@Solomon100319
};

#endif // !defined(AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
