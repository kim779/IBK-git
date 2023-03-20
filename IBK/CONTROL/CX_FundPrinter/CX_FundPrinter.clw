; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=5
Class1=CCX_FundPrinterApp
LastClass=CMainWnd
NewFileInclude2=#include "CX_FundPrinter.h"
ResourceCount=1
NewFileInclude1=#include "stdafx.h"
Class2=CMainDlg
LastTemplate=generic CWnd
Class3=CFPrinter
Class4=CPrintDlg
Class5=CMainWnd
Resource1=IDD_DIALOG2

[CLS:CCX_FundPrinterApp]
Type=0
HeaderFile=CX_FundPrinter.h
ImplementationFile=CX_FundPrinter.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CCX_FundPrinterApp

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMainDlg

[CLS:CFPrinter]
Type=0
HeaderFile=FPrinter.h
ImplementationFile=FPrinter.cpp
BaseClass=generic CWnd
Filter=W

[DLG:IDD_DIALOG2]
Type=1
Class=CPrintDlg
ControlCount=1
Control1=IDC_BUTTON1,button,1342275584

[CLS:CPrintDlg]
Type=0
HeaderFile=PrintDlg.h
ImplementationFile=PrintDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON1
VirtualFilter=dWC

[CLS:CMainWnd]
Type=0
HeaderFile=MainWnd.h
ImplementationFile=MainWnd.cpp
BaseClass=CWnd
Filter=W
LastObject=CMainWnd
VirtualFilter=WC

