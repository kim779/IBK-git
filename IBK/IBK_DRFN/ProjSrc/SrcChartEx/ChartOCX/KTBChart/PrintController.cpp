// PrintController.cpp: implementation of the CPrintController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"
#include "PrintController.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for ML_LANGUAGE_RES
#include "DlgPrintCFg.h"
#include "KTBChartCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CPrintController::OnPrint(const CRect& rcChartRegion, CKTBChartCtrl* pCtrl)
{
	PopUpOsAndMyPrintDlg(rcChartRegion, pCtrl);
}

void CPrintController::PopUpMyPrintDlg(const CRect& rcChartRegion, CKTBChartCtrl* pCtrl)
{
	_MHWND( pCtrl->GetSafeHwnd());	
	ML_SET_LANGUAGE_RES();
	CDlgPrintCfg dlg;
	if(dlg.DoModal() == IDOK)
	{
		// get the default printer
		CPrintDialog dlgOs(FALSE);
		dlgOs.GetDefaults();

		// is a default printer set up?
		HDC hdcPrinter = dlgOs.GetPrinterDC();
		if (hdcPrinter == NULL)
		   AfxMessageBox(_T("Buy a printer!"));
		else
		{
		   CDC dcPrinter;
		   dcPrinter.Attach(hdcPrinter);

		   // call StartDoc() to begin printing
		   DOCINFO docinfo;
		   memset(&docinfo, 0, sizeof(docinfo));
		   docinfo.cbSize = sizeof(docinfo);
		   docinfo.lpszDocName = _T("NeoChart2001");

		   // if it fails, complain and exit gracefully
		   if (dcPrinter.StartDoc(&docinfo) < 0)
			  AfxMessageBox(_T("Printer Wouldn't initalize"));
		   else 
		   {
			  BOOL bColor = dlg.GetPrintColor();
			  if(!dlg.GetPrintRange()) //��ü Data ���
				  OnPrintTotalData(dcPrinter, bColor, rcChartRegion, pCtrl);
			  else //��ȭ�� ���
			  {  	
				  if(dlg.PrintAllByOnePageDataCount())//��ȭ���� �����Ͱ����� ������ ��ü������ ���
					  OnPrintbyUserSetDataCountperOnePge(dcPrinter, bColor, rcChartRegion, pCtrl);
				  else //��ȭ�� ��� 
					  OnPrintCurrentPage(dcPrinter, bColor, rcChartRegion, pCtrl);
			  }
			  dcPrinter.EndDoc();
		   }
		}
	  CDC::FromHandle(hdcPrinter)->DeleteDC();
	}
}

//�����ϰ� os ����Ʈ dialog�� ����Ҷ� ���
void CPrintController::PopUpOsPrintDlg(const CRect& rcChartRegion, CKTBChartCtrl* pCtrl)
{
	//ms ���� �����ϴ� print ���
	DWORD dwStyle;
	dwStyle = PD_ALLPAGES | PD_USEDEVMODECOPIES;
//	dwStyle = PD_SELECTION | PD_USEDEVMODECOPIES;
	
	CPrintDialog dlg(FALSE, dwStyle);
	if(dlg.DoModal() != IDOK)
		return;
	 
	// is a default printer set up?
	HDC hdcPrinter = dlg.GetPrinterDC();
	if (hdcPrinter == NULL) 
	   AfxMessageBox(_T("Buy printer!"));
	else 
	{
	   // create a CDC and attach it to the default printer
	   CDC dcPrinter;
	   dcPrinter.Attach(hdcPrinter);

	   // call StartDoc() to begin printing
	   DOCINFO docinfo;
	   memset(&docinfo, 0, sizeof(docinfo));
	   docinfo.cbSize = sizeof(docinfo);
	   docinfo.lpszDocName = _T("NeoChart2001");

	   // if it fails, complain and exit gracefully
	   if (dcPrinter.StartDoc(&docinfo) < 0)
		  AfxMessageBox(_T("Printer Wouldn't initalize"));
	   else
	   {
		  //��ü Data ���
		  if(!dlg.PrintAll()) 
			  OnPrintTotalData(dcPrinter, 0, rcChartRegion, pCtrl);
		  else    //��ȭ�鿡 ��� 	
			  OnPrintCurrentPage(dcPrinter, 0, rcChartRegion, pCtrl);
		  dcPrinter.EndDoc();
	   }
	}
	CDC::FromHandle(hdcPrinter)->DeleteDC();
}

//nami 6.8
void CPrintController::OnPrintTotalData(CDC &dcPrinter, 
										const int nPrintColor,
										const CRect& rcChartRegion,
										CKTBChartCtrl* pCtrl)
{
	// start a page
	if (dcPrinter.StartPage() < 0) 
	{
	 AfxMessageBox(_T("Could not start page"));
	 dcPrinter.AbortDoc();
	}
	else 
	{
		int nHorz = dcPrinter.GetDeviceCaps(HORZRES);
		int nVert = dcPrinter.GetDeviceCaps(VERTRES);
		CRect rect(0,0,nHorz,nVert);

		// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
		pCtrl->LockScrollUIUpdateWithScrollInfo( TRUE);

		pCtrl->SetWholeView(TRUE);
		OnDraw(&dcPrinter, nPrintColor, rect, rcChartRegion, pCtrl);

		// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
		pCtrl->LockScrollUIUpdateWithScrollInfo( FALSE);

		dcPrinter.EndPage();
	}
}

void CPrintController::OnPrintCurrentPage(CDC &dcPrinter,
										  const int nPrintColor,
										  const CRect& rcChartRegion,
										  CKTBChartCtrl* pCtrl)
{
	// start a page
	if (dcPrinter.StartPage() < 0)
	{
		AfxMessageBox(_T("Could not start page"));
		dcPrinter.AbortDoc();
	} 
	else
	{
		int nHorz = dcPrinter.GetDeviceCaps(HORZRES);
		int nVert = dcPrinter.GetDeviceCaps(VERTRES);
		CRect rect(0,0,nHorz,nVert);

		// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
		pCtrl->LockScrollUIUpdateWithScrollInfo( TRUE);

		OnDraw(&dcPrinter, nPrintColor, rect, rcChartRegion, pCtrl);

		// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
		pCtrl->LockScrollUIUpdateWithScrollInfo( FALSE);

		dcPrinter.EndPage();
	}
}

void CPrintController::OnPrintbyUserSetDataCountperOnePge(CDC &dcPrinter,
										  const int nPrintColor,
										  const CRect& rcChartRegion,
										  CKTBChartCtrl* pCtrl)
{
	int nHorz = dcPrinter.GetDeviceCaps(HORZRES);
	int nVert = dcPrinter.GetDeviceCaps(VERTRES);
	CRect rect(0,0,nHorz,nVert);

	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	pCtrl->LockScrollUIUpdateWithScrollInfo( TRUE);

	BOOL bNextPage = pCtrl->ScrollTo( 0, FALSE);
	while(bNextPage) 
	{
		// start a page
		if (dcPrinter.StartPage() < 0)
		{
			AfxMessageBox(_T("Could not start page"));
			dcPrinter.AbortDoc();
		} 
		else OnDraw(&dcPrinter, nPrintColor, rect, rcChartRegion, pCtrl);

		dcPrinter.EndPage();
		bNextPage = pCtrl->SetScrollPosToNextPage();
	}

	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	pCtrl->LockScrollUIUpdateWithScrollInfo( FALSE);
}

void CPrintController::OnDraw(CDC* pdc, 
							  const int nPrintColor,
							  const CRect& rcDestination, 
							  const CRect& rcChartRegion,
							  CKTBChartCtrl* pCtrl)
{
	//	bitmap create
	// �μ� ����� ���� ��� ���� ���� - ojtaso (20070921)
//	CClientDC dc(pCtrl); 
	CRect rcSource(rcChartRegion);

	CDC memoryDC;
	CBitmap bufferBitmap;
	// �μ� ����� ���� ��� ���� ���� - ojtaso (20070921)
	memoryDC.CreateCompatibleDC(pdc);
	// �μ� ����� ���� ��� ���� ���� - ojtaso (20070921)
	bufferBitmap.CreateCompatibleBitmap(pdc, rcSource.Width(), rcSource.Height());

	CBitmap* pOldBitmap = (CBitmap*) memoryDC.SelectObject(&bufferBitmap);

	pCtrl->OnDrawForPrint(memoryDC, bufferBitmap, rcSource, nPrintColor ? TRUE : FALSE);

	pdc->StretchBlt(rcDestination.left, rcDestination.top, rcDestination.Width(), rcDestination.Height(), &memoryDC, 
					0, 0, rcSource.Width(), rcSource.Height(), SRCCOPY);

	memoryDC.SelectObject(pOldBitmap);
	bufferBitmap.DeleteObject();
	memoryDC.DeleteDC();
}

void CPrintController::PopUpOsAndMyPrintDlg(const CRect& rcChartRegion,
											CKTBChartCtrl* pCtrl)
{
	_MHWND( pCtrl->GetSafeHwnd());
	ML_SET_LANGUAGE_RES();
	CDlgPrintCfg dlg;
	if(dlg.DoModal() != IDOK)
		return;

	HDC hdcPrinter = dlg.GetHDC();
	if (hdcPrinter == NULL)
	   AfxMessageBox(_T("Buy a printer!"));
	else
	{
	   CDC dcPrinter;
	   dcPrinter.Attach(hdcPrinter);

	   // call StartDoc() to begin printing
	   DOCINFO docinfo;
	   memset(&docinfo, 0, sizeof(docinfo));
	   docinfo.cbSize = sizeof(docinfo);
	   docinfo.lpszDocName = _T("NeoChart2001");

	   // if it fails, complain and exit gracefully
	   if (dcPrinter.StartDoc(&docinfo) < 0)
		  AfxMessageBox(_T("Printer Wouldn't initalize"));
	   else 
	   {
		  BOOL bColor = dlg.GetPrintColor();
		  if(!dlg.GetPrintRange()) //��ü Data ���
			  OnPrintTotalData(dcPrinter, bColor, rcChartRegion, pCtrl);
		  else //��ȭ�� ���
		  {  	
			  if(dlg.PrintAllByOnePageDataCount())//��ȭ���� �����Ͱ����� ������ ��ü������ ���
				  OnPrintbyUserSetDataCountperOnePge(dcPrinter, bColor, rcChartRegion, pCtrl);
			  else //��ȭ�� ��� 
				  OnPrintCurrentPage(dcPrinter, bColor, rcChartRegion, pCtrl);
		  }
		  dcPrinter.EndDoc();				  
	   }
		CDC::FromHandle(hdcPrinter)->DeleteDC();
	}
}
