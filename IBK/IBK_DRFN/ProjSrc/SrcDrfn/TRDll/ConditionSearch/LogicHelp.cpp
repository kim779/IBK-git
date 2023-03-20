// LogicHelp.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogicHelp.h"
#include "MainFrmWnd.h"

extern HINSTANCE g_hInstance;
// CLogicHelp ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLogicHelp, CDialog)

CLogicHelp::CLogicHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CLogicHelp::IDD, pParent)
{
	m_pWndMainFrm = pParent;

	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
}

CLogicHelp::~CLogicHelp()
{
	m_brushStatic.DeleteObject();
}

void CLogicHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*DDX_Control(pDX, IDC_LIST1, m_ctrlList);*/
	/*DDX_Control(pDX, IDC_EDIT1, m_ctrlHelpText);*/
}


BEGIN_MESSAGE_MAP(CLogicHelp, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, OnBtnOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()

	ON_WM_CREATE()
//	ON_WM_CREATE()
END_MESSAGE_MAP()


// CLogicHelp �޽��� ó�����Դϴ�.

void CLogicHelp::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*ChangeHelpText(m_ctrlList.GetCurSel());*/
}

void CLogicHelp::ChangeHelpText(int nSel)
{
	/*m_ctrlHelpText.SetWindowText((LPCTSTR)m_arrHelpText.GetAt(nSel));*/
	Invalidate();
}

void CLogicHelp::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CLogicHelp::OnInitDialog()
{
	CDialog::OnInitDialog();

	CConditionMakeWnd* pMakeWnd = (CConditionMakeWnd*)m_pWndMainFrm;
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)pMakeWnd->m_pwndMain;
	m_clrMainSkin = pMainWnd->GetAxColor(66);

	m_brushStatic.CreateSolidBrush(m_clrMainSkin);

	m_btnOk.Create(_T("�ݱ�"), CRect(0, 0, 0, 0), this, IDOK);
	pMakeWnd->SetBtnImg(&m_btnOk, 4);

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
// 	m_ctrlList.AddString("1. ���ǽ�");
// 	m_ctrlList.AddString("2. ���ǽ� �׷�����");
// 	m_ctrlList.AddString("3. ����");
// 	m_ctrlList.AddString("4. ���ɱ׷�");
// 	m_ctrlList.AddString("5. ��ǥ��, A..z");

	CString szHelpText, strTemp;




// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n\r\n%s\r\n", 
// 		"[���ǽ�]",
// 		"���ǽ� �ʵ忡�� �߰��� ���ǵ��� ��� A, B, C ������ ǥ�õ˴ϴ�.",
// 		"�� ���� ������ and//or�� ���� Ŭ���ϸ�, ���չ���� ������ �� �ֽ��ϴ�."
// 		);
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n",
// 		"[���ǽ� �׷�����]",
// 		"���콺�� �̿��Ͽ�, ��ȣ(�׷�)�� ���� �������� �巡�� �մϴ�."
// 		);
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n",
// 		"[����]",
// 		"�Ϲݰ˻� ���ǽ��� and//or�� ������������ ���� ����������, �����˻� ���ǽ��� �ð������̹Ƿ� and�������θ� ���� �����մϴ�."
// 		);
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n\r\n%s\r\n",
// 		"[���ɱ׷�]",
// 		"���ɱ׷��� �����ϴ� ���, �ۼ��� ���ǽ��� ������ ��������׷� �� ���� �ȿ��� �˻��մϴ�.",
// 		"�� ���ɱ׷��� ���ǽ��� and//or�� ������������ ��ȸ �Ұ����ϸ� and�� or�������θ� ��ȸ �����մϴ�.");
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n",
// 		"[��ǥ��] A..z",
// 		"�� ���ǽĿ��� ��� ������ �ִ� ���� ���� 48���Դϴ�."
// 		);
// 	m_arrHelpText.Add(szHelpText);
	strTemp.Format("\r\n%s", "1. ���ǽ�");
	/*m_ctrlHelpText.SetFont(&m_fontBold);*/
	/*m_ctrlHelpText.SetWindowText(strTemp);*/
	/*int nTemp = m_ctrlHelpText.GetWindowTextLength();*/
// 	m_ctrlHelpText.SetSel(0, nTemp);
// 	m_ctrlHelpText.SetSelectionCharFormat()


// 	strTemp.Format("\r\n%s\r\n%s\r\n\r\n", 
// 		"���ǽ� �ʵ忡�� �߰��� ���ǵ��� ��� A, B, C ������ ǥ�õ˴ϴ�.",
// 		"�� ���� ������ and/or�� ���� Ŭ���ϸ�, ���չ���� ������ �� �ֽ��ϴ�."
// 		);
// 	m_ctrlHelpText.SetFont(&m_fontDefault, FALSE);
// 	int nLength = m_ctrlHelpText.GetWindowTextLength();
// 	m_ctrlHelpText.SetSel(nLength, nLength);
// 	m_ctrlHelpText.ReplaceSel(strTemp);

// 	szHelpText.Format("%s", strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s\r\n\r\n",
// 		"2. ���ǽ� �׷�����",
// 		"���콺�� �̿��Ͽ�, ��ȣ(�׷�)�� ���� �������� �巡�� �մϴ�."
// 		);
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s\r\n\r\n",
// 		"3. ����",
// 		"�Ϲݰ˻� ���ǽ��� and//or�� ������������ ���� ����������, �����˻� ���ǽ��� �ð������̹Ƿ� and�������θ� ���� �����մϴ�."
// 		);
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s\r\n%s\r\n\r\n",
// 		"4. ���ɱ׷�",
// 		"���ɱ׷��� �����ϴ� ���, �ۼ��� ���ǽ��� ������ ��������׷� �� ���� �ȿ��� �˻��մϴ�.",
// 		"�� ���ɱ׷��� ���ǽ��� and//or�� ������������ ��ȸ �Ұ����ϸ� and�� or�������θ� ��ȸ �����մϴ�.");
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s",
// 		"5. ��ǥ��, A..z",
// 		"�� ���ǽĿ��� ��� ������ �ִ� ���� ���� 25���Դϴ�."
// 		);
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 
// 	m_ctrlList.SetCurSel(0);
// 	m_ctrlList.ShowWindow(SW_HIDE);
// 	//m_ctrlHelpText.SetWindowText((LPCTSTR)m_arrHelpText.GetAt(0));
// 	m_ctrlHelpText.SetFont(&m_fontBold);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
	
	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CLogicHelp::OnBtnOk()
{
	OnOK();
}

void CLogicHelp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, m_clrMainSkin);
}

HBRUSH CLogicHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
// 	if(pWnd->m_hWnd == m_ctrlList.m_hWnd)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		pDC->SetBkColor(m_clrMainSkin);
// 		hbr = m_brushStatic;
// 	}
	
	return hbr;
}

void CLogicHelp::OnSize()
{
	CRect reClient, reLeft;
	GetClientRect(reClient);
	
	reClient.left += 5;
	reClient.top += 5;
	reClient.right -= 5;
	reClient.bottom -= 30;

	m_stcBkg.MoveWindow(reClient);

	reClient.left += 8;
	reClient.top += 8;
	reClient.right -= 8;
	
	reClient.bottom = reClient.top + 14;
	m_stcTitle1.MoveWindow(reClient);
	
	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo1_1.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo1_2.MoveWindow(reClient);

	reClient.top = reClient.bottom + 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle2.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo2_1.MoveWindow(reClient);

	reClient.top = reClient.bottom + 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle3.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo3_1.MoveWindow(reClient);

	reClient.top = reClient.bottom+ 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle4.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo4_1.MoveWindow(reClient);

	reClient.top = reClient.bottom + 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle5.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo5_1.MoveWindow(reClient);

// 	reLeft = reClient;
// 	
// 	reLeft.top = reClient.bottom - 25;
// 	reLeft.bottom = reLeft.top + 20;
// 	
// 	reLeft.right = reClient.right - 5;
// 	reLeft.left = reLeft.right - 60;
// 	m_btnOk.MoveWindow(&reLeft);
// 	m_btnOk.ShowWindow(SW_HIDE);
	
	/*m_ctrlHelpText.ShowWindow(SW_HIDE);*/
	Invalidate();
}

int CLogicHelp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) 
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CRect rtNULL(0, 0, 0, 0);
	
	m_stcBkg.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, rtNULL, this);
	m_stcBkg.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_stcTitle1.Create(_T("1. ���ǽ�"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle1.SetFont(&m_fontBold);
	m_stcTitle2.Create(_T("2. ���ǽ� �׷�����"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle2.SetFont(&m_fontBold);
	m_stcTitle3.Create(_T("3. ����"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle3.SetFont(&m_fontBold);
	m_stcTitle4.Create(_T("4. ���ɱ׷�"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle4.SetFont(&m_fontBold);
	m_stcTitle5.Create(_T("5. ��ǥ��, A..z"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle5.SetFont(&m_fontBold);

	m_stcInfo1_1.Create(_T("���ǽ� �ʵ忡�� �߰��� ���ǵ��� ��� A, B, C ������ ǥ�õ˴ϴ�."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo1_1.SetFont(&m_fontDefault);
	
	m_stcInfo1_2.Create(_T("�� ���� ������ and/or�� ���� Ŭ���ϸ�, ���չ���� ������ �� �ֽ��ϴ�."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo1_2.SetFont(&m_fontDefault);
	
	m_stcInfo2_1.Create(_T("���콺�� �̿��Ͽ�, ��ȣ(�׷�)�� ���� �������� �巡�� �մϴ�."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo2_1.SetFont(&m_fontDefault);
	
	m_stcInfo3_1.Create(_T("�Ϲݰ˻� ���ǽ��� and/or�� ������������ ���� ����������, �����˻� ���ǽ��� �ð������̹Ƿ� and�������θ� ���� �����մϴ�."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo3_1.SetFont(&m_fontDefault);
	
	m_stcInfo4_1.Create(_T("���ɱ׷��� �����ϴ� ���, �ۼ��� ���ǽ��� ������ ��������׷� �� ���� �ȿ��� �˻��մϴ�."),
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo4_1.SetFont(&m_fontDefault);
	
	m_stcInfo4_2.Create(_T("�� ���ɱ׷��� ���ǽ��� and/or�� ������������ ��ȸ �Ұ����ϸ� and�� or�������θ� ��ȸ �����մϴ�."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo4_2.SetFont(&m_fontDefault);
	
	m_stcInfo5_1.Create(_T("�� ���ǽĿ��� ��� ������ �ִ� ���� ���� 25���Դϴ�."), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo5_1.SetFont(&m_fontDefault);

	AfxSetResourceHandle(hInstSave);

	OnSize();

	return 0;
}