#if !defined(AFX_FLOATINGHOGADLG_H__21539FF9_3BC1_42EA_887C_7A7F9BAB708D__INCLUDED_)
#define AFX_FLOATINGHOGADLG_H__21539FF9_3BC1_42EA_887C_7A7F9BAB708D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatingHogaDlg.h : header file
//
#include "Resource.h"
#include "./define/ChartMsgDef.h"
#include "./control/RscDlg.h"
// By Jeon Yongbae  2008/07/09
//<<20100308_JS.Kim �ַθ�
//#include "../../../common/CommHeaderDef.h"
//>>

#include "../chart_common/PowLabel.h"

#include "../chart_common/Grid/Util.h"
//#include "../chart_common/Grid/GridCtrl.h"
#include "../chart_common/Grid/TR_drds/HogaData.h"
//#include "../chart_common/Grid/TR_drds/D49890.h"
#include "../../inc/ExGridCtrl.h"
#include "../../inc/ExOCXDLL.h"
#include "../chart_common/ILButton.h"

#include "../CommonTR/DrdsS31.h"
//<<20100308_JS.Kim �ַθ�
#include "../CommonTR/REAL_CAH1_GNF.h"
//>>
//#ifndef __DATA_UTIL_H
//	#include "../../COMMON_BASE\Drds\DataUtil.h"
//#endif

//#include "../../../DrMLib/DrMiddleLib.h"
//#include "../../COMMON_BASE/drds/DRDSDEF.h"

/////////////////////////////////////////////////////////////////////////////
// CFloatingHogaDlg dialog
//2006.08.08 {{ 6~10ȣ�� ������ ����. 5ȣ�������� �����}}
//////////////////////////////////////////////////////////////////////////
		
class CFloatingHogaDlg : public CRscDlg
{
// Construction
public:
	void SetMarkRealCurPrice(char *pData, int nSize); //���簡 ǥ��...

	CFloatingHogaDlg(CWnd* pParent = NULL);   // standard constructor
	HBITMAP			m_hBitmap;
	
	CMap<WORD, WORD, CString, CString>	m_mapHogaReal;

	//2005. 07. 14 ===============================================================
	CString			m_strCode;
	CString			m_strOldCode;

	CExGridCtrl*		m_pGridCtrl;
	CRect			m_rectGrid;										// �׸��� ������ ũ��

	LOGFONT			m_logGridFont;
	CFont			m_GridFont;

	TenHogaData		m_10HogaData;				// �ŵ�10 ~ �ż�10 ȣ�� ����
	CUtil			m_Util;											// ���� Util Ŭ����

	CArray			<CHogaData*, CHogaData*>				m_HogaData;
	void			HogaMemoryFree();
	void			SendTrHoga();
	void			RecvHogaData(int nLength , char* pData);

	//<<20100308_JS.Kim �ַθ�
	//void			SetRealHogaData(char* pstEUREAL_KXH1);
	void			SetRealHogaData(char* pstREAL_CAH1);
	//>>
//	void			SetRealHogaData(long m_pRealData);
	void			SetRealHogaData_Sub(CString strHoga,/* CString strNumber, CString strChangeVol,*/ CString strPreClosePrice, BOOL bMaeDo);
	void			SetRealHogaData_Rem(CString strNumber, BOOL bMaeDo);
	int				CheckRealHogaDataWithinHogaData(CString strData, int *nIndex);
	
	void			DisplayClear_AllHogaData();
	void			SetHogaData(char *pData, int nSize, BOOL bGetBasicData=FALSE);
	void			SetHogaData_Sub(CString strHoga, CString strNumber, CString strPreClosePrice, BOOL bMaeDo);
	void			CompareNUpdateHogaData(int nRow, int nCol, CString strText, int nMask, COLORREF crFgClr);	
	COLORREF		GetHogaGridBkColor(int nHogaRange);// ȣ���� �ش��ϴ� �׸��� ���������� ��� �´�.


	void			InitGrid();
	void			InitGridRowCol();
	void			ChangeGrid_AllWidth();	
	void			InitGridFormat();

	void			ChangeGrid_RowHeight();
	void			ChangeGrid_ColumnWidth();

	BOOL			m_bExpandRight;									// �ֹ�â Ȯ�� / ���
	BOOL			m_bExpandBottom;								// 10��, 5�� ȣ�� Ȯ�� / ���
	BOOL			m_bOpenCol;
	BOOL			m_bIsFirstLoad;									// ���簡���ֹ��� �ε� �Ǿ�����
	BOOL			m_bIsShowReserveCol;							// �ֹ�����Į�� ���̱�/���߱�
	int				m_nStaticWidth[4];								// �ֹ�����, ����, �ܰ�, �ѱݾ� ����ƽ ��

	BOOL			m_bIsTenHoga;									// 10�ܰ�ȣ�� <-> Fullȣ��
	BOOL			m_bUseLimitHoga;								// �������� ��ȸ
	int				m_nTotalRowCnt;									// ��ü ������
	int				m_nCurPriceRow;									// ���簡��
	int				m_nLowLimitRow;									// ���Ѱ� ��
	int				m_n5HogaSumRow;									// 5�ܰ��� ǥ�ÿ�
	int				m_nTotalSumRow;									// ��ü�� ǥ�ÿ�
	int				m_nExtraVolRow;									// �ð����ܷ���
	
	int				m_nHogaRangeForColor;							// ȣ������ ����

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ����, ����
	CString			m_strCurPrice;									// ���簡
	CString			m_strPreClosePrice;								// ���ذ�(��������)
	int				m_nHighLimit;									// ���Ѱ�
	int				m_nLowLimit;									// ���Ѱ�
	CString			m_strPreVol;									// ���ϰŷ���	JSJ_ITCastle_Add_040502
	CString			m_strSEqualPer;									// ���ð� ��� ���ϰŷ���
	int				m_nMaedoPosJango;								// �ŵ��ֹ� ���ɼ���
	int				m_nHogaRow;										// ȣ�� Row
	int				m_nStaticRow;									// Static Row
	int				m_nChartPriceRow;								// ��Ʈ���� ���� ������ �ش�ȣ����
	CString			m_strChartPrice;								// ��Ʈ���� ���� ����
	BOOL			m_bMoveChartPriceRow;							// ��Ʈ���� ���� ������ ��� �ش翭�� ��ũ���̵�
	char			m_chSign;


	// ---------------------------------------------------------------------------->>
	// (��� -> ������)   ==> (��� -> ����� -> ������)�� ���� 
	//BOOL			m_bDaebi;							// ���	 -> ������				JSJ_ITCastle_040430
	int				m_nThreeStateDaebiType;
	enum			{ e_DAEBI = 0 , e_DAEBI_PER , e_PROFIT_PER };	// ��� , ����� , ������
	//============================================================================


	BOOL	m_bIconState;
	CRect	m_dlgOriRect;
	BOOL	m_bCapTure;
	CWnd*	m_pMainDlg;
	// �ż�/�ŵ���ư �̹���
	CImageList		m_ImageListOrder;
// Dialog Data
	//{{AFX_DATA(CFloatingHogaDlg)
	enum { IDD = IDD_DLG_FLOATING };
	CPowLabel	m_staticGridFrame;
	CPowLabel	m_staticSymbol;
	CPowLabel	m_staitcTilte;	
	//CExDrawButton	m_ctrlButtonClose;
	CButton		m_ctrlButtonClose;
	CILButton	m_ctrlButtonSell;
	CILButton	m_ctrlButtonBuy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatingHogaDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFloatingHogaDlg)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);		
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonSell();
	afx_msg void OnButtonBuy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	IDrdsLib* m_pDrdsLib;

	BOOL MakeDrdsConnectAdvise();
	BOOL MakeDrdsUnConnectUnAdvise();
	void OnCodeAdvise(LPCSTR strCode);
	void OnCodeUnAdvise(LPCSTR strCode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATINGHOGADLG_H__21539FF9_3BC1_42EA_887C_7A7F9BAB708D__INCLUDED_)
