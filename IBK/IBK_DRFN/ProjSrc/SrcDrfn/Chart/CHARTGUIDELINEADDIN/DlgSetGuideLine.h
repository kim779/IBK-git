#if !defined(AFX_DLGSETGUIDELINE_H__566DA12E_84F8_41DE_8E1F_60BF51C2CC32__INCLUDED_)
#define AFX_DLGSETGUIDELINE_H__566DA12E_84F8_41DE_8E1F_60BF51C2CC32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./Control/BitmapItemComboBox.h"
#include "./Control/GraphData.h"
#include "./Control/ColorButton.h"
#include "./resource.h"
#ifndef __COMMON_DEF_H
	#include "./Client/commondef.h"
#endif


// DlgSetGuideLine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGuideLine dialog
//-----------------------------------------------------------------------------
// Author		: ITCNam(ITCastle, Nam)	Date :2005/1/27
// Comments		: ���ؼ� ����â ���̾˷α�
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
static TCHAR* szType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
		_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
		_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
		_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
		_T("24")
};


class  CSubGraphData;
class CDlgSetGuideLine : public CDialog
{
// Construction
public:	
	CDlgSetGuideLine(CWnd* pParent = NULL);   // standard constructor
	CConfigSet m_clConfigSet;
	CString m_strCfgFile;
	CString m_strFullAcct;
	CBrush  m_brGDrawColor;
	
		
	void AddDashToAccount(LPCSTR _lpIn, CString& szOut);
	void CreateEnvSetupFile();
	BOOL LoadCfgFile();
	BOOL SaveCfgFile();
	void AddLineType(CComboBox* pCombo, int Type);
	void Initial_ThicknessComboBox();
	void Initial_ColorBtn();
	void Initial_ChkBox();
	void Initial_AcctCombo();
	void Initial_Option();

// Dialog Data
	//{{AFX_DATA(CDlgSetGuideLine)
	enum { IDD = IDD_DLG_GLINE };
	BOOL	m_bNotDrawLine;					//���̵� ������ �׸���.
	CComboBox	m_cmbAcct;
	CEdit	m_edt1YP;						// �̵���� ù��° �Է°�	 ����Ʈ
	CEdit	m_edt2YP;						// �̵���� �ι�° �Է°�	 ����Ʈ
	CEdit	m_edt3YP;						// �̵���� ����° �Է°�	 ����Ʈ
	CEdit	m_edt4YP;						// �̵���� �׹�° �Է°�	 ����Ʈ
	CEdit	m_edtPmmPD;						// ��ո��Դܰ� + �Է°�	 ����Ʈ
	CEdit	m_edtPmmMD;						// ��ո��Դܰ� - �Է°�	 ����Ʈ
	BOOL	m_bChkYSprice;					//���ϰ��� �ð�				 üũ ��ư       
	BOOL	m_bChkYHprice;					//���ϰ��� ��				 üũ ��ư       
	BOOL	m_bChkYLprice;					//���ϰ��� ����				 üũ ��ư       
	BOOL	m_bChkYEprice;					//���ϰ��� ����				 üũ ��ư       
	BOOL	m_bChkYSHL;						//���ϰ��� (��+��+��)/3		 üũ ��ư               
	BOOL	m_bChkTSprice;					//���ϰ��� �ð�				 üũ ��ư       
	BOOL	m_bChkTHprice;					//���ϰ��� ��				 üũ ��ư       
	BOOL	m_bChkTLprice;					//���ϰ��� ����				 üũ ��ư       
	BOOL	m_bChkTEprice;					//���ϰ��� ����				 üũ ��ư       
	BOOL	m_bChkTHL;						//���ϰ��� (��+��)/2		 üũ ��ư               
	BOOL	m_bChkTSHL;						//���ϰ��� (��+��+��)/3		 üũ ��ư               
	BOOL	m_bChkTHLE;						//���ϰ��� (��+��+(��*2))/4  üũ ��ư
	BOOL	m_bChkHighLimit;				//���Ѱ�					 üũ ��ư               
	BOOL	m_bChkLowLimit;					//���Ѱ�					 üũ ��ư               
	BOOL	m_bChkPivot2Rst;				//�Ǻ� 2�� ����				 üũ ��ư               
	BOOL	m_bChkPivot1Rst;				//�Ǻ� 1�� ����				 üũ ��ư               
	BOOL	m_bChkPivotGLine;				//�Ǻ� ���ؼ�				 üũ ��ư               
	BOOL	m_bChkPivot1GG;					//�Ǻ� 1�� ����				 üũ ��ư       
	BOOL	m_bChkPivot2GG;					//�Ǻ� 2�� ����				 üũ ��ư	
	BOOL	m_bChkDMKPreHPrice;				//Demark �����			 üũ ��ư               
	BOOL	m_bChkDMKGLine;					//Demark ���ؼ�				 üũ ��ư       
	BOOL	m_bChkDMKPreLPrice;				//Demark ��������			 üũ ��ư	        
	BOOL	m_bChk2Pyung1;					//�̵���� ù��° �Է°�	 üũ ��ư               
	BOOL	m_bChk2Pyung2;					//�̵���� �ι�° �Է°�	 üũ ��ư               
	BOOL	m_bChk2Pyung3;					//�̵���� ����° �Է°�	 üũ ��ư               
	BOOL	m_bChk2Pyung4;					//�̵���� �׹�° �Է°�	 üũ ��ư	        
	BOOL	m_bChkPmmDanga;					//��ո��� �ܰ�				 üũ ��ư       
	BOOL	m_bChkPmmPDanga;				//��ո��� �ܰ� +n			 üũ ��ư               
	BOOL	m_bChkPmmMDanga;				//��ո��� �ܰ� -n			 üũ ��ư  	
	CColorButton m_btnYSprice;				//���ϰ��� �ð�				���� �� ���� ��ư
	CColorButton m_btnYHprice;				//���ϰ��� ��				���� �� ���� ��ư
	CColorButton m_btnYLprice;				//���ϰ��� ����				���� �� ���� ��ư
	CColorButton m_btnYEprice;				//���ϰ��� ����				���� �� ���� ��ư
	CColorButton m_btnYSHL;					//���ϰ��� (��+��+��)/3		���� �� ���� ��ư
	CColorButton m_btnTSprice;				//���ϰ��� �ð�				���� �� ���� ��ư
	CColorButton m_btnTHprice;				//���ϰ��� ��				���� �� ���� ��ư
	CColorButton m_btnTLprice;				//���ϰ��� ����				���� �� ���� ��ư
	CColorButton m_btnTEprice;				//���ϰ��� ����				���� �� ���� ��ư
	CColorButton m_btnTHL;					//���ϰ��� (��+��)/2		���� �� ���� ��ư
	CColorButton m_btnTSHL;					//���ϰ��� (��+��+��)/3		���� �� ���� ��ư
	CColorButton m_btnTHLE;					//���ϰ��� (��+��+(��*2))/4 ���� �� ���� ��ư
	CColorButton m_btnHighLimit;			//���Ѱ�					���� �� ���� ��ư
	CColorButton m_btnLowLimit;				//���Ѱ�					���� �� ���� ��ư
	CColorButton m_btnPivot2Rst;			//�Ǻ� 2�� ����				���� �� ���� ��ư 
	CColorButton m_btnPivot1Rst;			//�Ǻ� 1�� ����				���� �� ���� ��ư
	CColorButton m_btnPivotGLine;			//�Ǻ� ���ؼ�				���� �� ���� ��ư
	CColorButton m_btnPivot1GG;				//�Ǻ� 1�� ����				���� �� ���� ��ư
	CColorButton m_btnPivot2GG;				//�Ǻ� 2�� ����				���� �� ���� ��ư	
	CColorButton m_btnDMKPreHPrice;			//Demark �����			���� �� ���� ��ư
	CColorButton m_btnDMKGLine;				//Demark ���ؼ�				���� �� ���� ��ư
	CColorButton m_btnDMKPreLPrice;			//Demark ��������			���� �� ���� ��ư	
	CColorButton m_btn2Pyung1;				//�̵���� ù��° �Է°�	���� �� ���� ��ư
	CColorButton m_btn2Pyung2;				//�̵���� �ι�° �Է°�	���� �� ���� ��ư
	CColorButton m_btn2Pyung3;				//�̵���� ����° �Է°�	���� �� ���� ��ư
	CColorButton m_btn2Pyung4;				//�̵���� �׹�° �Է°�	���� �� ���� ��ư	
	CColorButton m_btnPmmDanga;				//��ո��� �ܰ�				���� �� ���� ��ư
	CColorButton m_btnPmmPDanga;			//��ո��� �ܰ� +n			���� �� ���� ��ư
	CColorButton m_btnPmmMDanga;			//��ո��� �ܰ� -n			���� �� ���� ��ư
	CBitmapItemComboBox m_cmbYSprice;		//���ϰ��� �ð�				�޺�
	CBitmapItemComboBox m_cmbYHprice;		//���ϰ��� ��				�޺�
	CBitmapItemComboBox m_cmbYLprice;		//���ϰ��� ����				�޺�
	CBitmapItemComboBox m_cmbYEprice;		//���ϰ��� ����				�޺�
	CBitmapItemComboBox m_cmbYSHL;			//���ϰ��� (��+��+��)/3		�޺�
	CBitmapItemComboBox m_cmbTSprice;		//���ϰ��� �ð�				�޺�
	CBitmapItemComboBox m_cmbTHprice;		//���ϰ��� ��				�޺�
	CBitmapItemComboBox m_cmbTLprice;		//���ϰ��� ����				�޺�
	CBitmapItemComboBox m_cmbTEprice;		//���ϰ��� ����				�޺�
	CBitmapItemComboBox m_cmbTHL;			//���ϰ��� (��+��)/2		�޺�
	CBitmapItemComboBox m_cmbTSHL;			//���ϰ��� (��+��+��)/3		�޺�
	CBitmapItemComboBox m_cmbTHLE;			//���ϰ��� (��+��+(��*2))/4 �޺�	
	CBitmapItemComboBox m_cmbHighLimit;		//���Ѱ�					�޺�
	CBitmapItemComboBox m_cmbLowLimit;		//���Ѱ�					�޺�
	CBitmapItemComboBox m_cmbPivot2Rst;		// �Ǻ� 2�� ����			�޺� 
	CBitmapItemComboBox m_cmbPivot1Rst;		// �Ǻ� 1�� ����			�޺�
	CBitmapItemComboBox m_cmbPivotGLine;	// �Ǻ� ���ؼ�				�޺�
	CBitmapItemComboBox m_cmbPivot1GG;		// �Ǻ� 1�� ����			�޺�
	CBitmapItemComboBox m_cmbPivot2GG;		// �Ǻ� 2�� ����			�޺�	
	CBitmapItemComboBox m_cmbDMKPreHPrice;	// Demark �����			�޺�
	CBitmapItemComboBox m_cmbDMKGLine;		// Demark ���ؼ�			�޺�
	CBitmapItemComboBox m_cmbDMKPreLPrice;	// Demark ��������			�޺�	
	CBitmapItemComboBox m_cmb2Pyung1;		// �̵���� ù��° �Է°�	�޺�
	CBitmapItemComboBox m_cmb2Pyung2;		// �̵���� �ι�° �Է°�	�޺�
	CBitmapItemComboBox m_cmb2Pyung3;		// �̵���� ����° �Է°�	�޺�
	CBitmapItemComboBox m_cmb2Pyung4;		// �̵���� �׹�° �Է°�	�޺�	
	CBitmapItemComboBox m_cmbPmmDanga;		// ��ո��� �ܰ�			�޺�
	CBitmapItemComboBox m_cmbPmmPDanga;		// ��ո��� �ܰ� +n		�޺�
	CBitmapItemComboBox m_cmbPmmMDanga;		// ��ո��� �ܰ� -n		�޺�	
	CComboBox			m_cmbAlign;			// ����ǥ�� (����/������/����)
	BOOL	m_bChkName;						// ����ǥ�� : �̸�	üũ ��ư       
	BOOL	m_bChkValue;					// ����ǥ�� : ��	üũ ��ư       
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetGuideLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetGuideLine)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnColor(UINT nID);
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETGUIDELINE_H__566DA12E_84F8_41DE_8E1F_60BF51C2CC32__INCLUDED_)
