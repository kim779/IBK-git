#if !defined(AFX_DLGTOOLBAR_H__F80CD3CD_73BD_46D1_B3DD_35C2D8037915__INCLUDED_)
#define AFX_DLGTOOLBAR_H__F80CD3CD_73BD_46D1_B3DD_35C2D8037915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgToolbar.h : header file
//

#include "../chart_common/BlockBaseEnum.h"
#include "RscDlg.h"
#include "../chart_common/ILButton.h"


#define NUM_BUTTONS 80
/////////////////////////////////////////////////////////////////////////////
// CDlgToolbar dialog
class CChartToolBtn;
class CFileBitmap;
class CLineThicknessSelDlg;
class CDlgToolbar : public CRscDlg
{
// Construction
public:
	CDlgToolbar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgToolbar();

	CChartToolBtn* m_pToolBtn[NUM_BUTTONS];
	virtual void ResizeControl(int cx, int cy);
	void SelectToolSelection();
	void SetChartMng(CWnd* pWnd) {m_pChartMng = pWnd;};
	void ResetAllButton();
	//2007.01.17 add by LYH  ���ٹ�ư ����ȭ
	void SetToggleButton(int nSelToolNo, BOOL bSelect);
	BOOL GetIndexArray(int nSelTool, CArray<int, int>& arIndx, CArray<int, int>& arCheck);
	BOOL GetToolNameMap(CMap<int, int, CString, CString>& mapToolName);
	CToolOptionInfo::TOOLOPTION GetToolOption(UINT nId);
	void SetToggleBtnFromOutside(UINT nID);

	IGateManager*	m_pGateManager;

protected:
	BOOL CreateToolButton();
	BOOL CreateImageList();
	CString GetChartToolDescription(CString strToolName);

	// Gdi objects
	HBRUSH			m_hWhiteBrush;		// brush for painting editbox

	CFont			m_BtnFont;
	CToolTipCtrl	m_ToolTip;
	CWnd*			m_pCurToggleBtn;
	BOOL			m_bCheckShowData;
	CWnd*			m_pMultiChartWnd;
	CWnd*			m_pChartMng;
	int				m_nCursorIndex;
	//2006.01.07 by LYH
	BOOL			m_bShowScale;
	int				m_nYUnitType;
	BOOL			m_bShowInnerToolbar;

	CImageList*   m_pIL_W28H22; //�� 28�� ���� 22�� ��ư �̹�������Ʈ

	//CStringArray		m_arrayToolName;		//�м���������Ʈ �̸�
	CArray<int, int>	m_arrayTotalIdx;		//��ü����Ʈ �ε���
	CArray<int, int>	m_arrayAnalIdx;			//�м���������Ʈ �ε���
	CArray<int, int>	m_arrayAdditionIdx;		//�ΰ���ɸ���Ʈ �ε���
	CArray<int, int>	m_arrayUserIdx;			//����ڼ�������Ʈ �ε���
	CMap<int, int, CString, CString>	m_mapToolName;			//�м���������Ʈ �̸�
	void ReadIniFile(BOOL bReload = FALSE);
	void GetFieldData(CString strFieldData, int &nFieldKey, CString &strFieldName);
	int m_nSelTool;

	//20070104 by ������
	//������ ũ�� ����� ������ ������ ��ư�� ȭ�麸�� ��� ��ġ�Ȱ�� ��/���� �޴����� ��ư���� ���� 
	//�����޴����� ��ư Ŭ���� ���޴�����
	//�����޴����� ��ư Ŭ���� �Ʒ� �޴�����
	void ResizeToolbar(int nY);
	BOOL DrawHighMenu(int nHighMenuCount, int nSelTool);
	BOOL DrawLowMenu(int nLowMenuCount, int nHighMenuCount, int nSelTool);
	void GetDrawInfo(int nY, int nSelTool);
	typedef struct _DRWAINFO
	{
		int nPreWindowPos;					//�������� ���� rect.bottom
		int nCurrWindowPos;					//�������� ���� rect.bottom
		int nRealMenuCount;					//���� ���� ��ư ����
		int nCalcMenuCount;					//����� toolbar�� ��ġ ������ ��ư ����
		int nHighMenuCount;					//������ rect.bottom < toolbar rect.bottom ��� ���� ��ġ ������ ��ư ����
		int nLowMenuCount;					//������ rect.bottom < toolbar rect.bottom ��� �Ʒ��� ��ġ ������ ��ư ����
		int nLastBtnPos;					//���� ������ ��ư�� rect.bottom
		int nLowBojo;						//�Ʒ� ��ư Ŭ���� �ڿ� ǥ���� ��ư�� �����ִ� ��� ����
		int nLowLastIndex;					//���� �������� ǥ�õ� ��ư�� index
		int nHighBojo;												
		int nHighLastIndex;
		int nTotBojo;
	} DRWAINFO;

	DRWAINFO m_drawinfo;
	int m_nResizeFlag;						//ResizeToolbar func ȣ�� ���� 

	CILButton* m_pBtnUp;
	CILButton* m_pBtnDown;
	// �̹���
	CImageList		m_ImgList;
	BOOL m_bInit;
	//by ������ end
	
public:
// Dialog Data
	//{{AFX_DATA(CDlgToolbar)
	enum { IDD = IDD_DLG_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgToolbar)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgToolbar)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTOOLBAR_H__F80CD3CD_73BD_46D1_B3DD_35C2D8037915__INCLUDED_)
