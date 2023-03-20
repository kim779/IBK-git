#pragma once
// GridWnd.h : header file
//

#include "../../h/interst.h"
#include "AxisExt.h"
#include "GridCtrlExt.h"
#include "../../control/fx_misc/fxImgButton.h"
#include <map>
#include <set>
#include <afxmt.h>

using namespace std;

#define COLUMN_SIZE (200)
#define DEF_ROW_SIZE (200)

#define GRIDMAXNUM 31

#define JMCODE 0	//�����ڵ�
#define YDAYVOLUMN 1	//���ϰŷ���
#define TDAYVALUE 2	//���簡
#define YDAYLASTVALUE 3 //��������
#define JISUGUBN 4	//��������

#define SEND_OUBS 0
#define SEND_GROUPLIST 1
#define SEND_CODES 2
#define SEND_GRID 3

/////////////////////////////////////////////////////////////////////////////
// CGridWnd window

// �� �Ѿ���� Ȯ���ϴ� �Լ�¥��
const int maxIDX = 10;

class CIndexMap
{
public:
	CIndexMap()
	{
		for (int ii = 0; ii < maxIDX; ii++)
			index[ii] = -1;
		idxCnt = 0;
	}
	// virtual ~CIndexMap();

public:
	int index[maxIDX];
	int idxCnt;
};

bool operator<(const CCellID &lsh, const CCellID &rsh);
bool operator<(CCellID &lsh, CCellID &rsh);

class CGridWnd : public CGridCtrlExt, public CAxisExt
{
	// Construction
public:
	CGridWnd(CWnd *pMainWnd, int nIndex);

	// Attributes
public:
	CWnd *m_pMainWnd;
	CWnd *m_pViewWnd;
	CWnd *m_pGroupWnd;

	COLORREF m_clrKospi;
	COLORREF m_clrKosdaq;
	COLORREF m_clrMarkerBKG;
	COLORREF m_clrMarkerTXT;
	BOOL m_bExpect;
	BOOL m_bAutoExpect; // ���� �ڵ�����
	BOOL m_bEditWork;
	bool m_bIsFirst;
	BOOL m_bAddCnd;
	BOOL m_bChgCnd;
	bool m_bRTMReady;
	bool m_bSelect;
	bool m_bLBtnDown;
	bool m_bKeyDown;
	BOOL m_bPopupDlg;

	int m_ClickRow;

	CString m_szTitle;
	CString m_szFIELD;
	CString m_szWIDTH;
	int m_nCodeWidth;
	bool m_bPlusMinus; // 2011.12.05 KSJ	�߰� ������ ���� �޼��� ������ �ʵ��� �ϱ� ����
	bool m_posField;
	bool m_ccField;	  // client calculate filed
	int m_bongField;  // �Ϻ��ʵ�
	int m_sonikField; // �򰡼��� �ʵ�
	int m_suikField;  // ���ͷ� �ʵ�
	bool m_endsort;
	int m_drag, m_drop;
	CString m_strCode;

	int m_yDayVolField;
	int m_recomDataField;
	int m_recomBasicCostField; // ��õ���� ���ذ�
	int m_recomSuikrateField;
	int m_recomBMjisuField;
	int m_recomBMSuikrateField;
	int m_recomgoalCostField;
	int m_recomJisuGubnField;
	int m_upjongCode;     //���� �����ڵ�
	int m_beforeVolField; //���ϰŷ���
	int m_newCurrField;   //���簡
	int m_EqualizerField; //ü�� ���������� �ʵ�

	double	m_dBase;		//�����ɼ� �⺻ ����

	int m_nType;

	int m_pdir;
	CCellID m_pid;
	COLORREF m_crNormalBk, m_crYellowBk;
	CCellID m_CurCell;
	CArray<_gridHdr, _gridHdr> m_gridHdrX;
	COLORREF m_upCOL, m_dnCOL, m_eqCOL;
	COLORREF m_fgCOL, m_bkCOL;
	COLORREF m_bkSELCOL, m_fgSELCOL;

	CString m_root, m_user, m_fileCFG, m_strTag;

	UINT m_kind;
	int m_nGridIndex;  //���° �׸�������.
	int m_nGroupIndex; //���° �׷��� �����ְ� �ִ���.
	CString m_section;
	CRect m_rcRect;

	int m_rowC;
	int m_colC;
	int m_nGridWidth;
	bool m_bMKClrApply;
	bool m_bSorting;

	double m_dKospi;
	double m_dKosdaq;

	CMapStringToString m_mapCode;
	CMapStringToString m_mapMargin;
	CMapStringToString m_mapCurValue; // 2012.04.03 KSJ ó�� ���簡 �ֱ�����..

	std::vector<std::shared_ptr<_intersx>> m_inters;
	std::vector<std::shared_ptr<_intersx>> m_arrBaseInters;

	int m_updateROW;
	int m_iStartPoint;  // Send ���۽� �ʵ��� ���� ����
	int m_iEndPoint;    // Send ���۽� �ʵ��� �� ����
	int m_iSendcount;   // Send �� ������ Ƚ��
	int m_nBookMarkRow; // 2013.05.21  KSJ �ϸ�ũ ���� Row

	BOOL m_bContinue; // ���������� �ʵ尡 �������
	BOOL m_bSecond;
	int m_staticUpdate;

	CStringArray m_pGridArray;
	CMapStringToOb  m_pSearchMap;

	std::map<CString, std::map<int, CString>> _mapFms;
	std::vector<std::unique_ptr<CIndexMap>>   _vIndex;

	CString m_strBeginTime; //����ȣ�� ���۽ð� xx:10:00
	CString m_strEndTime;	//����ȣ�� ����ð� xx:59:59

	int m_irowCode[100];	    //�ڵ尡 �����ϴ� ����
//	CString m_iYDayVol[100][5]; // ���ϰŷ����� �����ϱ� ���� ����(�����ȣ, ���ϰŷ���, ���簡, ��������, ��������)

	CBitmap *m_pTBmp;
	CBitmap *m_pBBmp;

	CIHStringMapArray m_arExist;

	CCriticalSection		m_syncRTM;
	CWinThread*			m_pThreadRTM;
	bool				m_bThreadRTM;
	BOOL				m_bRemove;


//	void	DrawLine();
	//����
	void	getBase(CString m_code);
	CString	getFormatData(CString src);
	int	getJKind(CString m_code);
	CString	CalFutureEvalPrice(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);
	CString	CalFutureEvalRate(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian);

	//�ֽ�
	CString CalcuPyungaSonik(struct _intersx* pinters, CString curr);
	CString CalcuSuik(struct _intersx* pinters, CString curr);

	void	calcInClient();
	void	calcInClient(int row);



	bool WaitRTM();

	HANDLE	m_hEventRTM;

private:	// RTM

	void	initRTM();
	void	stopRTM();
	void	removeRTM();
	void	removeRTMx();	// 2012.02.10 KSJ



public:
	UINT GetKind() { return m_kind; }
	void SetKind(UINT kind);
	void SetGridIndex(int nIndex)
	{
		m_nGridIndex = nIndex,
		m_section.Format("%s%02d", "GRID", m_nGridIndex);
	}
	int GetGroupIndex() { return m_nGroupIndex - 1; }

	void ClearInterest();
	void RemoveAll();
	void Reset(bool bAll = true);
	CString GetcolName(int index);
	CString GetCurGroupName() { return m_szTitle; }
	int GetCodeType(CString code);
	CString GetCodeName(CString code) { return CAxisExt::GetCodeName(code); }

	std::shared_ptr<struct _intersx> GetData(int nIndex) { return m_inters.at(nIndex); }
	// void	SetData(int nIndex, struct _inters* pinters) { m_inters.SetAt(nIndex, pinters); }
	int GetInter(CString code);
	void AddData(int nIndex, CString sztmp);
	void GetData(CArray<_inters *, _inters *> &arData);
	void GetData(CArray<_inters *, _inters *> &arData, int *nRowCount); // arData�� row���� ��ȯ

	void sendTransactionTR(int update, int nStart, int nEnd);
	void sendTransaction(int update = -1);
	void FieldSetup(bool bDlgSetup = false, int iEqualizer = 0, bool bResize = true);
	BOOL IsValidCode(CString strCode);
	int loadcfg_data();
	int GetGridWidth();
	int GetWidth();
	CString GetItemSymbol(int nRow, int nCol);

	void SetMarketColor(BOOL bApply, COLORREF clrKospi, COLORREF clrKosdaq);
	void SaveAsInterField();
	void MarkerSetup();

	int insertRow(int row, BOOL bInsertInter = TRUE);
	void InsertRowNCode(int row);
	void DeleteRow(int row);

	int CheckRealTimeCode(CString code);
	void ClearSearchMap();
	void ReSetSearchMap(); // 2011.12.29 KSJ
	void ReIndexing();

	void loadcfg();
	void savecfg();

	void parsingField(CString symbol, bool bDlgSetup = false);
	void parsingGrid(char *datB, int datL);
	void parsingGridOne(char *datB, int datL, int update = -1);
	void queryGrid(CString strCodes = "");	 //����׸��� ����� �ü�
	void parsingCodes(char *datB, int datL); //�ش�׷��� �����
	void queryCodes(CString strGroup, CString strGroupName);
	void RecvOper(int kind, CRecvData *rdata);
	void parsingAlertx(LPARAM lParam);
	void OperResize(int cx, int cy);

	CString CalMaketTime(CString strTime, bool bEnd);

	// Operations
public:
	void Init();
	BOOL Create(const RECT &rect, CWnd *parent, UINT nID, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE, DWORD dwExStyle = 0);
	void SetGridHeader(CString strHeader);
	void GetGridHeader(int nIndex, _gridHdr &gHdr);

	bool IsDuplicateCode(CString code, int nrow, bool drag = false);
	void RbuttonAction(int row);
	void CopyInter(_inters *pCpy, _inters *pSrc);

	void RemoveAllBaseInters();
	int AddInters(std::shared_ptr<_intersx> pinters);
	void RemoveInters(int nIndex);

	void insertInterest(int idx);
	void saveInterestX();
	void saveInterest(bool btmp = false, int gno = -1, bool bBookMark = false); // 2013.05.14 KSJ �ϸ�ũ �����Ҷ��� MO_VISIBLE ������ϵ���

	bool loadField(int fno = -1, bool bDlgSetup = false, int iEqualizer = 0);

protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridWnd)
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);
	//}}AFX_VIRTUAL

	std::map<CString, int> _mapAlert;

	// Implementation
public:
	virtual ~CGridWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
