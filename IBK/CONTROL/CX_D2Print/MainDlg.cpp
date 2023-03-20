// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_D2Print.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DATACOUNT 26	//2012.06.21 KSJ �ι�° ���������� ���������� ���� �����

/////////////////////////////////////////////////////////////////////////////
// MainDlg dialog


MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MainDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(MainDlg)
	//}}AFX_DATA_INIT

	m_nTotalPages = 0;
	m_nNowPage = 0;
	m_bFlag = FALSE;
}


void MainDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MainDlg)
	DDX_Control(pDX, IDC_EDIT1, m_editStatus);
	DDX_Control(pDX, IDCANCEL, m_btnOK);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	//{{AFX_MSG_MAP(MainDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(MainDlg, CDialog)
	//{{AFX_DISPATCH_MAP(MainDlg)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B55F180D-37A0-4B94-B87D-14AEFA5A607D}
static const IID IID_IMainDlg =
{ 0xb55f180d, 0x37a0, 0x4b94, { 0xb8, 0x7d, 0x14, 0xae, 0xfa, 0x5a, 0x60, 0x7d } };

BEGIN_INTERFACE_MAP(MainDlg, CDialog)
	INTERFACE_PART(MainDlg, IID_IMainDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainDlg message handlers

void MainDlg::Run()
{
	LoadData();

	SetTimer(100,100,NULL);	
}

void MainDlg::SetProgressRange(int iMax, int iMin) 
{
	m_Progress.SetRange(iMin, iMax); 
	m_Progress.SetPos(iMin); 
}

void MainDlg::SetProgressPos(int iPos) 
{
	if (iPos < 0)
	{
		int	ilower=0, iupper=0;
		m_Progress.GetRange(ilower, iupper);
		iPos = iupper;
	}
	else
		iPos += m_Progress.GetPos();

	m_Progress.SetPos(iPos); 
}

void MainDlg::Start()
{
	std::unique_ptr<CPrintDialog> pDlg = std::make_unique<CPrintDialog>(FALSE);
	if (pDlg->DoModal() == IDCANCEL)
	{
		pDlg.reset();
		OnCancel();
		return;
	}

	CPrintInfo pInfo;
	DOCINFO	docinfo{};

	docinfo.cbSize= sizeof(docinfo);
	docinfo.lpszDocName = _T("D+2 �ܰ� ���");

	pInfo.SetMinPage(1);
	pInfo.SetMaxPage(m_nTotalPages);
	SetProgressRange(m_nTotalPages, 0);
	//pInfo.m_rectDraw.SetRect(0, 0, 500, 800); 

	CDC* pDC = CDC::FromHandle(pDlg->GetPrinterDC());
	SetPrintAlign(pDC,pDlg->GetPrinterDC());
	if (pDC->StartDoc(&docinfo) < 0)
	{
		//SetPrinterStatus("�μ⸦ ������ �� �����ϴ�.");
		return ;
	}

	for (int ii = 0; ii < m_nTotalPages; ii++)
	{
		SetProgressPos(ii+1);
		pDC->StartPage();

		m_nNowPage = ii + 1;
		DrawStuff(pDC);
		pDC->EndPage();
	}
	pDC->EndDoc();
	pDlg.reset();
	
	//SetTextAlign(hdc, TA_UPDATECP);

	pDC->DeleteDC();
	OnCancel();
}

void MainDlg::SetPrintAlign(CDC *pDC, HDC hdcPrn)
{
	const	int	cxPage = ::GetDeviceCaps (hdcPrn, HORZRES) ;
	const	int	cyPage = ::GetDeviceCaps (hdcPrn, VERTRES) ;

	//pDC->SetMapMode (MM_ANISOTROPIC) ;
	SetMapMode(hdcPrn,MM_ANISOTROPIC);
	//pDC->SetWindowExt (2100, 2970) ;
	SetWindowExtEx(hdcPrn,2300,2970,NULL);
	//pDC->SetViewportExt (cxPage, cyPage) ;
	SetViewportExtEx(hdcPrn,cxPage,cyPage,NULL);

	const	int	nMarginX = (int)(50.0f / 2300.0f * (float)cxPage);
	const	int	nMarginY = (int)(150.0f / 2970.0f * (float)cyPage);

	//pDC->SetViewportOrg (nMarginX,  nMarginY) ;
	SetViewportOrgEx(hdcPrn,nMarginX,nMarginY,NULL);
	//pDC->SetTextAlign (TA_BASELINE | TA_CENTER) ;
}

//nAlign = 0 : LEFT,nAlign = 1 : RIGHT,nAlign = 2 : CENTER
void MainDlg::DrawBox(CDC* pDC,int x1,int y1,int x2,int y2,LPCTSTR text,BOOL MULTI,int nBold,BOOL BKMODE,int nAlign)
{
	CFont font;
	int	nFontSize = 30;

	if (nBold == FW_BOLD)
		nFontSize = 32;

	font.CreateFont(   nFontSize,			// nHeight
			   0,				// nWidth
			   0,				// nEscapement
			   0,				// nOrientation
			   nBold,			// nWeight
			   FALSE,			// bItalic
			   FALSE,			// bUnderline
			   0,				// cStrikeOut
			   ANSI_CHARSET,		// nCharSet
			   OUT_DEFAULT_PRECIS,		// nOutPrecision
			   CLIP_DEFAULT_PRECIS,		// nClipPrecision
			   DEFAULT_QUALITY,		// nQuality
			   DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			   "����ü");			// lpszFacename

	pDC->Rectangle(x1,y1,x2,y2);

	CRect	rect(x1+4,y1+1,x2-4,y2-1);
	CFont*	pOldFont = (CFont*)pDC->SelectObject(&font);

	if (BKMODE == TRUE)
		FillSolidBox(pDC,x1,y1,x2-x1,y2-y1,1);

	if (MULTI == TRUE)
	{
		CString tmp(text);

		rect.top = rect.top + 2;
		pDC->DrawText(text,-1,rect,DT_CENTER | DT_VCENTER);
	}
	else
	{
		if (nAlign == 0)
		{
			pDC->DrawText(text,-1,rect,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (nAlign == 1)
		{
			pDC->DrawText(text,-1,rect,DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			pDC->DrawText(text,-1,rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	pDC->SelectObject(pOldFont);
	DeleteObject(&font);
}

void MainDlg::DrawLine(CDC* pDC,POINT sPoint,POINT ePoint)
{
	pDC->MoveTo(sPoint);
	pDC->LineTo(ePoint);
}

// 2012.04.23 KSJ 2�� ������
void MainDlg::DrawTopBox(CDC* pDC)
{
	CString line;
	CStringArray ar;
	CPen	pen(PS_SOLID, 2, RGB(0,0,0));
	CPen*	pOldPen = pDC->SelectObject(&pen);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(BLACKTEXT);
	
	line = m_arData.GetAt(0);
	SplitString(line,"\t",ar);

	//����� ��������
	int	nTop = 0;
	int	nBottom = 50;

	DrawBox(pDC,0,nTop,200,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);
// 	FillSolidBox(pDC,0,0,316,50,0);		// ���¹�ȣ 

	ar[1].Insert(3,"-");
	ar[1].Insert(6,"-");
	DrawBox(pDC,200,nTop,500,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);
// 	FillSolidBox(pDC,316,0,316,50,0);		// ���¹�ȣ ��

	DrawBox(pDC,500,nTop,700,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,TRUE,2);
// 	FillSolidBox(pDC,632,0,316,50,0);	// ���¸�

	DrawBox(pDC,700,nTop,1000,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);
	// 	FillSolidBox(pDC,632,0,316,50,0);	// ���¸� ��

	DrawBox(pDC,1000,nTop,1200,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);
// 	FillSolidBox(pDC,948,0,316,50,0);	// �������

	DrawBox(pDC,1200,nTop,1600,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);
// 	FillSolidBox(pDC,1264,0,316,50,0);	// ������� ��
	//����� �������� ��

	//���̿� 30��ŭ ���

	//��� ���� �Ѿ�������
	ar.RemoveAll();
	line = m_arData.GetAt(1);
	SplitString(line,"\t",ar);

	nTop = 80;
	nBottom = 130;
	DrawBox(pDC,0,nTop,317,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//��Ź�ڻ��Ѿ�(D+2)
	DrawBox(pDC,317,nTop,634,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,TRUE,2);	//�ֽ����Ѿ�(D+2)
	DrawBox(pDC,634,nTop,951,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,TRUE,2);	//�ֽĸż��Ѿ�(D+2)
	DrawBox(pDC,951,nTop,1268,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,TRUE,2);	//�򰡼���
	DrawBox(pDC,1268,nTop,1585,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);	//BEP����
	DrawBox(pDC,1585,nTop,1902,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,TRUE,2);	//�Ѽ��ͷ�(%)
	DrawBox(pDC,1902,nTop,2230,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,TRUE,2);	//BEP���ͷ�(%)
	//��� ���� �Ѿ������� ��
	

	//��� ���� �Ѿ����� ��
	ar.RemoveAll();
	line = m_arData.GetAt(2);
	SplitString(line,"\t",ar);

	nTop = 130;
	nBottom = 180;
	DrawBox(pDC,0,nTop,317,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,FALSE,1);	//��Ź�ڻ��Ѿ�(D+2)
	DrawBox(pDC,317,nTop,634,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);	//�ֽ����Ѿ�(D+2)
	DrawBox(pDC,634,nTop,951,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,FALSE,1);	//�ֽĸż��Ѿ�(D+2)
	DrawBox(pDC,951,nTop,1268,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);	//�򰡼���
	DrawBox(pDC,1268,nTop,1585,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,FALSE,1);	//BEP����
	DrawBox(pDC,1585,nTop,1902,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);	//�Ѽ��ͷ�(%)
	DrawBox(pDC,1902,nTop,2230,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,FALSE,1);	//BEP���ͷ�(%)
	//��� ���� �Ѿ����� �� ��

	//���̿� 30��ŭ ���

	//��� ���� �ܰ����� 1��
	ar.RemoveAll();
	line = m_arData.GetAt(3);
	SplitString(line,"\t",ar);

	nTop = 210;
	nBottom = 260;
	DrawBox(pDC,0,nTop,200,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//����
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,TRUE,2);	//������
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,TRUE,2);	//����
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,TRUE,2);	//��ݰ��ɱݾ�
	//���̿� 50 ���
	DrawBox(pDC,1378,nTop,1578,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);	//����ݾ�
	DrawBox(pDC,1578,nTop,1954,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,TRUE,2);	//�ŵ�����
	DrawBox(pDC,1954,nTop,2230,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,TRUE,2);	//�ż�����
	//��� ���� �ܰ����� 1�� ��


	//��� ���� �ܰ����� 2��
	ar.RemoveAll();
	line = m_arData.GetAt(4);
	SplitString(line,"\t",ar);

	nTop = 260;
	nBottom = 310;
	DrawBox(pDC,0,nTop,200,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//����
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);	//������ ��
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,FALSE,1);	//���� ��
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);	//��ݰ��ɱݾ� ��
	//���̿� 50 ���
	DrawBox(pDC,1378,nTop,1578,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);	//����
	DrawBox(pDC,1578,nTop,1954,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);	//�ŵ����� ��
	DrawBox(pDC,1954,nTop,2230,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,FALSE,1);	//�ż����� ��
	//��� ���� �ܰ����� 2�� ��

	
	//��� ���� �ܰ����� 3��
	ar.RemoveAll();
	line = m_arData.GetAt(5);
	SplitString(line,"\t",ar);

	nTop = 310;
	nBottom = 360;
	DrawBox(pDC,0,nTop,200,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//D+1(����)
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);	//������ ��
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,FALSE,1);	//���� ��
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);	//��ݰ��ɱݾ� ��
	//���̿� 50 ���
	DrawBox(pDC,1378,nTop,1578,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);	//����
	DrawBox(pDC,1578,nTop,1954,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);	//�ŵ����� ��
	DrawBox(pDC,1954,nTop,2230,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,FALSE,1);	//�ż����� ��
	//��� ���� �ܰ����� 3�� ��

	//��� ���� �ܰ����� 4��
	ar.RemoveAll();
	line = m_arData.GetAt(6);
	SplitString(line,"\t",ar);
	
	nTop = 360;
	nBottom = 410;
	DrawBox(pDC,0,nTop,200,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//D+2(����)
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);	//������ ��
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,FALSE,1);	//���� ��
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);	//��ݰ��ɱݾ� ��
	//���̻� �׸� ���� ����
	//��� ���� �ܰ����� 4�� ��

	//��� ���� �ܰ����� 5��
	ar.RemoveAll();
	line = m_arData.GetAt(7);
	SplitString(line,"\t",ar);
	
	nTop = 410;
	nBottom = 460;
	DrawBox(pDC,0,nTop,200,nBottom+50,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//�Ÿż���
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,TRUE,2);	//���ϸŸ�
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,TRUE,2);	//���ϸŸ�
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,TRUE,2);	//�����հ�
	//���̿� 50 ���
	DrawBox(pDC,1378,nTop,1578,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);	//�ſ�(D)
	DrawBox(pDC,1578,nTop,1954,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,TRUE,2);	//����
	DrawBox(pDC,1954,nTop,2230,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,TRUE,2);	//����
	//��� ���� �ܰ����� 5�� ��

	//��� ���� �ܰ����� 6��
	ar.RemoveAll();
	line = m_arData.GetAt(8);
	SplitString(line,"\t",ar);
	
	nTop = 460;
	nBottom = 510;
	//DrawBox(pDC,0,nTop,200,nBottom+50,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//�Ÿż���
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,FALSE,1);	//���ϸŸ� ��
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);	//���ϸŸ� ��
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,FALSE,1);	//�����հ� ��
	//���̿� 50 ���
	DrawBox(pDC,1378,nTop,1578,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,TRUE,2);	//�հ�
	DrawBox(pDC,1578,nTop,1954,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,FALSE,1);	//���� ��
	DrawBox(pDC,1954,nTop,2230,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);	//���� ��
	//��� ���� �ܰ����� 6�� ��

	//���̿� 30 ���

	//��� ���� �ܰ����� 7��
	ar.RemoveAll();
	line = m_arData.GetAt(9);
	SplitString(line,"\t",ar);
	
	nTop = 540;
	nBottom = 590;
	DrawBox(pDC,0,nTop,200,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//���ű�����
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);	//���ű����� ��
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,TRUE,2);	//���űݴ�� 
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);	//���űݴ�� ��
	//���̿� 50 ���
	DrawBox(pDC,1378,nTop,1591,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);	//�㺸�����ݾ�
	DrawBox(pDC,1591,nTop,1804,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);	//�㺸�����ݾ� ��
	DrawBox(pDC,1804,nTop,2017,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,TRUE,2);	//���ݹ̼��ݾ� 
	DrawBox(pDC,2017,nTop,2230,nBottom,ar.GetAt(7),FALSE,FW_NORMAL,FALSE,1);	//���ݹ̼��ݾ� ��
	//��� ���� �ܰ����� 7�� ��

	//��� ���� �ܰ����� 8��
	ar.RemoveAll();
	line = m_arData.GetAt(10);
	SplitString(line,"\t",ar);
	
	nTop = 590;
	nBottom = 640;
	DrawBox(pDC,0,nTop,200,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);	//�㺸����
	DrawBox(pDC,200,nTop,576,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);	//�㺸���� ��
	DrawBox(pDC,576,nTop,952,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,TRUE,2);	//�㺸��� 
	DrawBox(pDC,952,nTop,1328,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);	//�㺸��� ��
	//���̿� 50 ���
	DrawBox(pDC,1378,nTop,1591,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);	//���㺸��
	DrawBox(pDC,1591,nTop,1804,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);	//���㺸�� ��
	DrawBox(pDC,1804,nTop,2017,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,TRUE,2);	//�㺸����(%) 
	DrawBox(pDC,2017,nTop,2230,nBottom,ar.GetAt(7),FALSE,FW_NORMAL,FALSE,1);	//�㺸����(%) ��
	//��� ���� �ܰ����� 8�� ��

	pDC->SelectObject(pOldPen);
	
	DeleteObject(&pen);
}
// KSJ

/* 2012.04.23 KSJ 1������
// 2012.01.04 KSJ 1��������
void MainDlg::DrawTopBox(CDC* pDC,double dpiRatio)
{
	CFont* pOldFont;

	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	POINT startP,endP;
	
	startP.x = 0;
	startP.y = 0;
	
	endP.x = 2100;
	endP.y = 0;
	
	DrawLine(pDC,startP,endP);
	
	startP.x = 0;
	startP.y = 350;
	
	endP.x = 2100;
	endP.y = 350;
	
	DrawLine(pDC,startP,endP);

	//Row�� ���� �ߴ´�.
	for (int nRow = 1; nRow < 6; nRow++)
	{
		startP.x = 0;
		startP.y = 50 * nRow;
		
		endP.x = 2100;
		endP.y = 50 * nRow;
		
		DrawLine(pDC,startP,endP);
	}

	//
	pDC->SetBkMode(TRANSPARENT);
	
	pDC->SetTextColor(BLACKTEXT);
	//

	CString line;
	
	CStringArray ar;
	
	line = m_arData.GetAt(0);
	SplitString(line,"\t",ar);

	//����� ��������
	DrawBox(pDC,0,0,300,50,ar.GetAt(0),FALSE,FW_BOLD,TRUE,2);
// 	FillSolidBox(pDC,0,0,316,50,0);		// ���¹�ȣ 

	ar[1].Insert(3,"-");
	ar[1].Insert(6,"-");
	DrawBox(pDC,300,0,550,50,ar.GetAt(1),FALSE,FW_BOLD,FALSE,2);
// 	FillSolidBox(pDC,316,0,316,50,0);		// ���¹�ȣ ��

	DrawBox(pDC,550,0,800,50,ar.GetAt(2),FALSE,FW_BOLD,TRUE,2);
// 	FillSolidBox(pDC,632,0,316,50,0);	// ���¸� 

	DrawBox(pDC,800,0,1300,50,ar.GetAt(3),FALSE,FW_BOLD,FALSE,0);
// 	FillSolidBox(pDC,948,0,316,50,0);	// ���¸� ��

	DrawBox(pDC,1300,0,1600,50,ar.GetAt(4),FALSE,FW_BOLD,TRUE,2);
// 	FillSolidBox(pDC,1264,0,316,50,0);	// ������� 

	DrawBox(pDC,1600,0,2100,50,ar.GetAt(5),FALSE,FW_BOLD,FALSE,1);
// 	FillSolidBox(pDC,1580,0,320,50,0);	// ������� ��
	//����� �������� ��

	
	//��� ���� �ܰ����� 1��
	ar.RemoveAll();
	line = m_arData.GetAt(1);
	SplitString(line,"\t",ar);

	DrawBox(pDC,0,50,120,200,ar.GetAt(0),FALSE,FW_BOLD,TRUE,2);	//������
	DrawBox(pDC,120,50,300,100,ar.GetAt(1),FALSE,FW_BOLD,TRUE,2);	//����
	DrawBox(pDC,300,50,550,100,ar.GetAt(2),FALSE,FW_BOLD,FALSE,1);	//���� ��
	DrawBox(pDC,550,50,800,100,ar.GetAt(3),FALSE,FW_BOLD,TRUE,2);	//����ݾ�
	DrawBox(pDC,800,50,1050,100,ar.GetAt(4),FALSE,FW_BOLD,TRUE,2);	//�ŵ�����
	DrawBox(pDC,1050,50,1300,100,ar.GetAt(5),FALSE,FW_BOLD,TRUE,2);	//�ż�����
	DrawBox(pDC,1300,50,1600,100,ar.GetAt(6),FALSE,FW_BOLD,TRUE,2);	//�ſ����ݾ�
	DrawBox(pDC,1600,50,2100,100,ar.GetAt(7),FALSE,FW_BOLD,FALSE,1);	//�ſ����ݾ� ��
	//��� ���� �ܰ����� 1�� ��
	

	//��� ���� �ܰ����� 2��
	ar.RemoveAll();
	line = m_arData.GetAt(2);
	SplitString(line,"\t",ar);

	DrawBox(pDC,120,100,300,150,ar.GetAt(0),FALSE,FW_BOLD,TRUE,2);	//D+1 ����
	DrawBox(pDC,300,100,550,150,ar.GetAt(1),FALSE,FW_BOLD,FALSE,1);	//D+1 ��
	DrawBox(pDC,550,100,800,150,ar.GetAt(2),FALSE,FW_BOLD,TRUE,2);	//����
	DrawBox(pDC,800,100,1050,150,ar.GetAt(3),FALSE,FW_BOLD,FALSE,1);	//���� �ŵ����� ��
	DrawBox(pDC,1050,100,1300,150,ar.GetAt(4),FALSE,FW_BOLD,FALSE,1);	//���� �ż����� ��
	DrawBox(pDC,1300,100,1600,150,ar.GetAt(5),FALSE,FW_BOLD,TRUE,2);	//�㺸������
	DrawBox(pDC,1600,100,2100,150,ar.GetAt(6),FALSE,FW_BOLD,FALSE,1);	//�㺸������ ��
	//��� ���� �ܰ����� 2�� ��


	//��� ���� �ܰ����� 3��
	ar.RemoveAll();
	line = m_arData.GetAt(3);
	SplitString(line,"\t",ar);

	DrawBox(pDC,120,150,300,200,ar.GetAt(0),FALSE,FW_BOLD,TRUE,2);	//D+2 ����
	DrawBox(pDC,300,150,550,200,ar.GetAt(1),FALSE,FW_BOLD,FALSE,1);	//D+2 ��
	DrawBox(pDC,550,150,800,200,ar.GetAt(2),FALSE,FW_BOLD,TRUE,2);	//����
	DrawBox(pDC,800,150,1050,200,ar.GetAt(3),FALSE,FW_BOLD,FALSE,1);	//���� �ŵ����� ��
	DrawBox(pDC,1050,150,1300,200,ar.GetAt(4),FALSE,FW_BOLD,FALSE,1);	//���� �ż����� ��
	DrawBox(pDC,1300,150,1600,200,ar.GetAt(5),FALSE,FW_BOLD,TRUE,2);	//�㺸����(ü�����)
	DrawBox(pDC,1600,150,2100,200,ar.GetAt(6),FALSE,FW_BOLD,FALSE,1);	//�㺸����(ü�����) ��
	//��� ���� �ܰ����� 3�� ��


	//��� ���� �ܰ����� 4��
	ar.RemoveAll();
	line = m_arData.GetAt(4);
	SplitString(line,"\t",ar);

	DrawBox(pDC,0,200,300,250,ar.GetAt(0),FALSE,FW_BOLD,TRUE,2);	//��ݰ��ɱݾ�
	DrawBox(pDC,300,200,550,250,ar.GetAt(1),FALSE,FW_BOLD,FALSE,1);	//��ݰ��ɱݾ� ��
	DrawBox(pDC,550,200,800,300,ar.GetAt(2),FALSE,FW_BOLD,TRUE,2);	//�ֹ�����(2ĭ)
	DrawBox(pDC,800,200,1050,250,ar.GetAt(3),FALSE,FW_BOLD,TRUE,2);	//����
	DrawBox(pDC,1050,200,1300,250,ar.GetAt(4),FALSE,FW_BOLD,TRUE,2);	//����(50%)
	DrawBox(pDC,1300,200,1600,250,ar.GetAt(5),FALSE,FW_BOLD,TRUE,2);	//���ݹ̼�
	DrawBox(pDC,1600,200,2100,250,ar.GetAt(6),FALSE,FW_BOLD,FALSE,1);	//���ݹ̼� ��
	//��� ���� �ܰ����� 4�� ��

	
	//��� ���� �ܰ����� 5��
	ar.RemoveAll();
	line = m_arData.GetAt(5);
	SplitString(line,"\t",ar);

	DrawBox(pDC,0,250,300,300,ar.GetAt(0),FALSE,FW_BOLD,TRUE,2);	//���ݾ�
	DrawBox(pDC,300,250,550,300,ar.GetAt(1),FALSE,FW_BOLD,FALSE,1);	//���ݾ� ��
	//DrawBox(pDC,550,250,800,300,ar.GetAt(2),FALSE,FW_BOLD,TRUE,2);	//�ֹ����� ������ �׷�����
	DrawBox(pDC,800,250,1050,300,ar.GetAt(2),FALSE,FW_BOLD,FALSE,1);	//���� �ŵ����� ��
	DrawBox(pDC,1050,250,1300,300,ar.GetAt(3),FALSE,FW_BOLD,FALSE,1);	//���� �ż����� ��
	DrawBox(pDC,1300,250,1600,300,ar.GetAt(4),FALSE,FW_BOLD,TRUE,2);	//���ڹ̳�
	DrawBox(pDC,1600,250,2100,300,ar.GetAt(5),FALSE,FW_BOLD,FALSE,1);	//���ڹ̳� ��
	//��� ���� �ܰ����� 5�� ��

	pDC->SelectObject(pOldPen);
	
	DeleteObject(&pen);
}
*/


/* 2012.01.04 KSJ ����
void MainDlg::DrawTopBox(CDC* pDC,double dpiRatio)
{
	CFont* pOldFont;

	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	POINT startP,endP;
	
	startP.x = 0;
	startP.y = 0;
	
	endP.x = 2100;
	endP.y = 0;
	
	DrawLine(pDC,startP,endP);
	
	startP.x = 0;
	startP.y = 350;
	
	endP.x = 2100;
	endP.y = 350;
	
	DrawLine(pDC,startP,endP);

	startP.x = 0;
	startP.y = 0;
	
	endP.x = 0;
	endP.y = 350;
	
	DrawLine(pDC,startP,endP);
	
// 	startP.x = 316;
// 	startP.y = 0;
// 	
// 	endP.x = 316;
// 	endP.y = 350;
// 	
// 	DrawLine(pDC,startP,endP);

	startP.x = 632;
	startP.y = 0;
	
	endP.x = 632;
	endP.y = 350;
	
	DrawLine(pDC,startP,endP);
	
// 	startP.x = 948;
// 	startP.y = 0;
// 	
// 	endP.x = 948;
// 	endP.y = 350;
// 	
// 	DrawLine(pDC,startP,endP);

	startP.x = 1264;
	startP.y = 0;
	
	endP.x = 1264;
	endP.y = 350;
	
	DrawLine(pDC,startP,endP);
	
// 	startP.x = 1580;
// 	startP.y = 0;
// 	
// 	endP.x = 1580;
// 	endP.y = 350;
// 	
// 	DrawLine(pDC,startP,endP);

	startP.x = 2100;
	startP.y = 0;

	endP.x = 2100;
	endP.y = 350;

	DrawLine(pDC,startP,endP);

	FillSolidBox(pDC,0,0,316,50,0);
	FillSolidBox(pDC,632,0,316,50,0);
	FillSolidBox(pDC,1264,0,316,50,0);

	FillSolidBox(pDC,0,60,316,290,1);

	FillSolidBox(pDC,632,60,316,290,1);

	FillSolidBox(pDC,1264,60,316,290,1);
	
	pDC->SelectObject(pOldPen);
	
	DeleteObject(&pen);

	CFont font;
	VERIFY(font.CreateFont(
		35,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"����ü"));                 // lpszFacename
	
	pOldFont = (CFont*)pDC->SelectObject(&font);
	
	CRect rect;
	rect.top = 0;
	rect.left = 0;
	rect.bottom = 50;
	rect.right = 316;

	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(BLACKTEXT);
	
	for (int i=0;i<6;i++)
	{
		CString line = m_arData.GetAt(i);

		CStringArray ar;

		SplitString(line,"\t",ar);

		for (int j=0;j<3;j++)
		{
			CString strToken = ar.GetAt(j);

			CStringArray arToken;

			SplitString(strToken,":",arToken); 

			pDC->DrawText(arToken[0], -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			rect.left = rect.right;
			rect.right = rect.left + 316;

			if (i == 0 && j == 0)
			{
				arToken[1].Insert(3,"-");
				arToken[1].Insert(6,"-");

				pDC->DrawText(arToken[1], -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (i == 0 && j == 1)
			{
				pDC->DrawText(arToken[1], -1, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
			else if (i == 1 && j == 2)
			{
				pDC->DrawText(arToken[1], -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				pDC->DrawText(arToken[1], -1, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			}

			rect.left = rect.right;
			rect.right = rect.left + 316;
		}

		if (i < 4)
		{
			rect.top = rect.top + 60;
		}
		else
		{
			rect.top = rect.top + 50;
		}
		rect.left = 0;
		rect.bottom = rect.top + 50;
		rect.right = 316;
	}	
	
	pDC->SelectObject(pOldFont);
	
	DeleteObject(&font);
}
*/

void MainDlg::DrawGrid(CDC* pDC)
{
	//CString topHeader[] = {"����","�����ȣ","�ܰ����","D+2�� �ܰ�","������հ�","���Աݾ�","���ͷ�","���ϸż�ü��","���ϸŵ�ü��","������","�����","�ֹ����ɼ���","�ſ�ݾ�","���簡","�򰡱ݾ�","�ŵ����ͱݾ�","�ż�����","�ŵ�����"};
	if (m_nNowPage == 1)
	{
		DrawGridHeader(pDC);
		if (m_arData.GetSize() > 10)
			DrawGridData(pDC);
		
	}
	else
	{
		DrawGridHeader(pDC, false);	//2012.06.21 KSJ �ι�° ���������� ��� ����ϵ��� �߰���.
		DrawGridData(pDC);
	}
	
}

void MainDlg::DrawGridData(CDC* pDC)
{
	CString	tmp;
	int	y = 0;
	CStringArray ar;

	m_bFlag = FALSE;
	if (m_arData.GetSize() < 30)
	{
		y = 770;
		for (int ii = 11; ii < m_arData.GetSize(); ii++)
		{
			ar.RemoveAll();
			tmp = m_arData.GetAt(ii);
			SplitString(tmp,":",ar);
			if (ar.GetSize() == 26)
				DrawGridLine(pDC,ar,y);
			y += 100;
		}
	}
	else
	{
		if (m_nNowPage == 1)
		{
			y = 770;
			for (int ii = 11; ii < 30; ii++)
			{
				ar.RemoveAll();
				tmp = m_arData.GetAt(ii);
				SplitString(tmp,":",ar);
				if (ar.GetSize() == 26)
					DrawGridLine(pDC,ar,y);
				y += 100;
			}
		}
		else
		{
			y = 100;

			const	int	nStart = 30 + (m_nNowPage - 2) * DATACOUNT;
			for (int ii = nStart; ii < nStart + DATACOUNT; ii++)
			{
				if (ii > m_arData.GetUpperBound())
					break;

				ar.RemoveAll();
				tmp = m_arData.GetAt(ii);
				SplitString(tmp,":",ar);
				if (ar.GetSize() == 26)
					DrawGridLine(pDC,ar,y);
				y += 100;
			}
		}
	}

	if (m_nNowPage == m_nTotalPages)
	{
// 		if (m_arData.GetSize() > 30)
			y = y - 100;

		DrawBottomData(pDC,y);
	}

	DrawPageNumber(pDC);
}

void MainDlg::DrawPageNumber(CDC* pDC)
{
	CFont	font;
	CRect	rect;
	CString	strPage;
	const	int	nFontSize = 30;

	font.CreateFont(nFontSize,			// nHeight
			0,				// nWidth
			0,				// nEscapement
			0,				// nOrientation
			FW_NORMAL,			// nWeight
			FALSE,				// bItalic
			FALSE,				// bUnderline
			0,				// cStrikeOut
			ANSI_CHARSET,			// nCharSet
			OUT_DEFAULT_PRECIS,		// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			DEFAULT_QUALITY,		// nQuality
			DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			"����ü");			// lpszFacename
	
	CFont*	pOldFont = (CFont*)pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(BLACKTEXT);
	
	rect.top = 2770;
	rect.left = 1800;
	rect.bottom = rect.top + 40;
	rect.right = 2100;

	strPage.Format("%d / %d",m_nNowPage,m_nTotalPages);
	pDC->DrawText(strPage,-1,rect,DT_RIGHT | DT_TOP);

	rect.left = 0;
	rect.right = 500;

	pDC->DrawText("IBK �������� D+2 �ܰ�",-1,rect, DT_LEFT | DT_TOP);
	
	pDC->SelectObject(pOldFont);
	
	DeleteObject(&font);
}

void MainDlg::DrawBottomData(CDC* pDC,int y)
{
// 	CString line = m_arData.GetAt(m_arData.GetSize() - 1);
// 	
// 	CStringArray ar;
// 	
// 	SplitString(line,"\t",ar);

	//2012.01.04 KSJ ������ 5���� �ø� ũ�� 190���� ����
// 	if (ar.GetSize() == 5)
// 	{
// 		for (int i=0;i<ar.GetSize();i++)
// 		{
// 			CString strToken = ar.GetAt(i);
// 			
// 			CStringArray arToken;
// 			
// 			SplitString(strToken,":",arToken);
// 
// 			DrawBox(pDC,c,y,c+190,y+40,arToken.GetAt(0),FALSE,FW_NORMAL,TRUE,2);
// 
// 			c = c + 190;
// 			
// 			if (i == ar.GetSize() -1)
// 			{
// 				DrawBox(pDC,c,y,2100,y+40,arToken.GetAt(1),FALSE,FW_NORMAL,FALSE,1);
// 			}
// 			else
// 			{
// 				DrawBox(pDC,c,y,c+190,y+40,arToken.GetAt(1),FALSE,FW_NORMAL,FALSE,1);
// 			}
// 
// 			c = c + 190;
// 		}
// 	}
	//KSJ

	y = y + 40;

	
	CFont	font;
	CRect	rect;
	const	int	nFontSize = 30;
	
	font.CreateFont(nFontSize,			// nHeight
			0,				// nWidth
			0,				// nEscapement
			0,				// nOrientation
			FW_NORMAL,			// nWeight
			FALSE,				// bItalic
			FALSE,				// bUnderline
			0,				// cStrikeOut
			ANSI_CHARSET,			// nCharSet
			OUT_DEFAULT_PRECIS,		// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			DEFAULT_QUALITY,		// nQuality
			DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			"����ü");			// lpszFacename

	CFont*	pOldFont = (CFont*)pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(BLACKTEXT);


	rect.top = y + 70;
	rect.left = 0;
	rect.bottom = rect.top + 90;
	rect.right = 2100;

	CString text = "�غ� ȭ�� ������ �����ڷ�θ� Ȱ���Ͻð� ��� ��쿡�� ���� å�Ӽ��翡 ���� �����ڷ�δ� ����� �� ������ �˷��帳�ϴ�.";

	pDC->DrawText(text,-1,rect,DT_LEFT | DT_TOP);

	pDC->SelectObject(pOldFont);
	DeleteObject(&font);
}

//2012.06.21 KSJ �ι�° ���������� �������ϵ��� ����
void MainDlg::DrawGridHeader(CDC* pDC , bool bFirst)
{
	int	nTop = 670;
	int	nBottom = 720;

	//2012.06.21 KSJ �ι�° ���������� �������ϵ��� ����
	if (!bFirst)
	{
		nTop = 0;
		nBottom = 50;
	}
	//KSJ

	//nAlign = 0 : LEFT,nAlign = 1 : RIGHT,nAlign = 2 : CENTER

	const	int	nPosition[] = {190, 350, 510, 740, 930, 1090, 1280, 1420, 1582, 1744, 1906, 2068, 2230};
	int	nNextPos = 0;
	int	nPrePos = 0;

	DrawBox(pDC,0,nTop,nPosition[nNextPos++],nBottom,"�����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"��������",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"ü�����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���Աݾ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�򰡼���",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"BEP�ܰ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"������",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"������",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���ϼ���",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���ϸŵ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���ϸż�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���ϸŵ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���ϸż�",FALSE,FW_NORMAL,TRUE,2);

	nTop = 720;
	nBottom = 770;

	//2012.06.21 KSJ �ι�° ���������� �������ϵ��� ����
	if (!bFirst)
	{
		nTop = 50;
		nBottom = 100;
	}
	//KSJ

	nNextPos = 0;
	nPrePos = 0;
	DrawBox(pDC,0,nTop,nPosition[nNextPos++],nBottom,"�ܰ���",FALSE,FW_NORMAL,TRUE,2);    
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���簡",FALSE,FW_NORMAL,TRUE,2);    
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"��մܰ�",FALSE,FW_NORMAL,TRUE,2);  
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�򰡱ݾ�",FALSE,FW_NORMAL,TRUE,2);  
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���ͷ�",FALSE,FW_NORMAL,TRUE,2);    
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"BEP���ͷ�",FALSE,FW_NORMAL,TRUE,2); 
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�ſ�ݾ�",FALSE,FW_NORMAL,TRUE,2); 
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�����ȣ",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"���ϼ���",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�ŵ�����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�ż�����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�ŵ�����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,"�ż�����",FALSE,FW_NORMAL,TRUE,2);
}

void MainDlg::DrawGridLine(CDC* pDC, const CStringArray& ar, int y1)
{
	int	nTop = y1;
	int	nBottom = y1+50;
	int	nIndex = 0;

	const	int	nPosition[] = {190, 350, 510, 740, 930, 1090, 1280, 1420, 1582, 1744, 1906, 2068, 2230};
	int	nNextPos = 0;
	int	nPrePos = 0;

	//nAlign = 0 : LEFT,nAlign = 1 : RIGHT,nAlign = 2 : CENTER
	//2012.06.08 KSJ nNextPos-1�� ���� �ʾ� ���� nPrePos����������.. �翬�� �Ǿ���ϴµ� �� �ȵǴ��� �𸣰���..
	DrawBox(pDC,0,nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,0);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);

	nTop = y1+50;
	nBottom = y1+100;
	
	nNextPos = 0;
	nPrePos = 0;
	DrawBox(pDC,0,nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,0);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,0);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom,ar.GetAt(nIndex++),FALSE,FW_NORMAL,m_bFlag,1);

	m_bFlag = !m_bFlag;
}
/* 2012.04.23 KSJ 1������
void MainDlg::DrawGridHeader(CDC* pDC)
{
	DrawBox(pDC,0,355,320,400,"����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,320,355,500,400,"D+2�ܰ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,500,355,710,400,"������հ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,710,355,920,400,"���Աݾ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,920,355,1130,400,"���ͷ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1130,355,1340,400,"�ſ�ݾ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1340,355,1550,400,"������",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1550,355,1725,400,"���ϸŵ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1725,355,2100,400,"���ϸŵ�",FALSE,FW_NORMAL,TRUE,2);

	DrawBox(pDC,0,400,320,445,"�����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,320,400,500,445,"�ֹ�����",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,500,400,710,445,"���簡",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,710,400,920,445,"�򰡱ݾ�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,920,400,1130,445,"�򰡼���",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1130,400,1340,445,"",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1340,400,1550,445,"������",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1550,400,1725,445,"���ϸż�",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,1725,400,2100,445,"���ϸż�",FALSE,FW_NORMAL,TRUE,2);
}
  
void MainDlg::DrawGridLine(CDC* pDC,CStringArray& ar,int y1)
{
	DrawBox(pDC,0,y1,320,y1+45,ar.GetAt(0),FALSE,FW_NORMAL,m_bFlag,0);
	DrawBox(pDC,320,y1,500,y1+45,ar.GetAt(1),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,500,y1,710,y1+45,ar.GetAt(2),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,710,y1,920,y1+45,ar.GetAt(3),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,920,y1,1130,y1+45,ar.GetAt(4),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1130,y1,1340,y1+45,ar.GetAt(5),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1340,y1,1550,y1+45,ar.GetAt(6),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1550,y1,1725,y1+45,ar.GetAt(7),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1725,y1,2100,y1+45,ar.GetAt(8),FALSE,FW_NORMAL,m_bFlag,1);

	DrawBox(pDC,0,y1+45,320,y1+90,ar.GetAt(9),FALSE,FW_NORMAL,m_bFlag,0);
	DrawBox(pDC,320,y1+45,500,y1+90,ar.GetAt(10),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,500,y1+45,710,y1+90,ar.GetAt(11),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,710,y1+45,920,y1+90,ar.GetAt(12),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,920,y1+45,1130,y1+90,ar.GetAt(13),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1130,y1+45,1340,y1+90,ar.GetAt(14),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1340,y1+45,1550,y1+90,ar.GetAt(15),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1550,y1+45,1725,y1+90,ar.GetAt(16),FALSE,FW_NORMAL,m_bFlag,1);
	DrawBox(pDC,1725,y1+45,2100,y1+90,ar.GetAt(17),FALSE,FW_NORMAL,m_bFlag,1);

	m_bFlag = !m_bFlag;
}
*/

//Type:0=INFO LINE Type:1=Data Line
void MainDlg::FillSolidBox(CDC* pDC,int x,int y,int w,int h,int type)
{
	if (type == 0)
	{
		pDC->FillSolidRect(x+1,y+1,w-2,h-2,RGB(190,199,205));
	}
	else
	{
		pDC->FillSolidRect(x+1,y+1,w-2,h-2,HEADERBKCL);
	}
}

void MainDlg::DrawStuff(CDC* pDC)
{
	if (m_nNowPage == 1)
	{
		DrawTopBox(pDC);

		DrawGrid(pDC);
	}
	else
	{
		DrawGrid(pDC);
	}
}

BOOL MainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dataFile = "d2data.dat";

	m_editStatus.SetWindowText("�μ���.......");

	m_nTotalPages = 0;

	Run();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MainDlg::SplitString(CString &strData, CString strToken,CStringArray &ar) //
{
	if (strToken == "")
		return;

	CString sResult = strData;
	int	iFind = strData.Find(strToken); 

	if (iFind < 0)
	{
		ar.Add(sResult);
		return;
	}
	
	while (iFind > -1)
	{
		if (iFind > -1) 
		{
			sResult = strData.Left(iFind);
			strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength());  

			ar.Add(sResult);

			iFind = strData.Find(strToken);
		}
	}

	sResult = strData;

	ar.Add(sResult);
}

void MainDlg::LoadData()
{
	CStdioFile file;

	if (!file.Open(m_dataFile,CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
	{
		m_editStatus.SetWindowText("�μ��� ����Ÿ�� �����ϴ�.");
		return;
	}
	
	CString strLine;

	while (file.ReadString(strLine))
	{
		//2012.01.05 KSJ ������ �����Ͱ� ���̸� �����̽� ����..
		//strLine.TrimRight();
		//KSJ

		if (strLine != "")
			m_arData.Add(strLine);
	}
	file.Close();

	if (m_arData.GetSize() > 30)
	{
		m_nTotalPages = (m_arData.GetSize() - 30) / DATACOUNT + 1;
		
		if ((m_arData.GetSize() - 30) % DATACOUNT > 0)
		{
			m_nTotalPages += 1;
		}
	}
	else
	{
		m_nTotalPages = 1;
	}
}

void MainDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(100);

	SetProgressPos(1);

	Start();
	
	CDialog::OnTimer(nIDEvent);
}
