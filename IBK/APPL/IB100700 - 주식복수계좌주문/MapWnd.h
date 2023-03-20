#pragma once
// MapWnd.h : header file
//

#include "BuyWnd.h"
#include "CancelWnd.h"
#include "ExpandWnd.h"
#include "ModifyWnd.h"
#include "SellWnd.h"
#include "../../control/fx_misc/fxTab.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CMapWnd window
struct	_jinfo {
	char    codx[12];		// �����ڵ�
	char    hnam[32];		// �����
	char    jgub[1];		// �屸�� : '1':�ŷ��� '2':�ڽ���, '3' :��3����, '5':ETF
	char    size[12];		// ����Ը�
	char    k2gb[20];		// �ұ��� ����,����
	char    usun[10];		// ������/�켱��
	char    ksmm[2];		// ����
	char    amga[20];		// �׸鰡 8->20 ��������
	char    jbkm[20];		// �ں���(����:��) 12->20 ��������
	char    siga[12];		// ���Ͻð��Ѿ�(����:��)
	char    sjjs[12];		// �����ֽļ�(����:õ��)
	char	jrat[3];		// ���ű���
	char	nrat[3];		// �ſ����ű��� 20070718 
	char    krgb[12];		// "�ŷ�����", "�ŷ��ߴ�", "��������", "�̻�޵�", "��������", "����"
	char    jggb[12];		// �� ����
	char    lock[12];		// ������
	char    dyga[8];		// ��밡
	char    ujgb[12];		// ��������
	char    ujcd[6];		// RTS �����ڵ�
	char    jisu[8];		// ��������
	char    diff[8];		// �������ϴ��
	char    rate[8];		// �������
	char    gvol[12];		// �����ŷ���(����:õ��)
	char    unio[10];		// KRX 100 (0:�Ϲ� 1:��������)
	
};
#define L_jinfo		sizeof(struct _jinfo)

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* pWizard);

// Attributes
public:
	CWnd*		m_pCmdWnd;


// Operations
public:
	inline CFont* GetNormalFont() { return m_pNormalFont; };
	inline CFont* GetBoldFont() { return m_pBoldFont; };
	inline CString GetAxRoot() { return m_strRoot; };
	inline CBitmap* GetAxBitmap(CString strPath) { return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (long)(LPCTSTR)strPath); };
	CFont* GetAxFont(CString strName, int nPt, bool bItalic, int nBold);
	COLORREF GetIndexColor(UINT nIndex);
	void Init();
	CString Variant(int nComm, CString strData = _T(""));
	BOOL SendTR(CString strTR, CString strData, int nKey, int nStat);
	void Procedure(CString strProc);
	int GetMajor();
	int GetMinor();
	CString Parser(CString &srcstr, CString substr = _T("\t"));
	void QueryGroupList();
	void QueryGroupAccount(CString strGroupID);
	void ParsingGroupList(CString strData, int nLen, CString strMsg);
	void ParseAlert(CString strAlert);
	void ParseAlert(struct _alertR* alertR);
	void ParsingSise(CString strData, int nLen);
	void ParsingGroupAccount(CString strData, int nLen, CString strMsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCustomPrice(CString strPrice);
	CString GetAccountRate(CString strAccount);
	void ChangeSiseData();
	void DeleteSign(CString& strVal);
	CString GetSiseData(CString strSymbol);
	CString m_strCode;
	void QueryRemain(CString strGroupID, CString strCode);
	void ParsingDomino(CString strData);
	void SetGroupList(CString strData);
	CString GetGroupList();
	void ParsingTrigger(CString strData);
	BOOL IsCustomer();
	void SetGuide(CString strGuide);
	void SetPallete();
	void SetCode(CString strCode);
	void FormatThousand(CString& strVal);
	void CheckPassWord(CString sAcc, CString sPass, CWnd* pCmdWnd) ;
	CString GetGroupID(int nIndex);

	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnExpand();
	afx_msg void OnTabChange(NMHDR* pHdr, LRESULT* result);
	DECLARE_MESSAGE_MAP()

protected:
	void ShowHideBodyWnd(int nIndex);
	void ChangeFont(int ifontsixze, LPCTSTR lszFont);
	void DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType);
	void DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);
	void LoadRoundBitmap();


private:
	CWnd* m_pWizard{};
	
	std::unique_ptr<CBuyWnd> m_pBuyWnd{};
	std::unique_ptr < CCancelWnd> m_pCancelWnd{};
	std::unique_ptr < CExpandWnd>	m_pExpandWnd{};
	std::unique_ptr < CModifyWnd> m_pModifyWnd{};
	std::unique_ptr < CSellWnd> m_pSellWnd{};
	std::unique_ptr < CfxImgButton> m_pExpand{};
	std::unique_ptr < CfxStatic>	m_pWarning{};
	std::unique_ptr < CfxStatic>	m_pGuide{};
	std::unique_ptr < CfxTab>		m_pTab{};

	BOOL m_bExpand=TRUE, m_bInit{};
	CFont* m_pNormalFont{};
	CFont* m_pBoldFont{};
	CString m_strRoot{};
	CString m_strGroupList{};

	CMap<int,int,CString,LPCSTR> m_gpidMap;
	CMapStringToString m_mapSise;
	CStringArray m_arrGroupAccount;

	LPCTSTR				m_lszFontName{};
	int					m_iCurFont{}, m_iOldFont{}, m_iDefaultFont{}, m_iDefaultSize[2]{}, m_iOldSize[2]{};
	int					m_iWndWidth{}, m_iWndHeight{}, m_iExpendWidth{};

	double				m_lSizeRate[2]{};
	BOOL				m_bExpendClick{};
	HBITMAP				m_hRoundBitmap[10]{}, m_hGuideBitmap{};
};

