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
#include "LineObj.h"

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

#define OL_FILE	"OrderLine.cfg"	// �ֹ��� ���� ���ϸ�

class CChartOrderLineAddInImp : public CChartAddInBase
{
public:
	double m_prevLastData;
	static CString m_strAddInItemName;	// Add-in �̸�
	virtual BOOL InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);

	CChartOrderLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartOrderLineAddInImp();

private:
	CString m_strDateType;			// ���غ�
	double m_dLastValue;	
	CString m_strToolLoading;
	CPoint m_ptInit;				// �ʱ� ��ġ(���콺 Ŭ��)
    CString m_strCode;              // �����ڵ�
	CString m_strCodeName;			// �����
	POINT m_ptOffSet;				// �׷��� ���� offset

    int m_nCommandType;             // ��� Ÿ��
	BOOL m_bCentered;				// ĵ�� ��� ����
	HMENU m_hPopupMenu;				// popup menu, Ŭ�� �� Ȱ��ȭ�Ǵ� �޴�
	HDC m_hDC;
	BOOL m_bNewDraw;				// �׸���/���� ���, TRUE:�׸���, FALSE:����
	POSITION m_PosForRemove;		// ������ �ʿ�

	DWORD m_dwMouseActionID;
	
	CObList m_LineList;				// �ֹ��� ����Ʈ
	CLineObj* m_pCurSelectedLine;	// ���õ� �ֹ���

	BOOL m_bLog;					// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	BOOL m_bReverse;				// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	double m_dViewMin;				// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double m_dViewMax;				// View���� ��� Graph�� View�� Maximum Scale ���̴�.
	CRect m_rctBlockRegion, m_rctGraphRegion ;
	CRect m_rctGraphDrwingRegion;
	int m_nStartIndex, m_nEndIndex;
	CPoint m_ptCursor;				// ���콺 Ŭ�� ��ġ
	int m_nDateType;				// ��¥ Ÿ��, ���غ�
	CStringList m_AllOrderLines;	// ����� ��� �ֹ���

	void LoadOrderLine();			// ����� �ֹ��� �ҷ�����
	void OnOLConfig();				// �ֹ��� ����
	void OnOLMan();					// �ֹ��� ����
	void OnDeleteLine();			// �ֹ��� ���� & �޸�(ȭ��) ����
	void OnDeleteLine2();			// �ֹ��� �޸�(ȭ��) ����
	void OnReleaseDraw();			// ���� �׸��� ����
	int GetIndexFromValue(long lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT*/);
	int GetPointFromIndex(int x/*IN*/, int y/*IN*/, CPoint& ptIndex/*OUT*/);
	int GetValueFromIndex(int ix/*IN*/, int iy/*IN*/, long& lx/*OUT*/, double&dy/*OUT*/);
	int GetIndexFromPoint(CPoint& pt/*IN,OUT*/, int& x/*OUT*/, int&y/*OUT*/);
	BOOL GetValueFromPoint(CPoint& ptAt/*IN,OUT*/, long &x/*OUT*/, double &y/*OUT*/);
	int GetYPositionOnLine(int& x/*IN,OUT*/, CPoint pt1/*IN*/, CPoint pt2/*IN*/);
	void UnregPS(long pLine/*IN*/);				// addin manager���� ������Ʈ ��� ����
	void RegPS(long pLine/*IN*/);				// ������Ʈ addin manager�� ���

	BOOL GetChartBasicData(	int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion);
	BOOL IsValidIndex(int nIndex);	// �־��� �ε����� ��ȿ�� �ε������� ���� �Ǻ�
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);	// ��Ʈ���� ����
	long UPOrDown(CLineObj* pLine, double dData);

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()
protected:
	virtual void OnDrawAfterGraph(HDC p_hDC);
	virtual BOOL OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnResetChart(void);
	virtual void OnLoadEnvironment(IPropertyMap *p_pIAddInEnvMap);	// ���� ȣ���
	virtual void OnSaveEnvironment(IPropertyMap *p_pIAddInEnvMap);	// ���� �� ȣ���
	virtual RECT DrawCursor(HDC p_hDC);								// ���콺�� �̵� �߿� (�׸��� �ϴ� ����) ȣ���
	virtual void OnPacketData(const char *p_szPacketNames, BOOL p_bWithReset);	// �����Ͱ� ��� �� �� ȣ���
	virtual void OnUpdatePacketData(BOOL p_bTrUpdate);
	virtual BOOL OnAddInToolCommand(const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL OnAddInToolCommand(const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual void OnRealData( const char *p_szPacketName, double p_dData);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);
};

#endif // !defined(AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
