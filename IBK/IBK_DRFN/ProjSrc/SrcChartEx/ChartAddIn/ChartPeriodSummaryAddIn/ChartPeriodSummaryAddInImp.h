// ChartStockOpinionAddInImp.h: interface for the CChartPeriodSummaryAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_Chart/IPropertyMap.h"
#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_AddIn/I133691/_IPacketListManager.h"			// for IPacketListManager


#include "PeriodObj.h"

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

class CChartPeriodSummaryAddInImp : public CChartAddInBase
{
public:
	CString m_strToolLoading;
	BOOL m_bLoadData;
	int m_nMouseClickStatus;
	CPoint m_ptInit;
	BOOL m_bDrag;
	BOOL m_bMouseCaptured;
	CPoint m_ptCursor;
	POINT m_ptOffSet;
//	int m_nDateType;
	HMENU m_hPopupMenu;				// �Ⱓ��༱ Ŭ�� �� Ȱ��ȭ�Ǵ� �޴� �ڵ�
	int m_nCommandType;				// ��� ����
	BOOL m_bCentered;				// ���� ��� ���� ����
	BOOL m_bAttachData;				// N/A
	BOOL m_bNewDraw;				// �׸��� ��忡�� ���콺 L ��ư Ŭ�� �� TREU, �� �ܴ� FALSE
	CPeriodObj* m_pCurSelectedLine;	//���� ���õ� �Ⱓ��� ������Ʈ
	POSITION m_PosForRemove;
	DWORD m_dwMouseActionID;
	static CString m_strAddInItemName;

//	CBitmap* m_pBmp;
//	CDC m_dcMem;
//	CDC m_dcTranMem;	// ��� ����� ����
//	CDC m_dcMemText;	// �ؽ�Ʈ��

	void UnregPS(long pLine/*IN*/);				// ������Ʈ addin manager�� ���
	void RegPS(long pLine/*IN*/);				// addin manager�� ��ϵ� ������Ʈ ��� ����
	BOOL IsValidIndex( const char *p_szRQ, int nIndex/*IN*/);		// �ǹ� �ִ� �ε��� ���� �Ǵ�
	int GetDateType( CPeriodObj *p_pLine);		// ���� ���� ���, �Ｚ���� ������
	void OnDeleteLine();						// �Ⱓ ��� ����
	void OnPSConfig(CPeriodObj* pLine/*IN*/);	// �Ⱓ��� ���� ���
	void OnReleaseDraw();						// ���� �׸��� ���� ���
	int GetPriceInfo( const char *p_szRQ, CPeriodObj::CPrice* pPriceInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);				// �������� ���
	int GetCandleInfo( const char *p_szRQ, CPeriodObj::CCandle* pCandleInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//�� ���� ���
	int GetAmountInfo( const char *p_szRQ, CPeriodObj::CAmount* pAmountInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//�ŷ��� ���� ���
	
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//int GetIndexFromValue( const char *p_szRQ, int p_nEndIndex, long lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT, N/A*/);		// �� --> �ε���
	int GetIndexFromValue( const char *p_szRQ, int p_nEndIndex, __int64 lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT, N/A*/);		// �� --> �ε���
	// 2011.01.06 by SYS <<

	int GetPointFromIndex( const char *p_szRQ, int x/*IN*/, int y/*IN*/, CPoint& ptIndex/*OUT*/);						// �ε���  --> ��ġ ���� 
//	int GetValueFromIndex(int ix/*IN*/, int iy/*IN*/, long& lx/*OUT*/, double&dy/*OUT*/);			// �ε��� --> ��
	int GetIndexFromPoint( const char *p_szRQ, CPoint& pt/*IN,OUT*/, int& x/*OUT*/, int&y/*OUT*/);						// ��ġ ���� --> �ε���
	//BOOL GetValueFromPoint(CPoint& ptAt/*IN,OUT*/, long &x/*OUT*/, double &y/*OUT*/);				// ��ġ ���� --> ��
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN,OUT*/);	// ��Ʈ�� tokenizing, �����ϴٸ� MFC �Ǵ� SDK �Լ��� ��ü
	BOOL GetChartBasicData(	CPeriodObj* p_pLine);	// ��Ʈ�� �⺻ ���� ���
//	BOOL GetSpanTime( CPoint ptPoint/*IN*/);	
//	BOOL GetStringfromDataIdx( const char *p_szRQ, int nDataIdx/*IN*/, CString &strSpan/*OUT*/);

	
	CChartPeriodSummaryAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartPeriodSummaryAddInImp();

	CObList m_LineList;

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//int GetData_X(const CPoint& point, long& lx);	// XScale : ���� Interface�� ��� - vntsorl(20070521)
	int GetData_X(const CPoint& point, __int64& lx);	// XScale : ���� Interface�� ��� - vntsorl(20070521)
	CString DoubleToString(const double& Num);
	// 2011.01.06 by SYS <<

private:
//	BOOL m_bLog;						// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
//	BOOL m_bReverse;					// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	CRect m_rctBoundMark;
//	double m_dViewMin;					// View���� ��� Graph�� View�� Minimun Scale ���̴�.
//	double m_dViewMax;					// View���� ��� Graph�� View�� Maximum Scale ���̴�.
//	CRect m_rctBlockRegion, m_rctGraphRegion ;	// �׸��� ���� ����
//	CRect m_rctGraphDrwingRegion;
//	int m_nStartIndex, m_nEndIndex;		// ȭ�� ���� ó�� �� �ε���
	CPoint m_ptPrevSpan;					// "���� Span"�� ǥ���� ������ �߽� Point
	CPoint m_ptAfterSpan;					// "���� Span"�� ǥ���� ������ �߽� Point

	int m_nPrevSpanIndex;					// "���� Span"�� Index
	int m_nAfterSpanIndex;					// "���� Span"�� Index
	int m_nPrevSpan;						// "���� Span" ��
	int m_nAfterSpan;						// "���� Span" ��

	CString m_strDateCompartment;			// XScale�� �ڷ����� ������
	CString m_strTimeCompartment;			// XScale�� �ð� ������

public:
	// 20081009 �ڵ��� >>
	IPacketListManager *	GetPacketListManager( void)
							{
								if( !m_pIPacketListManager) return NULL;
								m_pIPacketListManager->AddRef();
								return m_pIPacketListManager;
							}
	// 20081009 �ڵ��� <<
	// 20081007 JS.Kim	����ڽð��� ����
	bool	RecalcLineXPoint( const char* szName, int nTimeDiff);


// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()
		
// (2004.10.11, ��¿�) AddIn DLL�� OCX Event�� �����Ѵ�.
protected:
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
	// [04/10/29] Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	virtual void	OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB);

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
};

#endif // !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
