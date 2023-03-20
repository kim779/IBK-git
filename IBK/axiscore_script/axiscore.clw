; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgDown
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "axiscore.h"

ClassCount=3
Class1=CAxiscoreApp
Class2=CAxiscoreDlg

ResourceCount=4
Resource2=IDR_AXISTYPE
Resource3=IDD_AXISCORE_DIALOG
Resource1=IDR_MAINFRAME
Class3=CDlgDown
Resource4=IDD_DLG_DOWN

[CLS:CAxiscoreApp]
Type=0
HeaderFile=axiscore.h
ImplementationFile=axiscore.cpp
Filter=N
LastObject=CAxiscoreApp

[CLS:CAxiscoreDlg]
Type=0
HeaderFile=axiscoreDlg.h
ImplementationFile=axiscoreDlg.cpp
Filter=D
LastObject=CAxiscoreDlg



[DLG:IDD_AXISCORE_DIALOG]
Type=1
Class=CAxiscoreDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[MNU:IDR_AXISTYPE]
Type=1
Command1=ID_APP_ESETUP
Command2=ID_APP_EDITTOOL
Command3=ID_APP_EDITHOTKEY
Command4=ID_APP_TICKERSETUP
Command5=ID_APP_ORDERSETUP
Command6=ID_APP_ACCSETTING
Command7=ID_APP_CERTIFY
Command8=ID_APP_NEWSETUP
Command9=ID_APP_CALC
Command10=ID_APP_MEMO
Command11=ID_APP_TRSEARCH
Command12=ID_APP_USERSCREEN_SAVE
Command13=ID_APP_USETUPUPLOAD
Command14=ID_APP_USETUPDOWN
Command15=ID_APP_SAVEBMP_A
Command16=ID_APP_SAVEBMP
Command17=ID_APP_PRINT_SETUP
Command18=ID_APP_PRINT
Command19=ID_APP_LOCK
Command20=ID_APP_RECONNECT
Command21=ID_APP_EXIT
Command22=ID_VIEW_DB1
Command23=ID_VIEW_SCLIST
Command24=ID_VIEW_MODE
Command25=ID_VIEW_JCONTROL
Command26=ID_VIEW_VSCREEN1
Command27=ID_VIEW_VSCREEN2
Command28=ID_VIEW_VSCREEN3
Command29=ID_VIEW_VSCREEN4
Command30=ID_VIEW_CASCADE
Command31=ID_VIEW_TILE_HORZ
Command32=ID_VIEW_TILE_VERT
Command33=ID_APP_CLOSEA
Command34=ID_APP_CLOSEALL
Command35=ID_VIEW_FULLSCREEN
Command36=ID_APP_ABOUT
CommandCount=36

[DLG:IDD_DLG_DOWN]
Type=1
Class=CDlgDown
ControlCount=4
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_PROGRESS1,msctls_progress32,1350565888
Control4=IDC_STATIC_BITMAP,static,1342177294

[CLS:CDlgDown]
Type=0
HeaderFile=DlgDown.h
ImplementationFile=DlgDown.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgDown
VirtualFilter=dWC

