#pragma once

#include "../AxMapForm/AxMapForm.h"
#include "axStatic.h"
#include "exTreeCtrl.h"
#include "XXGrid.h"
#include "ImgBtn.h"
#include "afxtempl.h"
#include "../../control/fx_misc/fxStatic.h"

class 	_acSave	
{
public:
	CString sPriAcc;	//��ǥ���±���
	CString sSortNum;	//�����Ϸù�ȣ
	CString sAccntNum;	//���¹�ȣ
	CString sAccntName;	//�����̸�
	CString	sPassword;	//���º��
	CString sAllocRate;	//��������
	CString	sAllocMulti;	//�¼�
	CString sAccntNick;	//���º�Ī
public:
	_acSave()
	{
		sPriAcc = _T("");
		sSortNum = _T("");
		sAccntNum = _T("");
		sAccntName = _T("");
		sPassword = _T("");
		sAllocRate = _T("");
		sAllocMulti = _T("");
		sAccntNick = _T("");
	};

	_acSave(_acSave const &other)
	{
		Copy(other);
	};

	~_acSave() {}

	inline void Copy(_acSave const &other)
	{
		sPriAcc = other.sPriAcc;
		sSortNum = other.sSortNum;
		sAccntNum = other.sAccntNum;
		sAccntName = other.sAccntName;
		sPassword = other.sPassword;
		sAllocRate = other.sAllocRate;
		sAllocMulti = other.sAllocMulti;
		sAccntNick = other.sAccntNick;
	};

};

//=================================================================================================
class	_gpSave	
{
public:
	CString sGrSortNum;		// �׷��Ϸù�ȣ
	CString sGrName;		// �׷��
	CString sAccntCnt;		// ���°���
	CString	sGroupID;		// �׷���̵�
	CArray <_acSave *, _acSave *>	arAcEdit;
public:
	_gpSave()
	{
		sGrSortNum = _T("");
		sGrName = _T("");
		sAccntCnt = _T("");
		sGroupID = _T("");
		arAcEdit.RemoveAll();
	};

	~_gpSave()
	{
		_RemoveAr();
	};

	_gpSave(_gpSave const &other)
	{
		Copy(other);
	};

	inline	void _RemoveAr()
	{
		for (int ii = 0; ii < arAcEdit.GetSize(); ii++)
		{
			_acSave* acSave = arAcEdit.GetAt(ii);
			if (acSave)
			{
				delete acSave;
				acSave = NULL;
			}
		}
		arAcEdit.RemoveAll();
	};

	inline void Copy(_gpSave const &other)
	{
		sGrSortNum = other.sGrSortNum;
		sGrName = other.sGrName;
		sAccntCnt = other.sAccntCnt;
		sGroupID = other.sGroupID;

		_acSave* acSave = NULL;
		_acSave* newSave = NULL;
		for (int ii = 0; ii < other.arAcEdit.GetSize(); ii++)
		{
			acSave = other.arAcEdit.GetAt(ii);
			if (acSave)
			{
				newSave = new _acSave();
				newSave->Copy(*acSave);
				arAcEdit.Add(newSave);
			}
		}
	};
};

//=================================================================================================
class CMapForm : public CAxMapForm
{
public:
	CMapForm();
	virtual ~CMapForm();
	DECLARE_DYNCREATE(CMapForm)

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
public:
	//{{AFX_DATA(CMapForm)
	enum { IDD = IDD_FORMVIEW };
	CImgBtn		m_btnInital;
	CImgBtn		m_btnGrDown;
	CImgBtn		m_btnGrUp;
	CImgBtn		m_btnGrCopy;
	CExTreeCtrl	m_tree;
	CButton		m_passCheck;
	CAxStatic	m_stGrNum;
	CAxStatic	m_stGrName;
	CAxStatic	m_result;
	CImgBtn		m_btnOk;
	CImgBtn		m_btnDnBot;
	CImgBtn		m_btnUpTop;
	CImgBtn		m_btnCancel;
	CImgBtn		m_btnDirect;
	CImgBtn		m_btnUp;
	CImgBtn		m_btnRefresh;
	CImgBtn		m_btnInsert;
	CImgBtn		m_btnGrNew;
	CImgBtn		m_btnGrDel;
	CImgBtn		m_btnGrChg;
	CImgBtn		m_btnDn;
	CImgBtn		m_btnDel;
	CImgBtn		m_btnAllDel;
	CImgBtn		m_btnAllAdd;
	CImgBtn		m_btnAdd;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMapForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMapForm)
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnadd();
	afx_msg void OnBtnrefresh();
	afx_msg void OnSelchangeCbgr();
	afx_msg void OnBtngrnew();
	afx_msg void OnBtngrchg();
	afx_msg void OnBtngrdel();
	afx_msg void OnBtninsert();
	afx_msg void OnBtndel();
	afx_msg void OnBtnup();
	afx_msg void OnBtndn();
	afx_msg void OnBtnalladd();
	afx_msg void OnBtnalldel();
	afx_msg void OnBtndirect();
	afx_msg void OnBtncancel();
	afx_msg void OnBtnok();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnuptop();
	afx_msg void OnBtndnbot();
	afx_msg void OnBtngrcopy();
	afx_msg void OnBtngrdown();
	afx_msg void OnBtngrup();
	afx_msg void OnRdhm();
	afx_msg void OnRdfu();
	afx_msg void OnBtninitial();
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGrAccClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGrAccblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGrEditClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGrEditdblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGrEditEndSort(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBeginEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnKeyMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAccNameSearch(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	HBITMAP getBitmap2(CString bmps);

private:
	CBitmap* m_bitGuide;
	void	initConstruct();
	void	destroy();

	void	initControl();
	void	initGrid(CXXGrid* pGrid, UINT idc);
	void	InitList();
	void	updateTree();
	void	updateGrAcc();
	void	updateGrEdit(int gpIdx);
	void	updateGrEditIdx(_gpSave	*gpSave);		// numǥ��
	void	updateGrEditPri(_gpSave *gpSave);		// ��ǥ���� ����
	void	updateGrContent(int gpIdx);			// �׷��, ��ϰ��¼� ǥ��

	HTREEITEM	insertTreeItem(CString name, int data);	// tree item add

	void	enableGroup(BOOL bEnable);			// ��ư en/dis �߰������
	void	enableAcc(BOOL bEnable);			// ��ư en/dis �߰������

	void	setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit);
	bool	loadGrid(CXXGrid* pGrid, CString section);
	bool	saveGrid(CXXGrid* pGrid, CString section);

	CString	getLedger(CString sTran, char cGubn, CString sSvcn, CString sLastKey = "");
	
	void	queryAcc();							// ������ȸ
	void	queryStaffAcc(CString ledgerS);					// ����������ȸ
	void	queryGroup();							// �׷���ȸ
	void	queryGroupSave(int sendKey, int selType);			// �׷�����
	void	nextSend(char next, int sendKey);				// next ó�� �׷�����(14K�̻�)
	void	queryAccNameSearch(CString accNum);				// ���¸���ȸ
	void	queryAccInspectName(CString accNum);				// ���¸����
	void	queryAccInspect(CString accNum, CString accPass, int key);	// ������ȿ���˻�(���edit�Է½�)
	void	sendTR(CString trC, char* pBytes, int nBytes, char key);
	
	void	oubAccUser(char* buf);					// ��������ȸ
	void	oubAccList(char* buf, int len);				// ����������ȸ
	void	oubAccListEx(char* buf, int len);			// ����������ȸ(parsing)
	void	oubGroup(char* buf);					// �׷������ȸ
	void	oubGroupSave(char* buf, int sendKey);			// �׷��������
	void	oubAccountName(char *buf, int len);			// ���¸���ȸ
	void	oubAccInspectName(char *buf, int len);			// ���¸����
	void	oubAccInspect(char *buf, int len);			// ���°���
	void	oubAccInspectEdit(char *buf, int len);			// ������ȿ���˻�(���edit�Է½�)

	void	processError(void *pLedger);
	void	domino(CString str);

	void	deleteAllAcSave();
	void	deleteAllGpSave();

	void	setMessage(char* format, ...);

	void	setMapPass(CString acc, CString pass);
	CString	getMapPass(CString acc);

	CString	getPassString(CString sPass);					// grid�� '*'�� ���̵��� 
	CString	parser(CString &srcstr, CString substr);

	bool	checkIsGrName(CString newGrName);
	int	getCurGr(HTREEITEM &item);
	int	getGr(CString grName);
	CString	getCurGrName();
	_gpSave*	getCurGpSave(int& gpIdx);
	int	getGrAccSelectRow();
	_acSave*	getCurAcSave();
	int	getGrEditSelectRow();
	int	insertGrEditRow(_acSave* acSave, int row);
	bool	checkGrEdit(_gpSave* gpSave, CString accNum);
	
	void	sortGpSave();

	void	addGrEdit(int idx = -1);
	void	addGrEditAll();
	void	addInputAccount();
	void	delGrEdit();
	void	delGrEditAll();
	void	changeGrEdit(bool bUp, bool bBound = false);
	

	void	enableAllBtn(BOOL bEnable);
	void	redrawCtrl();
	void	selectRadio(UINT idc, bool bSend = true, bool bFirst = false);
	std::unique_ptr<CfxStatic> m_panel;

protected:
	std::unique_ptr<class CDlgDirect> m_dlgDirect;
	CString		m_sFontName;
	int		m_nFontPoint;
	CFont		*m_pFont;

	bool		m_bCustomer;			// ������ ����
	CString		m_sRootDir;			// root directory
	CString		m_szFilename;			// grid ���� ����

	CString		m_sStaffNum;			// �����ȣ
	CString		m_sPassWord;			// ��й�ȣ
	CString		m_sDeptCode;			// �μ���ȣ
	char		m_gubn;				// ����/�ϳ�����

	std::unique_ptr<CXXGrid> m_pGrAcc;
	std::unique_ptr<CXXGrid> m_pGrEdit;

	bool		m_bFirst;			// ��ȸ 
	COLORREF	m_clrBG;
	COLORREF	m_clrGuide;


	CArray <_acSave *, _acSave *>	m_arAcSave;
	CArray <_gpSave *, _gpSave *>	m_arGpSave;
	
	CMapStringToString		m_mapPass;
	CStringArray	m_arDelGp;

	CString		m_rtnStr;
	std::unique_ptr<char[]> m_sendB;
	std::unique_ptr<char[]> m_sendB2;

	int		m_nextTRPos;
	int		m_totTRCnt;
	int		m_subTRCnt;
	int		m_calBufL;

	CString		m_sInputAccPass;
	CString		m_sInputAccNum;
	CString		m_sInputAccName;

	int		m_passRow;			// pass ���� row

	enum SELTYPE	{ SEL_NOT= 0, SEL_HM, SEL_FU }	m_selType;
	bool		m_bChange;

	BOOL TRSend(CString name, BYTE type, CString data, int key);
private:
	BOOL	m_useAlias;
	
	BOOL	LoadConfig_UseAlias();
	CString GetAcctSymbol(const char* szAcct);

	CString GetAcctName(const char* szName);
	CString GetUserConfigFileName();
	CString GetAcctNameFromAllAccount(const char* acct, const char* name);

	void AddAcctSave(const char* no, const char* nm);
	void LoadAccountListFromWizard();

};

