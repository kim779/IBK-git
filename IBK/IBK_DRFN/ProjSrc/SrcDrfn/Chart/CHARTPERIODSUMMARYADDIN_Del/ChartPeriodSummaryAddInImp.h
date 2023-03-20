// ChartStockOpinionAddInImp.h: interface for the CChartPeriodSummaryAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/IPropertyMap.h"

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
	int m_nDateType;
	HDC m_hDC;
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
	BOOL IsValidIndex(int nIndex/*IN*/);		// �ǹ� �ִ� �ε��� ���� �Ǵ�
	int GetDateType();							// ���� ���� ���, �Ｚ���� ������
	void OnDeleteLine();						// �Ⱓ ��� ����
	void OnPSConfig(CPeriodObj* pLine/*IN*/);	// �Ⱓ��� ���� ���
	void OnReleaseDraw();						// ���� �׸��� ���� ���
	int GetPriceInfo(CPeriodObj::CPrice* pPriceInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);				// �������� ���
	int GetCandleInfo(CPeriodObj::CCandle* pCandleInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//�� ���� ���
	int GetAmountInfo(CPeriodObj::CAmount* pAmountInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//�ŷ��� ���� ���
	int GetIndexFromValue(long lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT, N/A*/);		// �� --> �ε���
	int GetPointFromIndex(int x/*IN*/, int y/*IN*/, CPoint& ptIndex/*OUT*/);						// �ε���  --> ��ġ ���� 
	int GetValueFromIndex(int ix/*IN*/, int iy/*IN*/, long& lx/*OUT*/, double&dy/*OUT*/);			// �ε��� --> ��
	int GetIndexFromPoint(CPoint& pt/*IN,OUT*/, int& x/*OUT*/, int&y/*OUT*/);						// ��ġ ���� --> �ε���
	//BOOL GetValueFromPoint(CPoint& ptAt/*IN,OUT*/, long &x/*OUT*/, double &y/*OUT*/);				// ��ġ ���� --> ��
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN,OUT*/);	// ��Ʈ�� tokenizing, �����ϴٸ� MFC �Ǵ� SDK �Լ��� ��ü
	BOOL GetChartBasicData(	int &p_nStartIndex/*OUT*/, int &p_nEndIndex/*OUT*/, int &p_nR/*OUT*/, int &p_nC/*OUT*/, CRect &p_rctBlockRegion/*OUT*/, CRect &p_rctGraphRegion/*OUT*/);	// ��Ʈ�� �⺻ ���� ���
	BOOL GetSpanTime( CPoint ptPoint/*IN*/);	
	BOOL GetStringfromDataIdx( int nDataIdx/*IN*/, CString &strSpan/*OUT*/);

	
	CChartPeriodSummaryAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartPeriodSummaryAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

	virtual BOOL InvokeAddIn( int p_nCommandType, long p_lData);	// ������� ����
		
protected:
	virtual void OnDrawAfterGraph( HDC p_hDC);		// ��Ʈ���� �� �ʿ� �׸� ��
	virtual void OnDrawBeforeGraph( HDC p_hDC);		// ��Ʈ���� �� �ʿ� �׸� ��
	virtual BOOL OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI);	// ON COMMAND�� ���õ� �׼� ó��
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);	// �����Ͱ� ��� �� �� ȣ���
	virtual void OnResetChart( void);	// ��Ʈ�� ���µ� �� ȣ���
	virtual BOOL OnAddInToolCommand(const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual void OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);	// ���� ȣ���
	virtual void OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);	// ���� �� ȣ���
	virtual RECT DrawCursor( HDC p_hDC);	// ���콺�� �׸��� �ϴ� ���� ȣ���
	virtual void OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);


	CObList m_LineList;

	int GetData_X(const CPoint& point, long& lx);	// XScale : ���� Interface�� ��� - vntsorl(20070521)
private:
	BOOL m_bLog;						// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	BOOL m_bReverse;					// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	CRect m_rctBoundMark;
	double m_dViewMin;					// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double m_dViewMax;					// View���� ��� Graph�� View�� Maximum Scale ���̴�.
	CRect m_rctBlockRegion, m_rctGraphRegion ;	// �׸��� ���� ����
	CRect m_rctGraphDrwingRegion;
	int m_nStartIndex, m_nEndIndex;		// ȭ�� ���� ó�� �� �ε���
	CPoint m_ptPrevSpan;					// "���� Span"�� ǥ���� ������ �߽� Point
	CPoint m_ptAfterSpan;					// "���� Span"�� ǥ���� ������ �߽� Point

	int m_nPrevSpanIndex;					// "���� Span"�� Index
	int m_nAfterSpanIndex;					// "���� Span"�� Index
	int m_nPrevSpan;						// "���� Span" ��
	int m_nAfterSpan;						// "���� Span" ��

	CString m_strDateCompartment;			// XScale�� �ڷ����� ������
	CString m_strTimeCompartment;			// XScale�� �ð� ������

};

#endif // !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
