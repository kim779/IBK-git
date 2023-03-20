; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CxSystem
LastTemplate=CHtmlView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Wizard.h"
CDK=Y

ClassCount=7
Class1=CWizardCtrl
Class2=CWizardPropPage

ResourceCount=1
LastPage=0
Class3=CTips
Class4=CHistory
Class5=CxScreen
Class6=CDde
Class7=CxSystem
Resource1=IDD_PROPPAGE_WIZARD

[CLS:CWizardCtrl]
Type=0
HeaderFile=WizardCtl.h
ImplementationFile=WizardCtl.cpp
Filter=W
BaseClass=COleControl
VirtualFilter=wWC
LastObject=CWizardCtrl

[CLS:CWizardPropPage]
Type=0
HeaderFile=WizardPpg.h
ImplementationFile=WizardPpg.cpp
Filter=D
LastObject=IDC_RADIO1
BaseClass=COlePropertyPage
VirtualFilter=idWC

[DLG:IDD_PROPPAGE_WIZARD]
Type=1
Class=CWizardPropPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:CHistory]
Type=0
HeaderFile=History.h
ImplementationFile=History.cpp
BaseClass=CListBox
Filter=W
LastObject=CHistory
VirtualFilter=bWC

[CLS:CxScreen]
Type=0
HeaderFile=xscreen.h
ImplementationFile=xscreen.cpp
BaseClass=CCmdTarget
Filter=N
LastObject=CxScreen
VirtualFilter=C

[CLS:CxSystem]
Type=0
HeaderFile=xsystem.h
ImplementationFile=xsystem.cpp
BaseClass=CCmdTarget
Filter=N
LastObject=CxSystem
VirtualFilter=C

[CLS:CDde]
Type=0
HeaderFile=Dde.h
ImplementationFile=Dde.cpp
BaseClass=CWnd
Filter=W
LastObject=CDde
VirtualFilter=WC

[CLS:CTips]
Type=0
HeaderFile=Tips.h
ImplementationFile=Tips.cpp
BaseClass=CWnd
Filter=W
LastObject=CTips
VirtualFilter=WC

