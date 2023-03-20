; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCertifyCtrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Certify.h"
CDK=Y

ClassCount=8
Class1=CCertifyCtrl
Class2=CCertifyPropPage

ResourceCount=9
LastPage=0
Resource2=IDD_GUIDE
Class3=CInvalid
Resource3=IDD_INVALID
Class4=COverPass
Resource4=IDD_PASSIN
Resource5=IDD_OVER
Class5=CGuide
Resource6=IDD_COUNT
Class6=CCountPass
Class7=CIssue
Resource7=IDD_PROPPAGE_CERTIFY
Class8=CPassInput
Resource8=IDD_ISSUE
Resource9=IDD_PASSOVER

[CLS:CCertifyCtrl]
Type=0
HeaderFile=CertifyCtl.h
ImplementationFile=CertifyCtl.cpp
Filter=W
LastObject=CCertifyCtrl
BaseClass=COleControl
VirtualFilter=wWC

[CLS:CCertifyPropPage]
Type=0
HeaderFile=CertifyPpg.h
ImplementationFile=CertifyPpg.cpp
Filter=D

[DLG:IDD_PROPPAGE_CERTIFY]
Type=1
Class=CCertifyPropPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_COUNT]
Type=1
Class=CCountPass
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_ISSUE,button,1342242816
Control6=IDC_OTHER,button,1342242816
Control7=IDC_STATIC,static,1342308864
Control8=IDC_COUNT,static,1342308866
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342177298
Control11=IDC_STATIC,static,1342308864

[CLS:CCountPass]
Type=0
HeaderFile=countPass.h
ImplementationFile=countPass.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCountPass

[DLG:IDD_GUIDE]
Type=1
Class=CGuide
ControlCount=12
Control1=IDC_TYPE,static,1342308864
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342177298
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,static,1342308864
Control9=IDCANCEL,button,1342242816
Control10=IDC_ISSUE,button,1342242816
Control11=IDC_OTHER,button,1342242816
Control12=IDC_STATIC,static,1342308864

[CLS:CGuide]
Type=0
HeaderFile=guide.h
ImplementationFile=guide.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGuide

[DLG:IDD_INVALID]
Type=1
Class=CInvalid
ControlCount=11
Control1=IDC_STATIC,static,1342308864
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC,static,1342177298
Control6=IDC_STATIC,static,1342308864
Control7=IDCANCEL,button,1342242816
Control8=IDC_ISSUE,button,1342242816
Control9=IDC_STATIC,static,1342308864
Control10=IDC_OTHER,button,1342242816
Control11=IDC_STATIC,static,1342308864

[CLS:CInvalid]
Type=0
HeaderFile=invalid.h
ImplementationFile=invalid.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CInvalid

[DLG:IDD_OVER]
Type=1
Class=COverPass
ControlCount=11
Control1=IDC_STATIC,static,1342308864
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC,static,1342177298
Control6=IDC_STATIC,static,1342308864
Control7=IDCANCEL,button,1342242816
Control8=IDC_ISSUE,button,1342242816
Control9=IDC_OTHER,button,1342242816
Control10=IDC_STATIC,static,1342308864
Control11=IDC_STATIC,static,1342308864

[CLS:COverPass]
Type=0
HeaderFile=overPass.h
ImplementationFile=overPass.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=COverPass

[DLG:IDD_ISSUE]
Type=1
Class=CIssue
ControlCount=12
Control1=IDC_STATIC,static,1342308864
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDCANCEL,button,1342242816
Control9=IDC_ISSUE,button,1342242816
Control10=IDC_OTHER,button,1342242816
Control11=IDC_STATIC,static,1342177298
Control12=IDC_STATIC,static,1342308864

[CLS:CIssue]
Type=0
HeaderFile=issue.h
ImplementationFile=issue.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CIssue

[DLG:IDD_PASSIN]
Type=1
Class=CPassInput
ControlCount=5
Control1=IDC_PASS,edit,1350631584
Control2=IDOK,button,1476460545
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STMSG,static,1342308352

[CLS:CPassInput]
Type=0
HeaderFile=passinput.h
ImplementationFile=passinput.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PASS

[DLG:IDD_PASSOVER]
Type=1
Class=CExitPass
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342177298

[CLS:CExitPass]
Type=0
HeaderFile=exitpass.h
ImplementationFile=exitpass.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CExitPass


