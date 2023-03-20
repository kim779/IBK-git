#pragma once
// CheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckBox window

#include "tool.h"
#include "mybitmap.h"
#include <afxtempl.h>

class CStockCheckBox : public CWnd
{
// Construction
public:
	CStockCheckBox();
	virtual ~CStockCheckBox();


// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DeleteTool();
	void ShowRow(int row);
	void SetArray();	
	void RemoveAll();
	DATA GetData(int row, int col);
	void AddData(int row, int col, const DATA& data);
	void DrawList(int row);
	int  GetInit(){ return m_nInit; }
	int  GetCount(){ return m_nCount; }
	int  GetScroll(){ return m_nScroll; }
	void SetScroll(int val){ m_nScroll = val; }
	void SetInit(int val){ m_nInit = val; }
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	void SetCount(int ct){ m_nCount = ct; if(m_nCount == 25) m_nColCount = 8;}	//2014.08.27 KSJ ���������� �ٲ��� �ʾ����� ó��
	void SetWnd(CWnd *pWnd){ m_pWnd = pWnd; }
	DWORD SetCheckCursor(int nCursorId, BOOL bRepaint = TRUE);
	void SearchStock(CString strName, int nNext);								//2015.09.09 KSJ ����ġ(0:����, 1:������, -1:�ڷ�)
	
	// Generated message map functions
protected:

	void ResetScrollBars();
	void Draw();
	
protected:
	void SetMonth(int row);		//2014.07.29 KSJ ������ ������ �����ش�.
	bool m_bSpread{};		//2014.07.29 KSJ �����Ѱ� ������������
	int m_nSFSelect{};		//2014.07.29 KSJ ������ ������ ���°����.
	int m_nSPSelect{};		//2014.07.29 KSJ ������ ������ ���°����.
	int m_nColCount{};		//2014.08.27 KSJ ���������� �ٲ��� �ʾ������� 8�� �ٲ�������� 14��
	int m_nSearchRow{};		//2014.08.27 KSJ �˻��߶��� ������ �������ֱ� ����

	int m_nArrSearch[50]{};		//2015.09.09 KSJ �˻��� �ε��� ����
	int m_nCurrIndex{};		//2015.09.09 KSJ ���� �ε���

	CMyBitmap	m_BitmapNormal;
	CMyBitmap	m_BitmapClick;
	std::unique_ptr<CTool> m_pTool{};
	
	int		m_nInit{};		// �׷����� ��ġ ..
	int		m_nCount{};		// �÷� ��...
	int		m_nScroll{};		// ��ũ���� ��ġ..
	
	CWnd* m_pWnd{};

	int		m_Key{};
	HCURSOR		m_hCursor{};

	CString		m_sCode;

	CMap<int, int, DATA, DATA>	m_Map;
	CStringArray			m_Array;

	//{{AFX_MSG(CCheckBox)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

