#if !defined(AFX_NUMERICALINQUIRYOPTIONDLG_H__7332B38D_3E88_4A1A_B537_EA4B6C588037__INCLUDED_)
#define AFX_NUMERICALINQUIRYOPTIONDLG_H__7332B38D_3E88_4A1A_B537_EA4B6C588037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumericalInquiryOptionDlg.h : header file
// NOTE: ����(04/03/17) ��ġ��ȸ����
// ��ġ��ȸ���� �ɼ� ���������� ���̾�α׷μ� ��ġ��ȸâ���, ���ڼ����, ��ġ��ȸ�������
// �ɼ��� �����ϰ� �ɼǰ��� �ϳ��� ����(��Ʈ������ ��)�� ��� ������Ƽ�������� ������.

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryOptionDlg dialog
class CToolandRButtonTabCtrl;
class CNumericalInquiryOptionDlg : public CDialog
{
// Construction
public:
	CNumericalInquiryOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNumericalInquiryOptionDlg)
	enum { IDD = IDD_DIALOG_NUMERICALINQUIRYOPTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumericalInquiryOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNumericalInquiryOptionDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCheckTooltip();
	afx_msg void OnCheckCrosslineview();
	afx_msg void OnRdoNumericalinquiry();
	afx_msg void OnRdoDragzoom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


// Function
public:
	int	 GetNumericalInquiryOption();
	void AddNumericalInquiryOption(int nOption);
	void SubNumericalInquiryOption(int nOption);

// Variable
private:
	int m_nOption;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMERICALINQUIRYOPTIONDLG_H__7332B38D_3E88_4A1A_B537_EA4B6C588037__INCLUDED_)
